#include "GhostAIControllerComponent.h"
#include "GhostBehavior.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "PathFinding.h"
#include <glm.hpp>
#include <memory>
#include <algorithm>
#include "Timer.h"
#include "SpriteAnimatorComponent.h"

dae::GhostAIControllerComponent::GhostAIControllerComponent
(GameObject* pOwner, GridComponent* pGrid, GameObject* pPlayer, std::unique_ptr<IGhostBehavior> behavior, int spriteSize)
	: Component(pOwner)
	, m_pGrid(pGrid)
	, m_pPlayer(pPlayer)
	, m_Behavior(std::move(behavior))
	, m_SpriteSize(spriteSize)
{
}

void dae::GhostAIControllerComponent::Update()
{
	auto* owner = GetOwner();
	if (!owner || !m_pGrid || !m_pPlayer || !m_Behavior) return;

	int cellSize = m_pGrid->GetCellSize();
	glm::vec3 position = owner->GetLocalPosition();
	glm::vec2 center = { position.x + 0.5f * m_SpriteSize, position.y + 0.5f * m_SpriteSize };
	int cellX = static_cast<int>(center.x) / cellSize;
	int cellY = static_cast<int>(center.y) / cellSize;

	// Calculate the exact center of the current cell
	float cellCenterX = cellX * cellSize + cellSize / 2.f;
	float cellCenterY = cellY * cellSize + cellSize / 2.f;

	// Use epsilon for floating-point comparison
	const float epsilon = 1.f;
	bool isCentered =
		(std::abs(center.x - cellCenterX) < epsilon) &&
		(std::abs(center.y - cellCenterY) < epsilon);

	// Get target cell from behavior
	glm::ivec2 ghostCell(cellX, cellY);
	glm::ivec2 targetCell = m_Behavior->GetTargetCell(owner, m_pPlayer, m_pGrid);

	// Only update direction/path if centered
	if (isCentered)
	{
		// Snap to cell center to avoid drift
		owner->SetWorldPosition(
			cellCenterX - m_SpriteSize / 2.f,
			cellCenterY - m_SpriteSize / 2.f,
			0.f
		);

		int gridW = m_pGrid->GetGridWidth();
		int gridH = m_pGrid->GetGridHeight();

		if (ghostCell.x < 0 || ghostCell.x >= gridW ||
			ghostCell.y < 0 || ghostCell.y >= gridH ||
			targetCell.x < 0 || targetCell.x >= gridW ||
			targetCell.y < 0 || targetCell.y >= gridH)
		{
			owner->SetDirection(glm::vec3(0.f));
			return;
		}


		auto path = dae::FindPathBFS(m_pGrid, ghostCell, targetCell);

		if (!path.empty()) {
			glm::ivec2 nextCell = path.front();
			glm::ivec2 dir = nextCell - ghostCell;
			owner->SetDirection(glm::vec3(dir.x, dir.y, 0.f));
		}
		else {
			owner->SetDirection(glm::vec3(0.f));
		}
	}

	// --- Movement logic (center-to-center) ---
	auto dir = owner->GetDirection();
	if (dir != glm::vec3(0.f)) {
		int nextCellX = cellX + static_cast<int>(dir.x);
		int nextCellY = cellY + static_cast<int>(dir.y);

		bool canMove = !isCentered || m_pGrid->IsCellWalkable(nextCellX, nextCellY);

		float deltaTime = Timer::GetInstance().GetDeltaTime();
		if (canMove)
		{
			position.x += dir.x * deltaTime * owner->GetSpeed();
			position.y += dir.y * deltaTime * owner->GetSpeed();
			owner->SetWorldPosition(position);
		}
		else
		{
			// Snap to cell center and stop
			owner->SetWorldPosition(
				cellCenterX - m_SpriteSize / 2.f,
				cellCenterY - m_SpriteSize / 2.f,
				0.f
			);
			owner->SetDirection(glm::vec3(0.f));
		}
	}

	// --- Sprite animation logic (directional ghost textures) ---
	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		if (dir == glm::vec3(-1.f, 0.f, 0.f))
			animator->SetCurrentRow(1); // Assuming row 1 is for left direction
		else if (dir == glm::vec3(1.f, 0.f, 0.f))
			animator->SetCurrentRow(0); // Assuming row 0 is for right direction
		else if (dir == glm::vec3(0.f, -1.f, 0.f))
			animator->SetCurrentRow(3); // Assuming row 3 is for up direction
		else if (dir == glm::vec3(0.f, 1.f, 0.f))
			animator->SetCurrentRow(2); // Assuming row 2 is for down direction
	}
}

void dae::GhostAIControllerComponent::SetSpawnCell(int cellX, int cellY)
{
	if (!GetOwner() || !m_pGrid) return;

	int cellSize = m_pGrid->GetCellSize();
	// Center the sprite in the cell
	float x = cellX * cellSize + cellSize / 2.0f - m_SpriteSize / 2.0f;
	float y = cellY * cellSize + cellSize / 2.0f - m_SpriteSize / 2.0f;
	GetOwner()->SetWorldPosition(x, y, 0.f);
}

