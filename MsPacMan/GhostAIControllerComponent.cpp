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
#include "ScoreComponent.h"
#include "HealthComponent.h"

std::queue<dae::GhostAIControllerComponent*> dae::GhostAIControllerComponent::s_RespawnQueue;
int dae::GhostAIControllerComponent::s_GhostsEatenInFrightened = 0;

dae::GhostAIControllerComponent::GhostAIControllerComponent
(
	GameObject* pOwner,
	GridComponent* pGrid,
	const std::vector<std::shared_ptr<GameObject>>& players,
	std::unique_ptr<IGhostBehavior> behavior,
	int spriteSize
)
	: Component(pOwner)
	, m_pGrid(pGrid)
	, m_pPlayers(players)
	, m_Behavior(std::move(behavior))
	, m_SpriteSize(spriteSize)
{
	m_CurrentState = std::make_unique<GhostChaseState>(this);
	//SetState(std::unique_ptr<GhostState>(new GhostChaseState(this)));
}

void dae::GhostAIControllerComponent::Update()
{
	if (!GetOwner() || !m_pGrid || m_pPlayers.empty() || !m_Behavior) return;

	// Update the state timer
	m_StateTimer += Timer::GetInstance().GetDeltaTime();

	if (dynamic_cast<GhostChaseState*>(m_CurrentState.get()) || dynamic_cast<GhostScatterState*>(m_CurrentState.get()))
	{
		glm::vec3 ghostPos = GetOwner()->GetLocalPosition();
		int ghostSprite = m_SpriteSize;
		const float collisionRadius = 12.0f; 
		bool collided = false;

		for (const auto& player : m_pPlayers)
		{
			if (!player) continue;
			glm::vec3 playerPos = player->GetLocalPosition();
			int playerSprite = player->GetComponentPtr<SpriteAnimatorComponent>() ? 32 : ghostSprite; 

			float ghostCenterX = ghostPos.x + 0.5f * ghostSprite;
			float ghostCenterY = ghostPos.y + 0.5f * ghostSprite;
			float playerCenterX = playerPos.x + 0.5f * playerSprite;
			float playerCenterY = playerPos.y + 0.5f * playerSprite;

			float dx = ghostCenterX - playerCenterX;
			float dy = ghostCenterY - playerCenterY;
			float distSq = dx * dx + dy * dy;

			if (distSq < collisionRadius * collisionRadius)
			{
				// Collided with this player
				if (auto* health = player->GetComponentPtr<HealthComponent>())
				{
					health->TakeDamage(1); 
				}
				collided = true;
				break;
			}
		}
		if (collided) return;
	}

	// Check for frightened state timeout
	if (dynamic_cast<GhostFrightenedState*>(m_CurrentState.get()))
	{
		// 1. Check for timer expiration
		if (m_StateTimer >= m_FrightenedDuration)
		{
			m_CurrentState->OnExit();
			SetState(std::make_unique<GhostChaseState>(this));
			s_GhostsEatenInFrightened = 0; // Reset ghost-eaten counter
			return;
		}

		// 2. Check for collision with any player
		glm::vec3 ghostPos = GetOwner()->GetLocalPosition();
		int ghostSprite = m_SpriteSize;
		const float collisionRadius = 12.0f; // Adjust as needed (in pixels)
		bool collided = false;

		for (const auto& player : m_pPlayers)
		{
			if (!player) continue;
			glm::vec3 playerPos = player->GetLocalPosition();
			int playerSprite = player->GetComponentPtr<SpriteAnimatorComponent>() ? 32 : ghostSprite; // fallback

			float ghostCenterX = ghostPos.x + 0.5f * ghostSprite;
			float ghostCenterY = ghostPos.y + 0.5f * ghostSprite;
			float playerCenterX = playerPos.x + 0.5f * playerSprite;
			float playerCenterY = playerPos.y + 0.5f * playerSprite;

			float dx = ghostCenterX - playerCenterX;
			float dy = ghostCenterY - playerCenterY;
			float distSq = dx * dx + dy * dy;

			if (distSq < collisionRadius * collisionRadius)
			{
				// Collided with this player
				m_CurrentState->OnExit();
				SetState(std::make_unique<GhostEatenState>(this));

				// Calculate score for this ghost
				static const int ghostScores[] = { 200, 400, 800, 1600 };
				int scoreIdx = s_GhostsEatenInFrightened;
				if (scoreIdx > 3) scoreIdx = 3;
				int score = ghostScores[scoreIdx];

				// Add score to this player
				if (auto* playerScore = player->GetComponentPtr<ScoreComponent>())
				{
					playerScore->AddScore(score);
				}

				// Increment for next ghost
				++s_GhostsEatenInFrightened;
				collided = true;
				break;
			}
		}
		if (collided) return;
	}

	// Sequential respawn logic for EatenState
	if (typeid(*m_CurrentState) == typeid(GhostEatenState))
	{
		// Get ghost's current cell
		auto* grid = m_pGrid;
		auto* owner = GetOwner();
		int cellSize = grid->GetCellSize();
		glm::vec3 pos = owner->GetLocalPosition();
		int cellX = static_cast<int>(pos.x + 0.5f * m_SpriteSize) / cellSize;
		int cellY = static_cast<int>(pos.y + 0.5f * m_SpriteSize) / cellSize;

		// Home base cell (adjust as needed for your map)
		int homeX = grid->GetGridWidth() / 2;
		int homeY = grid->GetGridHeight() / 2;

		// If ghost reaches home base and not already queued
		if (cellX == homeX && cellY == homeY && !m_WaitingInHouse)
		{
			m_CurrentState->OnExit();
			SetState(std::make_unique<GhostChaseState>(this));
			m_WaitingInHouse = true;
			s_RespawnQueue.push(this);
			m_StateTimer = 0.0f;
		}
	}
	// If in respawn state, handle respawn logic
	if (m_WaitingInHouse && !s_RespawnQueue.empty())
	{
		if (s_RespawnQueue.front() == this)
		{
			m_StateTimer += Timer::GetInstance().GetDeltaTime();
			if (m_StateTimer >= m_RespawnWaitTime)
			{
				s_RespawnQueue.pop();
				SetState(std::make_unique<GhostChaseState>(this));
				m_WaitingInHouse = false;
				m_StateTimer = 0.0f;
			}
			return; // Still waiting in house
		}
		else
		{
			return; // Not this ghost's turn to respawn yet
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
		// Only allow frightened if ghost is currently in chase or scatter state
		if (m_CurrentState &&
			(typeid(*m_CurrentState) == typeid(GhostChaseState) ||
				typeid(*m_CurrentState) == typeid(GhostFrightenedState) ||
				typeid(*m_CurrentState) == typeid(GhostScatterState)))
		{
			m_CurrentState->OnExit();
			SetState(std::make_unique<GhostFrightenedState>(this));
			s_GhostsEatenInFrightened = 0; // Reset on new frightened period
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
