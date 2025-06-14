#include "GameState.h"

#include "Scene.h"
#include "SceneManager.h"
#include "ResourceManager.h"

#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotationComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "PlayerControllerComponent.h"
#include "SpriteAnimatorComponent.h"
#include "GridComponent.h"
#include "GhostAIControllerComponent.h"
#include "PowerPelletEatenComponent.h"

#include "GhostBehavior.h"

#include "InputManager.h"
#include "GameObjectCommand.h"

#include "GameObserver.h"

#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

#include <SDL.h> // For SDLK_w, SDLK_s

#include "GameStateManager.h"

#pragma region SinglePlaerState
void dae::SinglePlayer::Update()
{
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	if (state[SDL_SCANCODE_ESCAPE])
	{
		dae::SceneManager::GetInstance().GetActiveScene()->RemoveAll();
		
		dae::GameStateManager::GetInstance().SetState("Menu");
		return;
	}
}

void dae::SinglePlayer::Render() const
{
}

void dae::SinglePlayer::OnEnter()
{
	OnEnter("");
}

void dae::SinglePlayer::OnEnter(const std::string& previousState)
{
	m_Scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);

	if (m_Scene)
	{
		dae::SceneManager::GetInstance().SetActiveScene(m_SceneName);

		// Only load assets if coming from Menu
		if (previousState == "Menu")
		{
			LoadSinglePlayerAssets();

			auto& soundSystem = dae::ServiceLocator::GetInstance().GetSoundSystem();
			soundSystem.Play("../Data/SFX/ms_start.wav", 0.5f, false);
		}
	}
}

void dae::SinglePlayer::OnExit()
{
	auto& soundSystem = dae::ServiceLocator::GetInstance().GetSoundSystem();
	soundSystem.StopAllSounds();
}

void dae::SinglePlayer::LoadSinglePlayerAssets()
{

	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto fps = std::make_shared<dae::GameObject>();
	//auto& fpsComp = fps->AddComponent<dae::FPSComponent>();
	//auto& fpsTextComp = fps->AddComponent<dae::TextComponent>("", font, glm::vec3(0, 0, 0));
	//fpsTextComp.SetText("FPS: " + fpsComp.GetStringFPS());
	//m_Scene->Add(fps);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Create a text object for msPacMan and mrPacMan
	auto msPacManScoreText = std::make_shared<dae::GameObject>();
	msPacManScoreText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 520, 0));
	auto msPacManHealthText = std::make_shared<dae::GameObject>();
	msPacManHealthText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 550, 0));

	// Create a map object with a grid component
	auto pMap = std::make_shared<dae::GameObject>();
	auto& gridComp = pMap->AddComponent<dae::GridComponent>();
	gridComp.LoadGrid("../Data/MsPacMan_Grid_01.txt");
	gridComp.SetCellSize(16);
	pMap->AddComponent<dae::RenderComponent>("MsPacMan_Map_01.png", glm::vec3(0, 0, 0));

	// Create msPacMan and mrPacMan
	auto msPacMan = std::make_shared<dae::GameObject>();
	msPacMan->AddComponent<dae::SpriteAnimatorComponent>("MsPacMan_Down16.png", 32, 32, 4, 0.2f); // 0.2 seconds per frame
	msPacMan->SetSpeed(75.f);
	auto& msPacManHealthComp = msPacMan->AddComponent<dae::HealthComponent>();
	auto& msPacManScoreComp = msPacMan->AddComponent<dae::ScoreComponent>();
	auto& msPacManPowerPelletComp = msPacMan->AddComponent<dae::PowerPelletEatenComponent>();
	msPacManScoreComp.BindTextComponent(msPacManScoreText->GetComponentPtr<dae::TextComponent>());
	msPacManHealthComp.BindTextComponent(msPacManHealthText->GetComponentPtr<dae::TextComponent>());
	auto& msPacManControllerComp = msPacMan->AddComponent<dae::PlayerControllerComponent>(0, &gridComp, 32); // 16 is the sprite size
	msPacManControllerComp.SetSpawnCell(14, 23); // Set spawn cell for msPacMan

	std::vector<std::shared_ptr<dae::GameObject>> players{ msPacMan };

	auto ghostBlinky = std::make_shared<dae::GameObject>();
	ghostBlinky->AddComponent<dae::SpriteAnimatorComponent>("GhostScared.png", 32, 32, 2, 0.2f); // Use Blinky's sprite
	ghostBlinky->SetSpeed(50.f);
	auto& ghostAIControllerComp = ghostBlinky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		players,
		std::make_unique<dae::BlinkyBehavior>(),
		32
	);
	ghostAIControllerComp.SetSpawnCell(14, 11);

	auto ghostPinky = std::make_shared<dae::GameObject>();
	ghostPinky->AddComponent<dae::SpriteAnimatorComponent>("Pinky.png", 32, 32, 2, 0.2f); // Use Pinky's sprite
	ghostPinky->SetSpeed(50.f);
	auto& ghostAIControllerCompPinky = ghostPinky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		players,
		std::make_unique<dae::PinkyBehavior>(),
		32
	);
	ghostAIControllerCompPinky.SetSpawnCell(12, 14);

	auto ghostInky = std::make_shared<dae::GameObject>();
	ghostInky->AddComponent<dae::SpriteAnimatorComponent>("Inky.png", 32, 32, 2, 0.2f); // Use Inky's sprite
	ghostInky->SetSpeed(50.f);
	auto& ghostAIControllerCompInky = ghostInky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		players,
		std::make_unique<dae::InkyBehavior>(),
		32
	);
	ghostAIControllerCompInky.SetSpawnCell(14, 14);

	auto ghostClyde = std::make_shared<dae::GameObject>();
	ghostClyde->AddComponent<dae::SpriteAnimatorComponent>("Clyde.png", 32, 32, 2, 0.2f); // Use Clyde's sprite
	ghostClyde->SetSpeed(50.f);
	auto& ghostAIControllerCompClyde = ghostClyde->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		players,
		std::make_unique<dae::ClydeBehavior>(),
		32
	);
	ghostAIControllerCompClyde.SetSpawnCell(15, 14);

	// Add msPacMan and mrPacMan to the scene
	m_Scene->Add(msPacMan);
	m_Scene->Add(ghostBlinky);
	m_Scene->Add(ghostPinky);
	m_Scene->Add(ghostInky);
	m_Scene->Add(ghostClyde);
	m_Scene->Add(msPacManScoreText);
	m_Scene->Add(msPacManHealthText);
	m_Scene->Add(pMap);

	//// Create observer for health and score for msPacMan
	auto msPacManHealthObserver = msPacMan->AddObserver(std::make_unique<dae::HealthObserver>());
	msPacManHealthComp.m_Subject.AddObserver(msPacManHealthObserver);
	msPacManHealthObserver->OnNotify(*msPacMan, dae::Event::PLAYER_HIT);

	auto msPacManScoreObserver = msPacMan->AddObserver(std::make_unique<dae::ScoreObserver>());
	msPacManScoreComp.m_Subject.AddObserver(msPacManScoreObserver);
	msPacManScoreComp.m_Subject.Notify(*msPacMan, dae::Event::PLAYER_SCORED);

	msPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerCompClyde);
	msPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerCompInky);
	msPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerCompPinky);
	msPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerComp);

	// After creating msPacMan and ghosts in LoadSinglePlayerAssets:
	m_Player = msPacMan;
	m_Ghosts = { ghostBlinky, ghostPinky, ghostInky, ghostClyde };
	m_PlayerSpawnCell = { 14, 23 };
	m_GhostSpawnCells = { {14, 11}, {12, 14}, {14, 14}, {15, 14} };
}

void dae::SinglePlayer::Reset()
{
	// Reset player position
	if (m_Player)
	{
		if (auto controller = m_Player->GetComponentPtr<dae::PlayerControllerComponent>())
			controller->SetSpawnCell(m_PlayerSpawnCell.first, m_PlayerSpawnCell.second);
	}

	// Reset ghosts' positions and states
	for (size_t i = 0; i < m_Ghosts.size(); ++i)
	{
		auto& ghost = m_Ghosts[i];
		if (ghost)
		{
			if (auto ghostAI = ghost->GetComponentPtr<dae::GhostAIControllerComponent>())
			{
				ghostAI->SetSpawnCell(m_GhostSpawnCells[i].first, m_GhostSpawnCells[i].second);
				// Reset state to Chase
				ghostAI->SetState(std::make_unique<GhostChaseState>(ghostAI));
			}
		}
	}
}

#pragma endregion
#pragma region MultiPlayerState
void dae::MultiPlayer::Update()
{
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	if (state[SDL_SCANCODE_ESCAPE])
	{
		dae::SceneManager::GetInstance().GetActiveScene()->RemoveAll();

		dae::GameStateManager::GetInstance().SetState("Menu");
		return;
	}
}

void dae::MultiPlayer::Render() const
{
}

void dae::MultiPlayer::OnEnter()
{
	OnEnter("");
}

void dae::MultiPlayer::OnEnter(const std::string& previousState)
{
	m_Scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);

	if (m_Scene)
	{
		dae::SceneManager::GetInstance().SetActiveScene(m_SceneName);

		// Only load assets if coming from Menu
		if (previousState == "Menu")
		{
			LoadMultiPlayerAssets();

			auto& soundSystem = dae::ServiceLocator::GetInstance().GetSoundSystem();
			soundSystem.Play("../Data/SFX/ms_start.wav", 0.5f, false);
		}
	}
}

void dae::MultiPlayer::OnExit()
{
	auto& soundSystem = dae::ServiceLocator::GetInstance().GetSoundSystem();
	soundSystem.StopAllSounds();
}

void dae::MultiPlayer::LoadMultiPlayerAssets()
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Create a text object for msPacMan and mrPacMan
	auto msPacManScoreText = std::make_shared<dae::GameObject>();
	msPacManScoreText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 520, 0));
	auto msPacManHealthText = std::make_shared<dae::GameObject>();
	msPacManHealthText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 550, 0));

	// Create a text object for mrPacMan
	auto mrPacManScoreText = std::make_shared<dae::GameObject>();
	mrPacManScoreText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(300, 520, 0));
	auto mrPacManHealthText = std::make_shared<dae::GameObject>();
	mrPacManHealthText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(300, 550, 0));


	// Create a map object with a grid component
	auto pMap = std::make_shared<dae::GameObject>();
	auto& gridComp = pMap->AddComponent<dae::GridComponent>();
	gridComp.LoadGrid("../Data/MsPacMan_Grid_01.txt");
	gridComp.SetCellSize(16);
	pMap->AddComponent<dae::RenderComponent>("MsPacMan_Map_01.png", glm::vec3(0, 0, 0));

	// Create msPacMan and mrPacMan
	auto msPacMan = std::make_shared<dae::GameObject>();
	msPacMan->AddComponent<dae::SpriteAnimatorComponent>("MsPacMan_Down16.png", 32, 32, 4, 0.2f); // 0.2 seconds per frame
	msPacMan->SetSpeed(75.f);
	auto& msPacManHealthComp = msPacMan->AddComponent<dae::HealthComponent>();
	auto& msPacManScoreComp = msPacMan->AddComponent<dae::ScoreComponent>();
	auto& msPacManPowerPelletComp = msPacMan->AddComponent<dae::PowerPelletEatenComponent>();
	msPacManScoreComp.BindTextComponent(msPacManScoreText->GetComponentPtr<dae::TextComponent>());
	msPacManHealthComp.BindTextComponent(msPacManHealthText->GetComponentPtr<dae::TextComponent>());
	auto& msPacManControllerComp = msPacMan->AddComponent<dae::PlayerControllerComponent>(0, &gridComp, 32); // 16 is the sprite size
	msPacManControllerComp.SetSpawnCell(12, 23); // Set spawn cell for msPacMan

	auto mrPacMan = std::make_shared<dae::GameObject>();
	mrPacMan->AddComponent<dae::SpriteAnimatorComponent>("MsPacMan_Down16.png", 32, 32, 4, 0.2f); // 0.2 seconds per frame
	mrPacMan->SetSpeed(75.f);
	auto& mrPacManHealthComp = mrPacMan->AddComponent<dae::HealthComponent>();
	auto& mrPacManScoreComp = mrPacMan->AddComponent<dae::ScoreComponent>();
	auto& mrPacManPowerPelletComp = mrPacMan->AddComponent<dae::PowerPelletEatenComponent>();
	mrPacManScoreComp.BindTextComponent(mrPacManScoreText->GetComponentPtr<dae::TextComponent>());
	mrPacManHealthComp.BindTextComponent(mrPacManHealthText->GetComponentPtr<dae::TextComponent>());
	auto& mrPacManControllerComp = mrPacMan->AddComponent<dae::PlayerControllerComponent>(1, &gridComp, 32); // 16 is the sprite size
	mrPacManControllerComp.SetSpawnCell(16, 23); // Set spawn cell for mrPacMan

	std::vector<std::shared_ptr<dae::GameObject>> players{ msPacMan, mrPacMan };

	auto ghostBlinky = std::make_shared<dae::GameObject>();
	ghostBlinky->AddComponent<dae::SpriteAnimatorComponent>("GhostScared.png", 32, 32, 2, 0.2f); // Use Blinky's sprite
	ghostBlinky->SetSpeed(50.f);
	auto& ghostAIControllerComp = ghostBlinky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		players,
		std::make_unique<dae::BlinkyBehavior>(),
		32
	);
	ghostAIControllerComp.SetSpawnCell(14, 11);

	auto ghostPinky = std::make_shared<dae::GameObject>();
	ghostPinky->AddComponent<dae::SpriteAnimatorComponent>("Pinky.png", 32, 32, 2, 0.2f); // Use Pinky's sprite
	ghostPinky->SetSpeed(50.f);
	auto& ghostAIControllerCompPinky = ghostPinky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		players,
		std::make_unique<dae::PinkyBehavior>(),
		32
	);
	ghostAIControllerCompPinky.SetSpawnCell(12, 14);

	auto ghostInky = std::make_shared<dae::GameObject>();
	ghostInky->AddComponent<dae::SpriteAnimatorComponent>("Inky.png", 32, 32, 2, 0.2f); // Use Inky's sprite
	ghostInky->SetSpeed(50.f);
	auto& ghostAIControllerCompInky = ghostInky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		players,
		std::make_unique<dae::InkyBehavior>(),
		32
	);
	ghostAIControllerCompInky.SetSpawnCell(14, 14);

	auto ghostClyde = std::make_shared<dae::GameObject>();
	ghostClyde->AddComponent<dae::SpriteAnimatorComponent>("Clyde.png", 32, 32, 2, 0.2f); // Use Clyde's sprite
	ghostClyde->SetSpeed(50.f);
	auto& ghostAIControllerCompClyde = ghostClyde->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		players,
		std::make_unique<dae::ClydeBehavior>(),
		32
	);
	ghostAIControllerCompClyde.SetSpawnCell(15, 14);

	// Add msPacMan and mrPacMan to the scene
	m_Scene->Add(msPacMan);
	m_Scene->Add(mrPacMan);
	m_Scene->Add(ghostBlinky);
	m_Scene->Add(ghostPinky);
	m_Scene->Add(ghostInky);
	m_Scene->Add(ghostClyde);
	m_Scene->Add(msPacManScoreText);
	m_Scene->Add(msPacManHealthText);
	m_Scene->Add(mrPacManScoreText);
	m_Scene->Add(mrPacManHealthText);
	m_Scene->Add(pMap);

	//// Create observer for health and score for msPacMan
	auto msPacManHealthObserver = msPacMan->AddObserver(std::make_unique<dae::HealthObserver>());
	msPacManHealthComp.m_Subject.AddObserver(msPacManHealthObserver);
	msPacManHealthObserver->OnNotify(*msPacMan, dae::Event::PLAYER_HIT);

	auto msPacManScoreObserver = msPacMan->AddObserver(std::make_unique<dae::ScoreObserver>());
	msPacManScoreComp.m_Subject.AddObserver(msPacManScoreObserver);
	msPacManScoreComp.m_Subject.Notify(*msPacMan, dae::Event::PLAYER_SCORED);

	auto mrPacManHealthObserver = mrPacMan->AddObserver(std::make_unique<dae::HealthObserver>());
	mrPacManHealthComp.m_Subject.AddObserver(mrPacManHealthObserver);
	mrPacManHealthObserver->OnNotify(*mrPacMan, dae::Event::PLAYER_HIT);

	auto mrPacManScoreObserver = mrPacMan->AddObserver(std::make_unique<dae::ScoreObserver>());
	mrPacManScoreComp.m_Subject.AddObserver(mrPacManScoreObserver);
	mrPacManScoreComp.m_Subject.Notify(*mrPacMan, dae::Event::PLAYER_SCORED);

	msPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerCompClyde);
	msPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerCompInky);
	msPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerCompPinky);
	msPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerComp);

	// Add observers for ghost AI controllers
	mrPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerCompClyde);
	mrPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerCompInky);
	mrPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerCompPinky);
	mrPacManPowerPelletComp.m_Subject.AddObserver(&ghostAIControllerComp);

	m_Players = { msPacMan, mrPacMan };
	m_Ghosts = { ghostBlinky, ghostPinky, ghostInky, ghostClyde };
	m_PlayerSpawnCells = { {12, 23}, {16, 23} }; // msPacMan, mrPacMan
	m_GhostSpawnCells = { {14, 11}, {12, 14}, {14, 14}, {15, 14} };

}

void dae::MultiPlayer::Reset()
{
	// Reset player positions
	for (size_t i = 0; i < m_Players.size(); ++i)
	{
		auto& player = m_Players[i];
		if (player)
		{
			if (auto controller = player->GetComponentPtr<dae::PlayerControllerComponent>())
				controller->SetSpawnCell(m_PlayerSpawnCells[i].first, m_PlayerSpawnCells[i].second);
			player->SetDirection({ 0,0,0 });
		}
	}

	// Reset ghosts' positions and states
	for (size_t i = 0; i < m_Ghosts.size(); ++i)
	{
		auto& ghost = m_Ghosts[i];
		if (ghost)
		{
			if (auto ghostAI = ghost->GetComponentPtr<dae::GhostAIControllerComponent>())
			{
				ghostAI->SetSpawnCell(m_GhostSpawnCells[i].first, m_GhostSpawnCells[i].second);
				ghostAI->SetState(std::make_unique<GhostChaseState>(ghostAI));
			}
		}
	}
}
#pragma endregion
#pragma region GameOverState
void dae::GameOverState::Update()
{
}

void dae::GameOverState::Render() const
{
}

void dae::GameOverState::OnEnter()
{
}

void dae::GameOverState::OnExit()
{
}

void dae::GameOverState::LoadGameOverAssets()
{
}
#pragma endregion
#pragma region PausedState
void dae::PausedState::Update()
{
}

void dae::PausedState::Render() const
{
}

void dae::PausedState::OnEnter()
{
}

void dae::PausedState::OnExit()
{
}

void dae::PausedState::LoadPauseAssets()
{
}
#pragma endregion
#pragma region MenuState
void dae::MenuState::Update()
{
	static bool wWasPressed = false;
	static bool sWasPressed = false;
	static bool enterWasPressed = false;

	const Uint8* state = SDL_GetKeyboardState(nullptr);

	// Move up
	if (state[SDL_SCANCODE_W]) {
		if (!wWasPressed) {
			m_SelectedIndex = (m_SelectedIndex - 1 + static_cast<int>(m_MenuItems.size())) % static_cast<int>(m_MenuItems.size());
			UpdateMenuVisuals();
		}
		wWasPressed = true;
	}
	else {
		wWasPressed = false;
	}

	// Move down
	if (state[SDL_SCANCODE_S]) {
		if (!sWasPressed) {
			m_SelectedIndex = (m_SelectedIndex + 1) % static_cast<int>(m_MenuItems.size());
			UpdateMenuVisuals();
		}
		sWasPressed = true;
	}
	else {
		sWasPressed = false;
	}

	// Select with Enter
	if (state[SDL_SCANCODE_RETURN] || state[SDL_SCANCODE_KP_ENTER]) {
		if (!enterWasPressed) {
			switch (m_SelectedIndex) {
			case 0: // SinglePlayer
				dae::GameStateManager::GetInstance().SetState("SinglePlayer");
				break;
			case 1: // MultiPlayer
				dae::GameStateManager::GetInstance().SetState("MultiPlayer");
				break;
			case 2: // Exit
				SDL_Event quitEvent;
				quitEvent.type = SDL_QUIT;
				SDL_PushEvent(&quitEvent);
				break;
			}
		}
		enterWasPressed = true;
	}
	else {
		enterWasPressed = false;
	}
}

void dae::MenuState::Render() const
{
}

void dae::MenuState::OnEnter()
{
	m_Scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);
	if (m_Scene)
	{
		dae::SceneManager::GetInstance().SetActiveScene(m_SceneName);
		LoadMenuAssets();

		auto& soundSystem = dae::ServiceLocator::GetInstance().GetSoundSystem();
		soundSystem.Play("../Data/SFX/ms_start.wav", 0.5f, false);
	}
}

void dae::MenuState::OnExit()
{
	auto& soundSystem = dae::ServiceLocator::GetInstance().GetSoundSystem();
	soundSystem.StopAllSounds();
}

void dae::MenuState::LoadMenuAssets()
{

	auto font = dae::ResourceManager::GetInstance().LoadFont("Pixel.ttf", 50);

	const int windowWidth = 448;
	const int windowHeight = 640;
	const int verticalPadding = 20;

	auto createCenteredText = [&](const std::string& text, int index) -> std::shared_ptr<dae::GameObject>
		{
			auto go = std::make_shared<dae::GameObject>();
			auto& textComp = go->AddComponent<dae::TextComponent>(text, font, glm::vec3(0, 0, 0));
			textComp.Update();

			int textWidth = textComp.GetTextWidth();
			int textHeight = textComp.GetTextHeight();

			int centeredX = (windowWidth / 2) - (textWidth / 2);
			int totalMenuHeight = (textHeight * 3) + (verticalPadding * 2);
			int startY = (windowHeight / 2) - (totalMenuHeight / 2);
			int y = startY + index * (textHeight + verticalPadding);

			textComp.SetPosition(glm::vec3(centeredX, y, 0));
			return go;
		};

	m_MenuItems.clear();
	m_MenuItems.push_back(createCenteredText("SinglePlayer", 0));
	m_MenuItems.push_back(createCenteredText("MultiPlayer", 1));
	m_MenuItems.push_back(createCenteredText("Exit", 2));

	for (auto& item : m_MenuItems)
		m_Scene->Add(item);

	UpdateMenuVisuals();
}


void dae::MenuState::UpdateMenuVisuals()
{
	SDL_Color selectedColor{ 255, 255, 0, 255 }; 
	SDL_Color normalColor{ 255, 255, 255, 255 }; 

	for (size_t i = 0; i < m_MenuItems.size(); ++i)
	{
		auto textComp = m_MenuItems[i]->GetComponentPtr<dae::TextComponent>();
		if (textComp)
		{
			if (static_cast<int>(i) == m_SelectedIndex)
			{
				textComp->SetColor(selectedColor);
			}
			else
			{
				textComp->SetColor(normalColor);
			}
			textComp->Update();
		}
	}
}






#pragma endregion
