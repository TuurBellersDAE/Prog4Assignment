#include "GhostState.h"
#include "GhostAIControllerComponent.h"
#include "GhostBehavior.h"
#include "PathFinding.h"
#include "SpriteAnimatorComponent.h"
#include "Timer.h"

#include <iostream>

void dae::GhostMoveAndAnimate(
	GhostAIControllerComponent* controller,
	const glm::ivec2& targetCell)
{
	auto* owner = controller->GetOwner();

	// After setting direction:
	auto lastDir = owner->GetDirection();
	if (lastDir != glm::vec3(0.f)) {
		controller->SetLastDirection(glm::ivec2(static_cast<int>(lastDir.x), static_cast<int>(lastDir.y)));
	}

	auto* grid = controller->GetGrid();
	int spriteSize = controller->GetSpriteSize();

	if (!owner || !grid) return;

	int cellSize = grid->GetCellSize();
	glm::vec3 position = owner->GetLocalPosition();
	glm::vec2 center = { position.x + 0.5f * spriteSize, position.y + 0.5f * spriteSize };
	int cellX = static_cast<int>(center.x) / cellSize;
	int cellY = static_cast<int>(center.y) / cellSize;

	float cellCenterX = cellX * cellSize + cellSize / 2.f;
	float cellCenterY = cellY * cellSize + cellSize / 2.f;

	const float epsilon = 1.f;
	bool isCentered =
		(std::abs(center.x - cellCenterX) < epsilon) &&
		(std::abs(center.y - cellCenterY) < epsilon);

	glm::ivec2 ghostCell(cellX, cellY);

	if (isCentered)
	{
		owner->SetWorldPosition(
			cellCenterX - spriteSize / 2.f,
			cellCenterY - spriteSize / 2.f,
			0.f
		);

		int gridW = grid->GetGridWidth();
		int gridH = grid->GetGridHeight();

		if (ghostCell.x < 0 || ghostCell.x >= gridW ||
			ghostCell.y < 0 || ghostCell.y >= gridH ||
			targetCell.x < 0 || targetCell.x >= gridW ||
			targetCell.y < 0 || targetCell.y >= gridH)
		{
			// Instead of stopping, pick a random walkable neighbor
			glm::ivec2 lastDirection = controller->GetLastDirection();
			glm::ivec2 fallback = dae::GetRandomNeighbor(grid, ghostCell.x, ghostCell.y, lastDirection);
			glm::ivec2 dir = fallback - ghostCell;
			if (dir != glm::ivec2(0, 0))
				owner->SetDirection(glm::vec3(dir.x, dir.y, 0.f));
			// else: stay in place, but do not set direction to zero
			return;
		}

		auto path = dae::FindPathBFS(grid, ghostCell, targetCell);

		if (!path.empty()) {
			glm::ivec2 nextCell = path.front();
			glm::ivec2 dir = nextCell - ghostCell;
			owner->SetDirection(glm::vec3(dir.x, dir.y, 0.f));
		}
		else {
			// Fallback: pick a random walkable neighbor
			glm::ivec2 lastDirection = controller->GetLastDirection();
			glm::ivec2 fallback = dae::GetRandomNeighbor(grid, ghostCell.x, ghostCell.y, lastDirection);
			glm::ivec2 dir = fallback - ghostCell;
			if (dir != glm::ivec2(0, 0))
				owner->SetDirection(glm::vec3(dir.x, dir.y, 0.f));
			// else: stay in place, but do not set direction to zero
		}
	}

	// --- Movement logic (center-to-center) ---
	auto currentDir = owner->GetDirection();
	if (currentDir != glm::vec3(0.f)) {
		int nextCellX = cellX + static_cast<int>(currentDir.x);
		int nextCellY = cellY + static_cast<int>(currentDir.y);

		bool canMove = !isCentered || grid->IsCellWalkable(nextCellX, nextCellY);

		float deltaTime = Timer::GetInstance().GetDeltaTime();
		if (canMove)
		{
			position.x += currentDir.x * deltaTime * owner->GetSpeed();
			position.y += currentDir.y * deltaTime * owner->GetSpeed();
			owner->SetWorldPosition(position);
		}
		else
		{
			owner->SetWorldPosition(
				cellCenterX - spriteSize / 2.f,
				cellCenterY - spriteSize / 2.f,
				0.f
			);
			// Instead of stopping, pick a random walkable neighbor
			glm::ivec2 lastDirection = controller->GetLastDirection();
			glm::ivec2 fallback = dae::GetRandomNeighbor(grid, cellX, cellY, lastDirection);
			glm::ivec2 dir = fallback - glm::ivec2(cellX, cellY);
			if (dir != glm::ivec2(0, 0))
				owner->SetDirection(glm::vec3(dir.x, dir.y, 0.f));
			// else: do not set direction to zero
		}
	}

	// --- Sprite animation logic (directional ghost textures) ---
	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		if (currentDir == glm::vec3(-1.f, 0.f, 0.f))
			animator->SetCurrentRow(1);
		else if (currentDir == glm::vec3(1.f, 0.f, 0.f))
			animator->SetCurrentRow(0);
		else if (currentDir == glm::vec3(0.f, -1.f, 0.f))
			animator->SetCurrentRow(3);
		else if (currentDir == glm::vec3(0.f, 1.f, 0.f))
			animator->SetCurrentRow(2);
	}
}


#pragma region GhostChaseState
void dae::GhostChaseState::Update()
{
	auto* owner = m_Controller->GetOwner();
	auto player = m_Controller->GetPrimaryPlayer();
	auto* grid = m_Controller->GetGrid();
	auto* behavior = m_Controller->GetBehavior();
	if (!owner || !player || !grid || !behavior) return;

	// Chase: Use normal targeting
	glm::ivec2 targetCell = behavior->GetTargetCell(owner, player.get(), grid, m_Controller);
	GhostMoveAndAnimate(m_Controller, targetCell);
}

void dae::GhostChaseState::Render() const
{
}

void dae::GhostChaseState::OnEnter()
{
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;

	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		auto* behavior = m_Controller->GetBehavior();

		if (dynamic_cast<BlinkyBehavior*>(behavior))
			animator->Texture("Blinky.png");
		else if (dynamic_cast<PinkyBehavior*>(behavior))
			animator->Texture("Pinky.png");
		else if (dynamic_cast<InkyBehavior*>(behavior))
			animator->Texture("Inky.png");
		else if (dynamic_cast<ClydeBehavior*>(behavior))
			animator->Texture("Clyde.png");
	}
}

void dae::GhostChaseState::OnExit()
{
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;
	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		animator->SetCurrentRow(0); // Reset to default row
	}
}
#pragma endregion
#pragma region GhostScatterState
void dae::GhostScatterState::Update()
{
	auto* owner = m_Controller->GetOwner();
	auto player = m_Controller->GetPrimaryPlayer();
	auto* grid = m_Controller->GetGrid();
	auto* behavior = m_Controller->GetBehavior();
	if (!owner || !player || !grid || !behavior) return;

	// Scatter: Use normal targeting
	glm::ivec2 targetCell = behavior->GetTargetCell(owner, player.get(), grid, m_Controller);
	GhostMoveAndAnimate(m_Controller, targetCell);
}

void dae::GhostScatterState::Render() const
{
}

void dae::GhostScatterState::OnEnter()
{
	std::cout << "GhostScatterState: Entering scatter state." << std::endl;
}

void dae::GhostScatterState::OnExit()
{
	std::cout << "GhostScatterState: Exiting scatter state." << std::endl;
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;
	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		animator->SetCurrentRow(0); // Reset to default row
	}
}
#pragma endregion
#pragma region GhostFrightenedState
void dae::GhostFrightenedState::Update()
{
	auto* owner = m_Controller->GetOwner();
	auto player = m_Controller->GetPrimaryPlayer();
	auto* grid = m_Controller->GetGrid();
	auto* behavior = m_Controller->GetBehavior();
	if (!owner || !player || !grid || !behavior) return;

	// Frightened: Use normal targeting
	glm::ivec2 targetCell = behavior->GetTargetCell(owner, player.get(), grid, m_Controller);
	GhostMoveAndAnimate(m_Controller, targetCell);

	m_Controller->SetCurrentTargetCell(targetCell);

	// Always use row 0 for frightened animation
	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		animator->SetCurrentRow(0);
	}
}

void dae::GhostFrightenedState::Render() const
{
	std::cout << "GhostFrightenedState: Rendering frightened state." << std::endl;
	// Rendering logic can be added here if needed
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;

	owner->Render();
}

void dae::GhostFrightenedState::OnEnter()
{
	std::cout << "GhostFrightenedState: Entering frightened state." << std::endl;
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;
	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		animator->Texture("GhostScared.png");
	}

	owner->SetDirection(-owner->GetDirection()); // Stop movement when entering frightened state
}

void dae::GhostFrightenedState::OnExit()
{
	std::cout << "GhostFrightenedState: Exiting frightened state." << std::endl;
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;
	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		animator->SetCurrentRow(0); // Reset to default row
	}
}
#pragma endregion
#pragma region GhostEatenState
void dae::GhostEatenState::Update()
{
	auto* owner = m_Controller->GetOwner();
	auto player = m_Controller->GetPrimaryPlayer();
	auto* grid = m_Controller->GetGrid();
	auto* behavior = m_Controller->GetBehavior();
	if (!owner || !player || !grid || !behavior) return;

	// Eaten: Use normal targeting
	glm::ivec2 targetCell = behavior->GetTargetCell(owner, player.get(), grid, m_Controller);
	GhostMoveAndAnimate(m_Controller, targetCell);
}

void dae::GhostEatenState::Render() const
{
}

void dae::GhostEatenState::OnEnter()
{
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;
	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		animator->Texture("GhostEaten.png");
	}
}

void dae::GhostEatenState::OnExit()
{
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;
	// Reset direction
	owner->SetIsMoving(false);
}
#pragma endregion

void dae::GhostReswpawnState::Update()
{
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;

	// Stop all movement
	owner->SetDirection(glm::vec3(0.f, 0.f, 0.f));

	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		animator->SetCurrentRow(0);
	}
}

void dae::GhostReswpawnState::Render() const
{
}

void dae::GhostReswpawnState::OnEnter()
{
	auto* owner = m_Controller->GetOwner();
	if (!owner) return;

	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		auto* behavior = m_Controller->GetBehavior();

		if (dynamic_cast<BlinkyBehavior*>(behavior))
			animator->Texture("Blinky.png");
		else if (dynamic_cast<PinkyBehavior*>(behavior))
			animator->Texture("Pinky.png");
		else if (dynamic_cast<InkyBehavior*>(behavior))
			animator->Texture("Inky.png");
		else if (dynamic_cast<ClydeBehavior*>(behavior))
			animator->Texture("Clyde.png");
	}
}

void dae::GhostReswpawnState::OnExit()
{
}

