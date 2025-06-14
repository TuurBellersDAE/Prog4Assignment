#include "GhostBehavior.h"
#include "GameObject.h"
#include "GridComponent.h"
#include <algorithm>
#include <random>
#include "GhostAIControllerComponent.h"
#include "GhostState.h"

glm::ivec2 dae::FindNearestWalkableCell(const GridComponent* grid, glm::ivec2 cell)
{
	if (grid->IsCellWalkable(cell.x, cell.y))
		return cell;

	// Simple 4-directional search for nearest walkable cell
	static const glm::ivec2 directions[] = {
		{1,0}, {-1,0}, {0,1}, {0,-1}
	};
	for (int radius = 1; radius < 5; ++radius) // Search up to 4 cells away
	{
		for (const auto& dir : directions)
		{
			glm::ivec2 neighbor = cell + dir * radius;
			if (neighbor.x >= 0 && neighbor.x < grid->GetGridWidth() &&
				neighbor.y >= 0 && neighbor.y < grid->GetGridHeight() &&
				grid->IsCellWalkable(neighbor.x, neighbor.y))
			{
				return neighbor;
			}
		}
	}
	// Fallback: return original cell
	return cell;
}

glm::ivec2 dae::GetRandomNeighbor(
	const GridComponent* grid,
	int cellX, int cellY,
	const glm::ivec2& currentDirection)
{
	static const glm::ivec2 directions[] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
	std::vector<glm::ivec2> candidates;
	glm::ivec2 reverseDir = { -currentDirection.x, -currentDirection.y };
	glm::ivec2 forwardCell = { cellX + currentDirection.x, cellY + currentDirection.y };
	glm::ivec2 reverseCell = { cellX + reverseDir.x, cellY + reverseDir.y };
	std::vector<glm::ivec2> reverseCandidates;

	for (const auto& dir : directions)
	{
		int nx = cellX + dir.x;
		int ny = cellY + dir.y;

		// Skip the current cell
		if (nx == cellX && ny == cellY)
			continue;

		if (!grid->IsCellWalkable(nx, ny))
			continue;

		if (currentDirection != glm::ivec2{ 0, 0 } && dir == reverseDir)
		{
			reverseCandidates.emplace_back(nx, ny); // Save reversal for fallback
		}
		else
		{
			candidates.emplace_back(nx, ny); // Non-reverse options
		}
	}

	// 1. If there are other candidates, randomly pick one
	if (!candidates.empty()) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, static_cast<int>(candidates.size()) - 1);
		return candidates[dis(gen)];
	}

	// 2. If no candidates, but cell ahead is walkable, target cell ahead
	if (currentDirection != glm::ivec2{ 0, 0 } &&
		grid->IsCellWalkable(forwardCell.x, forwardCell.y))
	{
		return forwardCell;
	}

	// 3. Only reverse if no candidates and cell ahead is not walkable
	if (!reverseCandidates.empty()) {
		std::cout << "[GhostBehavior] Only reverse move possible from (" << cellX << "," << cellY << ") to ("
			<< reverseCell.x << "," << reverseCell.y << ")." << std::endl;
		return reverseCandidates.front();
	}

	// No move possible, stay in place
	std::cout << "[GhostBehavior] No move possible from (" << cellX << "," << cellY << "). Neighbors: ";
	for (const auto& dir : directions) {
		int nx = cellX + dir.x;
		int ny = cellY + dir.y;
		std::cout << "(" << nx << "," << ny << "):" << grid->IsCellWalkable(nx, ny) << " ";
	}
	std::cout << std::endl;
	return { cellX, cellY };
}


glm::ivec2 dae::BlinkyBehavior::GetTargetCell(const GameObject* ghost, const GameObject* player,
	const GridComponent* grid, GhostAIControllerComponent* controller)
{
	auto* state = controller->GetCurrentState();
	int gridW = grid->GetGridWidth();
	int gridH = grid->GetGridHeight();

	glm::vec3 playerPos = player->GetLocalPosition();
	int cellSize = grid->GetCellSize();
	int cellX = static_cast<int>(playerPos.x + 0.5f * cellSize) / cellSize;
	int cellY = static_cast<int>(playerPos.y + 0.5f * cellSize) / cellSize;

	if (dynamic_cast<const GhostChaseState*>(state))
	{
		// Chase: target player
		return { cellX, cellY };
	}
	else if (dynamic_cast<const GhostScatterState*>(state))
	{
		// Scatter: top-right corner
		return { 26, 1 };
	}
	else if (dynamic_cast<const GhostFrightenedState*>(state))
	{
		auto spriteSize = controller->GetSpriteSize();
		float centerX = ghost->GetLocalPosition().x + 0.5f * spriteSize;
		float centerY = ghost->GetLocalPosition().y + 0.5f * spriteSize;

		int ghostCellX = static_cast<int>(centerX / cellSize + 0.5f);
		int ghostCellY = static_cast<int>(centerY / cellSize + 0.5f);

		glm::ivec2 currentTarget = controller->GetCurrentTargetCell();

		bool needNewTarget = (currentTarget.x < 0 || currentTarget.y < 0);

		if (!needNewTarget)
		{
			// Check if ghost's center is inside the current target cell
			int cellLeft = currentTarget.x * cellSize;
			int cellRight = (currentTarget.x + 1) * cellSize;
			int cellTop = currentTarget.y * cellSize;
			int cellBottom = (currentTarget.y + 1) * cellSize;

			if (centerX >= cellLeft && centerX < cellRight &&
				centerY >= cellTop && centerY < cellBottom)
			{
				needNewTarget = true;
			}
		}

		glm::ivec2 nextTarget;
		if (needNewTarget)
		{
			// Always pick from the current target cell (if valid), not the previous cell
			int fromX = ghostCellX;
			int fromY = ghostCellY;
			if (currentTarget.x >= 0 && currentTarget.y >= 0)
			{
				fromX = currentTarget.x;
				fromY = currentTarget.y;
			}
			glm::vec3 dir = ghost->GetDirection();
			glm::ivec2 currentDir = glm::ivec2(dir.x, dir.y);
			nextTarget = GetRandomNeighbor(grid, fromX, fromY, currentDir);
			controller->SetCurrentTargetCell(nextTarget);
		}
		else
		{
			nextTarget = currentTarget;
		}

		return nextTarget;
	}
	else if (dynamic_cast<const GhostEatenState*>(state))
	{
		// Eaten: return to ghost house (center)
		return { gridW / 2, gridH / 2 };
	}
	return { cellX, cellY };
}

glm::ivec2 dae::PinkyBehavior::GetTargetCell(const GameObject* ghost, const GameObject* player,
	const GridComponent* grid, GhostAIControllerComponent* controller)
{
	auto* state = controller->GetCurrentState();
	int gridW = grid->GetGridWidth();
	int gridH = grid->GetGridHeight();

	glm::vec3 playerPos = player->GetLocalPosition();
	int cellSize = grid->GetCellSize();
	int cellX = static_cast<int>(playerPos.x + 0.5f * cellSize) / cellSize;
	int cellY = static_cast<int>(playerPos.y + 0.5f * cellSize) / cellSize;

	if (dynamic_cast<const GhostChaseState*>(state))
	{
		// Chase: 4 cells ahead of player
		glm::ivec2 target = { cellX + 4, cellY };
		target.x = std::clamp(target.x, 0, gridW - 1);
		target.y = std::clamp(target.y, 0, gridH - 1);
		return dae::FindNearestWalkableCell(grid, target);
	}
	else if (dynamic_cast<const GhostScatterState*>(state))
	{
		// Scatter: top-left corner
		return { 1, 1 };
	}
	else if (dynamic_cast<const GhostFrightenedState*>(state))
	{
		auto spriteSize = controller->GetSpriteSize();
		float centerX = ghost->GetLocalPosition().x + 0.5f * spriteSize;
		float centerY = ghost->GetLocalPosition().y + 0.5f * spriteSize;

		int ghostCellX = static_cast<int>(centerX / cellSize + 0.5f);
		int ghostCellY = static_cast<int>(centerY / cellSize + 0.5f);

		glm::ivec2 currentTarget = controller->GetCurrentTargetCell();

		bool needNewTarget = (currentTarget.x < 0 || currentTarget.y < 0);

		if (!needNewTarget)
		{
			// Check if ghost's center is inside the current target cell
			int cellLeft = currentTarget.x * cellSize;
			int cellRight = (currentTarget.x + 1) * cellSize;
			int cellTop = currentTarget.y * cellSize;
			int cellBottom = (currentTarget.y + 1) * cellSize;

			if (centerX >= cellLeft && centerX < cellRight &&
				centerY >= cellTop && centerY < cellBottom)
			{
				needNewTarget = true;
			}
		}

		glm::ivec2 nextTarget;
		if (needNewTarget)
		{
			// Always pick from the current target cell (if valid), not the previous cell
			int fromX = ghostCellX;
			int fromY = ghostCellY;
			if (currentTarget.x >= 0 && currentTarget.y >= 0)
			{
				fromX = currentTarget.x;
				fromY = currentTarget.y;
			}
			glm::vec3 dir = ghost->GetDirection();
			glm::ivec2 currentDir = glm::ivec2(dir.x, dir.y);
			nextTarget = GetRandomNeighbor(grid, fromX, fromY, currentDir);
			controller->SetCurrentTargetCell(nextTarget);
		}
		else
		{
			nextTarget = currentTarget;
		}

		return nextTarget;
	}
	else if (dynamic_cast<const GhostEatenState*>(state))
	{
		return { gridW / 2, gridH / 2 };
	}
	return { cellX, cellY };
}

glm::ivec2 dae::InkyBehavior::GetTargetCell(const GameObject* ghost, const GameObject* player,
	const GridComponent* grid, GhostAIControllerComponent* controller)
{
	auto* state = controller->GetCurrentState();
	int gridW = grid->GetGridWidth();
	int gridH = grid->GetGridHeight();

	glm::vec3 playerPos = player->GetLocalPosition();
	int cellSize = grid->GetCellSize();
	int cellX = static_cast<int>(playerPos.x + 0.5f * cellSize) / cellSize;
	int cellY = static_cast<int>(playerPos.y + 0.5f * cellSize) / cellSize;

	if (dynamic_cast<const GhostChaseState*>(state))
	{
		// Chase: 2 cells ahead of player
		glm::ivec2 target = { cellX + 2, cellY };
		target.x = std::clamp(target.x, 0, gridW - 1);
		target.y = std::clamp(target.y, 0, gridH - 1);
		return dae::FindNearestWalkableCell(grid, target);
	}
	else if (dynamic_cast<const GhostScatterState*>(state))
	{
		// Scatter: bottom-right corner
		return { 26, 29 };
	}
	else if (dynamic_cast<const GhostFrightenedState*>(state))
	{
		auto spriteSize = controller->GetSpriteSize();
		float centerX = ghost->GetLocalPosition().x + 0.5f * spriteSize;
		float centerY = ghost->GetLocalPosition().y + 0.5f * spriteSize;

		int ghostCellX = static_cast<int>(centerX / cellSize + 0.5f);
		int ghostCellY = static_cast<int>(centerY / cellSize + 0.5f);

		glm::ivec2 currentTarget = controller->GetCurrentTargetCell();

		bool needNewTarget = (currentTarget.x < 0 || currentTarget.y < 0);

		if (!needNewTarget)
		{
			// Check if ghost's center is inside the current target cell
			int cellLeft = currentTarget.x * cellSize;
			int cellRight = (currentTarget.x + 1) * cellSize;
			int cellTop = currentTarget.y * cellSize;
			int cellBottom = (currentTarget.y + 1) * cellSize;

			if (centerX >= cellLeft && centerX < cellRight &&
				centerY >= cellTop && centerY < cellBottom)
			{
				needNewTarget = true;
			}
		}

		glm::ivec2 nextTarget;
		if (needNewTarget)
		{
			// Always pick from the current target cell (if valid), not the previous cell
			int fromX = ghostCellX;
			int fromY = ghostCellY;
			if (currentTarget.x >= 0 && currentTarget.y >= 0)
			{
				fromX = currentTarget.x;
				fromY = currentTarget.y;
			}
			glm::vec3 dir = ghost->GetDirection();
			glm::ivec2 currentDir = glm::ivec2(dir.x, dir.y);
			nextTarget = GetRandomNeighbor(grid, fromX, fromY, currentDir);
			controller->SetCurrentTargetCell(nextTarget);
		}
		else
		{
			nextTarget = currentTarget;
		}

		return nextTarget;
	}
	else if (dynamic_cast<const GhostEatenState*>(state))
	{
		return { gridW / 2, gridH / 2 };
	}
	return { cellX, cellY };
}

glm::ivec2 dae::ClydeBehavior::GetTargetCell(const GameObject* ghost, const GameObject* player,
	const GridComponent* grid, GhostAIControllerComponent* controller)
{
	auto* state = controller->GetCurrentState();
	int gridW = grid->GetGridWidth();
	int gridH = grid->GetGridHeight();

	glm::vec3 playerPos = player->GetLocalPosition();
	int cellSize = grid->GetCellSize();
	int cellX = static_cast<int>(playerPos.x + 0.5f * cellSize) / cellSize;
	int cellY = static_cast<int>(playerPos.y + 0.5f * cellSize) / cellSize;

	if (dynamic_cast<const GhostChaseState*>(state))
	{
		// Chase: 1 cell ahead of player
		glm::ivec2 target = { cellX + 1, cellY };
		target.x = std::clamp(target.x, 0, gridW - 1);
		target.y = std::clamp(target.y, 0, gridH - 1);
		return dae::FindNearestWalkableCell(grid, target);
	}
	else if (dynamic_cast<const GhostScatterState*>(state))
	{
		// Scatter: bottom-left corner
		return { 1, 29 };
	}
	else if (dynamic_cast<const GhostFrightenedState*>(state))
	{
		auto spriteSize = controller->GetSpriteSize();
		float centerX = ghost->GetLocalPosition().x + 0.5f * spriteSize;
		float centerY = ghost->GetLocalPosition().y + 0.5f * spriteSize;

		int ghostCellX = static_cast<int>(centerX / cellSize + 0.5f);
		int ghostCellY = static_cast<int>(centerY / cellSize + 0.5f);

		glm::ivec2 currentTarget = controller->GetCurrentTargetCell();

		bool needNewTarget = (currentTarget.x < 0 || currentTarget.y < 0);

		if (!needNewTarget)
		{
			// Check if ghost's center is inside the current target cell
			int cellLeft = currentTarget.x * cellSize;
			int cellRight = (currentTarget.x + 1) * cellSize;
			int cellTop = currentTarget.y * cellSize;
			int cellBottom = (currentTarget.y + 1) * cellSize;

			if (centerX >= cellLeft && centerX < cellRight &&
				centerY >= cellTop && centerY < cellBottom)
			{
				needNewTarget = true;
			}
		}

		glm::ivec2 nextTarget;
		if (needNewTarget)
		{
			// Always pick from the current target cell (if valid), not the previous cell
			int fromX = ghostCellX;
			int fromY = ghostCellY;
			if (currentTarget.x >= 0 && currentTarget.y >= 0)
			{
				fromX = currentTarget.x;
				fromY = currentTarget.y;
			}
			glm::vec3 dir = ghost->GetDirection();
			glm::ivec2 currentDir = glm::ivec2(dir.x, dir.y);
			nextTarget = GetRandomNeighbor(grid, fromX, fromY, currentDir);
			controller->SetCurrentTargetCell(nextTarget);
		}
		else
		{
			nextTarget = currentTarget;
		}

		return nextTarget;
	}
	else if (dynamic_cast<const GhostEatenState*>(state))
	{
		return { gridW / 2, gridH / 2 };
	}
	return { cellX, cellY };
}


