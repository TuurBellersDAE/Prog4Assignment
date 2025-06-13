#include "GameState.h"

#include "Scene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"

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

#include "GhostBehavior.h"

#include "InputManager.h"
#include "GameObjectCommand.h"

#include "GameObserver.h"

#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

#pragma region PlayingState
void dae::PlayingState::Update()
{
}

void dae::PlayingState::Render() const
{
}

void dae::PlayingState::OnEnter()
{
	m_Scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);

	if (m_Scene)
	{
		dae::SceneManager::GetInstance().SetActiveScene(m_SceneName);


		LoadPlayingAssets();

	}
}

void dae::PlayingState::OnExit()
{
	if (m_Scene)
	{
		dae::SceneManager::GetInstance().SetActiveScene(""); // Clear active scene
	}
}

void dae::PlayingState::LoadPlayingAssets()
{
	dae::ServiceLocator::GetInstance().RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	auto& soundSystem = dae::ServiceLocator::GetInstance().GetSoundSystem();
	soundSystem.LoadSound("../Data/ms_eat_dot.wav");
	soundSystem.LoadSound("../Data/ms_death.wav");
	soundSystem.LoadSound("../Data/SFX/credit.wav");

	auto backGroundImage = std::make_shared<dae::GameObject>();
	backGroundImage->AddComponent<dae::RenderComponent>("background.tga", glm::vec3(0, 0, 0));
	m_Scene->Add(backGroundImage);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fps = std::make_shared<dae::GameObject>();
	auto& fpsComp = fps->AddComponent<dae::FPSComponent>();
	auto& fpsTextComp = fps->AddComponent<dae::TextComponent>("", font, glm::vec3(0, 0, 0));
	fpsTextComp.SetText("FPS: " + fpsComp.GetStringFPS());
	m_Scene->Add(fps);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//Keybinds text objects
	auto keyboarKeybindsText = std::make_shared<dae::GameObject>();
	keyboarKeybindsText->AddComponent<dae::TextComponent>("WASD: Move msPacMan, C inflict self harm, Z and X to score points", font, glm::vec3(0, 60, 0));
	m_Scene->Add(keyboarKeybindsText);

	auto controllerKeybindsText = std::make_shared<dae::GameObject>();
	controllerKeybindsText->AddComponent<dae::TextComponent>("D-Pad: Move mrPacMan, X inflict self harm, A and B to score points", font, glm::vec3(0, 80, 0));
	m_Scene->Add(controllerKeybindsText);

	auto soundKeybindsText = std::make_shared<dae::GameObject>();
	soundKeybindsText->AddComponent<dae::TextComponent>("Score and Damage plays a sound", font, glm::vec3(0, 100, 0));
	m_Scene->Add(soundKeybindsText);


	// Create a text object for msPacMan and mrPacMan
	auto msPacManScoreText = std::make_shared<dae::GameObject>();
	msPacManScoreText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 120, 0));
	auto msPacManHealthText = std::make_shared<dae::GameObject>();
	msPacManHealthText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 140, 0));

	auto mrPacManScoreText = std::make_shared<dae::GameObject>();
	mrPacManScoreText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 160, 0));
	auto mrPacManHealthText = std::make_shared<dae::GameObject>();
	mrPacManHealthText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 180, 0));

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
	msPacManScoreComp.BindTextComponent(msPacManScoreText->GetComponentPtr<dae::TextComponent>());
	msPacManHealthComp.BindTextComponent(msPacManHealthText->GetComponentPtr<dae::TextComponent>());
	auto& msPacManControllerComp = msPacMan->AddComponent<dae::PlayerControllerComponent>(0, &gridComp, 32); // 16 is the sprite size
	msPacManControllerComp.SetSpawnCell(14, 23); // Set spawn cell for msPacMan


	auto ghostBlinky = std::make_shared<dae::GameObject>();
	ghostBlinky->AddComponent<dae::SpriteAnimatorComponent>("Blinky.png", 32, 32, 2, 0.2f); // Use Blinky's sprite
	ghostBlinky->SetSpeed(50.f);
	auto& ghostAIControllerComp = ghostBlinky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		msPacMan.get(),
		std::make_unique<dae::BlinkyBehavior>(),
		32
	);
	ghostAIControllerComp.SetSpawnCell(14, 23);

	auto ghostPinky = std::make_shared<dae::GameObject>();
	ghostPinky->AddComponent<dae::SpriteAnimatorComponent>("Pinky.png", 32, 32, 2, 0.2f); // Use Pinky's sprite
	ghostPinky->SetSpeed(50.f);
	auto& ghostAIControllerCompPinky = ghostPinky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		msPacMan.get(),
		std::make_unique<dae::PinkyBehavior>(),
		32
	);
	ghostAIControllerCompPinky.SetSpawnCell(14, 23);

	auto ghostInky = std::make_shared<dae::GameObject>();
	ghostInky->AddComponent<dae::SpriteAnimatorComponent>("Inky.png", 32, 32, 2, 0.2f); // Use Inky's sprite
	ghostInky->SetSpeed(50.f);
	auto& ghostAIControllerCompInky = ghostInky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		msPacMan.get(),
		std::make_unique<dae::InkyBehavior>(),
		32
	);
	ghostAIControllerCompInky.SetSpawnCell(14, 23);

	auto ghostClyde = std::make_shared<dae::GameObject>();
	ghostClyde->AddComponent<dae::SpriteAnimatorComponent>("Clyde.png", 32, 32, 2, 0.2f); // Use Clyde's sprite
	ghostClyde->SetSpeed(50.f);
	auto& ghostAIControllerCompClyde = ghostClyde->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		msPacMan.get(),
		std::make_unique<dae::ClydeBehavior>(),
		32
	);
	ghostAIControllerCompClyde.SetSpawnCell(14, 23);

	// Add msPacMan and mrPacMan to the scene
	m_Scene->Add(msPacMan);
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
}

void dae::MenuState::Render() const
{
}

void dae::MenuState::OnEnter()
{
}

void dae::MenuState::OnExit()
{
}

void dae::MenuState::LoadMenuAssets()
{
}
#pragma endregion

