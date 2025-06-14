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
#include "Renderer.h"
#include "PlayerControllerComponent.h"

dae::GhostAIControllerComponent::GhostAIControllerComponent
(GameObject* pOwner, GridComponent* pGrid, GameObject* pPlayer, std::unique_ptr<IGhostBehavior> behavior, int spriteSize)
	: Component(pOwner)
	, m_pGrid(pGrid)
	, m_pPlayer(pPlayer)
	, m_Behavior(std::move(behavior))
	, m_SpriteSize(spriteSize)
{
	SetState(std::unique_ptr<GhostState>(new GhostChaseState(this)));
}

void dae::GhostAIControllerComponent::Update()
{
	if (!GetOwner() || !m_pGrid || !m_pPlayer || !m_Behavior) return;

	// Update the state timer
	m_StateTimer += Timer::GetInstance().GetDeltaTime();

	// Check for frightened state timeout
	if (dynamic_cast<GhostFrightenedState*>(m_CurrentState.get()))
	{
		// 1. Check for timer expiration
		if (m_StateTimer >= m_FrightenedDuration)
		{
			m_CurrentState->OnExit();
			SetState(std::make_unique<GhostChaseState>(this));
			return;
		}

		// 2. Check for collision with player
		glm::vec3 ghostPos = GetOwner()->GetLocalPosition();
		glm::vec3 playerPos = m_pPlayer->GetLocalPosition();

		// Use center positions for both
		int ghostSprite = m_SpriteSize;
		int playerSprite = m_pPlayer->GetComponentPtr<SpriteAnimatorComponent>()
			? 32
			: ghostSprite; // fallback

		float ghostCenterX = ghostPos.x + 0.5f * ghostSprite;
		float ghostCenterY = ghostPos.y + 0.5f * ghostSprite;
		float playerCenterX = playerPos.x + 0.5f * playerSprite;
		float playerCenterY = playerPos.y + 0.5f * playerSprite;

		float dx = ghostCenterX - playerCenterX;
		float dy = ghostCenterY - playerCenterY;
		float distSq = dx * dx + dy * dy;

		const float collisionRadius = 12.0f; // Adjust as needed (in pixels)
		if (distSq < collisionRadius * collisionRadius)
		{
			m_CurrentState->OnExit();
			SetState(std::make_unique<GhostEatenState>(this));
			return;
		}
	}

	// Update the current state if it exists
	if (m_CurrentState)
	{
		m_CurrentState->Update();
		return;
	}
}


void dae::GhostAIControllerComponent::Render() const
{
#ifdef _DEBUG
	// Rendering is handled by the SpriteAnimatorComponent, no need to implement here

	// --- Debug rendering for target cell and last direction ---
	// You need access to the SDL_Renderer* (replace 'renderer' with your actual renderer variable)
	SDL_Renderer* gRenderer = Renderer::GetInstance().GetSDLRenderer();

	if (!m_pGrid || !GetOwner()) return;

	// Draw target cell rectangle
	glm::ivec2 targetCell = m_CurrentTargetCell;
	int cellSize = m_pGrid->GetCellSize();
	if (targetCell.x >= 0 && targetCell.y >= 0) {
		SDL_Rect rect;
		rect.x = targetCell.x * cellSize;
		rect.y = targetCell.y * cellSize;
		rect.w = cellSize;
		rect.h = cellSize;
		SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 128); // Red, semi-transparent
		SDL_RenderDrawRect(gRenderer, &rect);
	}

	// Draw last direction line from ghost center
	glm::vec3 pos = GetOwner()->GetLocalPosition();
	int centerX = static_cast<int>(pos.x) + m_SpriteSize / 2;
	int centerY = static_cast<int>(pos.y) + m_SpriteSize / 2;
	glm::ivec2 lastDir = m_LastDirection;
	if (lastDir != glm::ivec2{ 0, 0 }) {
		int lineLen = m_SpriteSize; // Length of the line
		int endX = centerX + lastDir.x * lineLen;
		int endY = centerY + lastDir.y * lineLen;
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); // Green
		SDL_RenderDrawLine(gRenderer, centerX, centerY, endX, endY);
	}
#endif
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

void dae::GhostAIControllerComponent::OnNotify(const GameObject& /*gameObject*/, const Event& event)
{
	if (event == Event::GHOST_EATEN)
	{
		if (m_CurrentState)
			m_CurrentState->OnExit();
		SetState(std::make_unique<GhostEatenState>(this));
	}
	else if (event == Event::GHOST_FRIGHTENED)
	{
		// Only allow frightend if ghost is currently in chase or scatter state
		if (m_CurrentState &&
			(typeid(*m_CurrentState) == typeid(GhostChaseState) ||
				typeid(*m_CurrentState) == typeid(GhostFrightenedState) ||
				typeid(*m_CurrentState) == typeid(GhostScatterState)))
		{
			m_CurrentState->OnExit();
			SetState(std::make_unique<GhostFrightenedState>(this));
		}
	}
	else if (event == Event::GHOST_SCATTER)
	{
		if (m_CurrentState)
			m_CurrentState->OnExit();
		SetState(std::make_unique<GhostScatterState>(this));
	}
	else if (event == Event::GHOST_CHASE)
	{
		if (m_CurrentState)
			m_CurrentState->OnExit();
		SetState(std::make_unique<GhostChaseState>(this));
	}
}

