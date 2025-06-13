#include "GhostBehavior.h"
#include "GameObject.h"
#include "GridComponent.h"
#include <algorithm>

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


glm::ivec2 dae::BlinkyBehavior::GetTargetCell(const GameObject* /*ghost*/, const GameObject* player, const GridComponent* grid)
{
	glm::vec3 playerPos = player->GetLocalPosition();
	int cellSize = grid->GetCellSize();
	int cellX = static_cast<int>(playerPos.x + 0.5f * cellSize) / cellSize;
	int cellY = static_cast<int>(playerPos.y + 0.5f * cellSize) / cellSize;
	return { cellX, cellY };
}

glm::ivec2 dae::PinkyBehavior::GetTargetCell(const GameObject* /*ghost*/, const GameObject* player, const GridComponent* grid)
{
	glm::vec3 playerPos = player->GetLocalPosition();
	int cellSize = grid->GetCellSize();
	int cellX = static_cast<int>(playerPos.x + 0.5f * cellSize) / cellSize;
	int cellY = static_cast<int>(playerPos.y + 0.5f * cellSize) / cellSize;
	// Pinky moves to the position 4 cells ahead of the player
	glm::ivec2 target = { cellX + 4, cellY };
	// Clamp to grid bounds
	target.x = std::clamp(target.x, 0, grid->GetGridWidth() - 1);
	target.y = std::clamp(target.y, 0, grid->GetGridHeight() - 1);
	// Ensure target is walkable
	return dae::FindNearestWalkableCell(grid, target);
}

glm::ivec2 dae::InkyBehavior::GetTargetCell(const GameObject* /*ghost*/, const GameObject* player, const GridComponent* grid)
{
	glm::vec3 playerPos = player->GetLocalPosition();
	int cellSize = grid->GetCellSize();
	int cellX = static_cast<int>(playerPos.x + 0.5f * cellSize) / cellSize;
	int cellY = static_cast<int>(playerPos.y + 0.5f * cellSize) / cellSize;
	// Inky moves to the position 2 cells ahead of the player
	glm::ivec2 target = { cellX + 2, cellY };
	target.x = std::clamp(target.x, 0, grid->GetGridWidth() - 1);
	target.y = std::clamp(target.y, 0, grid->GetGridHeight() - 1);
	return dae::FindNearestWalkableCell(grid, target);
}

glm::ivec2 dae::ClydeBehavior::GetTargetCell(const GameObject* /*ghost*/, const GameObject* player, const GridComponent* grid)
{
	glm::vec3 playerPos = player->GetLocalPosition();
	int cellSize = grid->GetCellSize();
	int cellX = static_cast<int>(playerPos.x + 0.5f * cellSize) / cellSize;
	int cellY = static_cast<int>(playerPos.y + 0.5f * cellSize) / cellSize;
	// Clyde moves to the position 1 cell ahead of the player
	glm::ivec2 target = { cellX + 1, cellY };
	target.x = std::clamp(target.x, 0, grid->GetGridWidth() - 1);
	target.y = std::clamp(target.y, 0, grid->GetGridHeight() - 1);
	return dae::FindNearestWalkableCell(grid, target);
}

