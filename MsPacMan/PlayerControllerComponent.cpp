#include "PlayerControllerComponent.h"
#include "InputManager.h"
#include "GameObject.h"
#include "GameObjectCommand.h"
#include "SpriteAnimatorComponent.h"
#include "Timer.h"

#include "SDLSoundSystem.h"
#include "ServiceLocator.h"
#include "PowerPelletEatenComponent.h"

dae::PlayerControllerComponent::PlayerControllerComponent(GameObject* pOwner, int controllerId, GridComponent* pGrid, int spriteSize)
	: Component(pOwner), m_ControllerId(controllerId), m_pGrid(pGrid), m_SpriteSize(spriteSize)
{
	// Register keyboard commands
	auto& inputManager = InputManager::GetInstance();

	if (pOwner)
	{
		// Instead of setting direction directly, set pending direction
		//inputManager.RegisterKeyCommand(SDLK_a, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(-1.f, 0.f, 0.f)));
		//inputManager.RegisterKeyCommand(SDLK_d, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(1.f, 0.f, 0.f)));
		//inputManager.RegisterKeyCommand(SDLK_w, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(0.f, -1.f, 0.f)));
		//inputManager.RegisterKeyCommand(SDLK_s, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(0.f, 1.f, 0.f)));
		//
		//inputManager.RegisterKeyCommand(SDLK_z, std::make_unique<dae::ScoreCommand>(pOwner, 10));
		//inputManager.RegisterKeyCommand(SDLK_x, std::make_unique<dae::ScoreCommand>(pOwner, 10));
		//inputManager.RegisterKeyCommand(SDLK_c, std::make_unique<dae::DamageCommand>(pOwner, 10));

		// Controller movement commands for the specified controller ID
		inputManager.RegisterControllerCommand(controllerId, ControllerButton::DPadLeft, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(-1.f, 0.f, 0.f)));
		inputManager.RegisterControllerCommand(controllerId, ControllerButton::DPadRight, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(1.f, 0.f, 0.f)));
		inputManager.RegisterControllerCommand(controllerId, ControllerButton::DPadUp, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(0.f, -1.f, 0.f)));
		inputManager.RegisterControllerCommand(controllerId, ControllerButton::DPadDown, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(0.f, 1.f, 0.f)));
	}
}

dae::PlayerControllerComponent::~PlayerControllerComponent()
{
}

void dae::PlayerControllerComponent::Update()
{
	auto* owner = GetOwner();
	if (!owner || !m_pGrid) return;

	// --- Grid movement logic ---
	auto deltaTime = Timer::GetInstance().GetDeltaTime();
	auto position = owner->GetLocalPosition();
	int cellSize = m_pGrid->GetCellSize();

	glm::vec2 center = { position.x + 0.5f * m_SpriteSize, position.y + 0.5f * m_SpriteSize };
	int cellX = static_cast<int>(center.x) / cellSize;
	int cellY = static_cast<int>(center.y) / cellSize;

	bool isCentered =
		(static_cast<int>(center.x) % cellSize == cellSize / 2) &&
		(static_cast<int>(center.y) % cellSize == cellSize / 2);

	// Try to change direction if requested and centered
	if (isCentered && m_PendingDirection != glm::vec3(0.f))
	{
		int nextCellX = cellX + static_cast<int>(m_PendingDirection.x);
		int nextCellY = cellY + static_cast<int>(m_PendingDirection.y);

		if (m_pGrid->IsCellWalkable(nextCellX, nextCellY))
		{
			owner->SetDirection(m_PendingDirection);
			m_PendingDirection = glm::vec3(0.f);
		}
	}

	// Move in current direction if possible
	auto dir = owner->GetDirection();
	if (dir != glm::vec3(0.f))
	{
		int nextCellX = cellX + static_cast<int>(dir.x);
		int nextCellY = cellY + static_cast<int>(dir.y);

		bool canMove = m_pGrid->IsCellWalkable(nextCellX, nextCellY);

		if (canMove)
		{
			position.x += dir.x * deltaTime * owner->GetSpeed();
			position.y += dir.y * deltaTime * owner->GetSpeed();
			owner->SetWorldPosition(position);
		}
		else
		{
			// Snap to cell center and stop
			position.x = cellX * cellSize + cellSize / 2.f - m_SpriteSize / 2.f;
			position.y = cellY * cellSize + cellSize / 2.f - m_SpriteSize / 2.f;
			owner->SetWorldPosition(position);
			owner->SetDirection(glm::vec3(0.f));
		}
	}

	CellType currentCellType = m_pGrid->GetCell(cellX, cellY);

	// Play pellet sound when eating a pellet
	if (currentCellType == CellType::Pellet)
	{
		m_pGrid->SetCell(cellX, cellY, CellType::Empty);
		if (auto scoreComp = owner->GetComponentPtr<ScoreComponent>())
		{
			ServiceLocator::GetInstance().GetSoundSystem().Play("../Data/SFX/credit.wav", 0.5f, true);
			scoreComp->AddScore(10);
		}
		m_PelletSoundGraceTimer = m_PelletSoundGraceDuration;
		m_PelletSoundActive = true;
	}
	else if (currentCellType == CellType::PowerPellet)
	{
		m_pGrid->SetCell(cellX, cellY, CellType::Empty);
		if (auto scoreComp = owner->GetComponentPtr<ScoreComponent>())
		{
			scoreComp->AddScore(50);
		}

		if (auto ghostInteractionComp = owner->GetComponentPtr<PowerPelletEatenComponent>())
		{
			ghostInteractionComp->OnNotify(*owner, Event::GHOST_FRIGHTENED);
		}
	}

	// Pellet sound grace period logic
	if (m_PelletSoundActive)
	{
		if (currentCellType != CellType::Pellet)
		{
			m_PelletSoundGraceTimer -= deltaTime;
			if (m_PelletSoundGraceTimer <= 0.0f)
			{
				ServiceLocator::GetInstance().GetSoundSystem().StopLoop("../Data/SFX/credit.wav");
				m_PelletSoundActive = false;
				m_PelletSoundGraceTimer = 0.0f;
			}
		}
		else
		{
			// If we're still on a pellet, keep the timer at grace duration
			m_PelletSoundGraceTimer = m_PelletSoundGraceDuration;
		}
	}


	// --- Sprite animation logic (unchanged) ---
	if (auto animator = owner->GetComponentPtr<SpriteAnimatorComponent>())
	{
		if (dir == glm::vec3(-1.f, 0.f, 0.f))
			animator->Texture("MsPacMan_Left16.png");
		else if (dir == glm::vec3(1.f, 0.f, 0.f))
			animator->Texture("MsPacMan_Right16.png");
		else if (dir == glm::vec3(0.f, -1.f, 0.f))
			animator->Texture("MsPacMan_Up16.png");
		else if (dir == glm::vec3(0.f, 1.f, 0.f))
			animator->Texture("MsPacMan_Down16.png");
	}
}

void dae::PlayerControllerComponent::SetSpawnCell(int cellX, int cellY)
{
	if (!GetOwner() || !m_pGrid) return;

	int cellSize = m_pGrid->GetCellSize();
	// Center the sprite in the cell
	float x = cellX * cellSize + cellSize / 2.0f - m_SpriteSize / 2.0f;
	float y = cellY * cellSize + cellSize / 2.0f - m_SpriteSize / 2.0f;
	GetOwner()->SetWorldPosition(x, y, 0.f);
}


