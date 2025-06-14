#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
/*
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"

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

//#include "Observer.h"
#include "GameObserver.h"

#include "SDLSoundSystem.h"
#include "ServiceLocator.h"*/

//#include <steam_api.h>

#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

#include "GameStateManager.h"
#include "GameState.h"

void load()
{
	dae::ServiceLocator::GetInstance().RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	auto& soundSystem = dae::ServiceLocator::GetInstance().GetSoundSystem();
	soundSystem.LoadSound("../Data/ms_eat_dot.wav");
	soundSystem.LoadSound("../Data/ms_death.wav");
	soundSystem.LoadSound("../Data/SFX/credit.wav");

	// Initialize the game state manager
	auto& gameStateManager = dae::GameStateManager::GetInstance();
	// Register the playing state
	gameStateManager.RegisterState("SinglePlayer", std::make_unique<dae::SinglePlayer>("SinglePlayer"));
	// Register the menu state
	gameStateManager.RegisterState("Menu", std::make_unique<dae::MenuState>("Menu"));
	// Register the multiplayer state
	gameStateManager.RegisterState("MultiPlayer", std::make_unique<dae::MultiPlayer>("MultiPlayer"));
	gameStateManager.SetState("Menu");
	/*dae::ServiceLocator::GetInstance().RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	auto& soundSystem = dae::ServiceLocator::GetInstance().GetSoundSystem();
	soundSystem.LoadSound("../Data/ms_eat_dot.wav");
	soundSystem.LoadSound("../Data/ms_death.wav");
	soundSystem.LoadSound("../Data/SFX/credit.wav");

	std::shared_ptr<dae::Scene> scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto backGroundImage = std::make_shared<dae::GameObject>();
	backGroundImage->AddComponent<dae::RenderComponent>("background.tga", glm::vec3(0, 0, 0));
	scene->Add(backGroundImage);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fps = std::make_shared<dae::GameObject>();
	auto& fpsComp = fps->AddComponent<dae::FPSComponent>();
	auto& fpsTextComp = fps->AddComponent<dae::TextComponent>("", font, glm::vec3(0, 0, 0));
	fpsTextComp.SetText("FPS: " + fpsComp.GetStringFPS());
	scene->Add(fps);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//Keybinds text objects
	auto keyboarKeybindsText = std::make_shared<dae::GameObject>();
	keyboarKeybindsText->AddComponent<dae::TextComponent>("WASD: Move msPacMan, C inflict self harm, Z and X to score points", font, glm::vec3(0, 60, 0));
	scene->Add(keyboarKeybindsText);

	auto controllerKeybindsText = std::make_shared<dae::GameObject>();
	controllerKeybindsText->AddComponent<dae::TextComponent>("D-Pad: Move mrPacMan, X inflict self harm, A and B to score points", font, glm::vec3(0, 80, 0));
	scene->Add(controllerKeybindsText);

	auto soundKeybindsText = std::make_shared<dae::GameObject>();
	soundKeybindsText->AddComponent<dae::TextComponent>("Score and Damage plays a sound", font, glm::vec3(0, 100, 0));
	scene->Add(soundKeybindsText);


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
	auto& blinkyHealthComp = ghostBlinky->AddComponent<dae::HealthComponent>();
	auto& blinkyScoreComp = ghostBlinky->AddComponent<dae::ScoreComponent>();
	blinkyScoreComp.BindTextComponent(mrPacManScoreText->GetComponentPtr<dae::TextComponent>());
	blinkyHealthComp.BindTextComponent(mrPacManHealthText->GetComponentPtr<dae::TextComponent>());
	auto& ghostAIControllerComp = ghostBlinky->AddComponent<dae::GhostAIControllerComponent>(
		&gridComp,
		msPacMan.get(),
		std::make_unique<dae::BlinkyBehavior>(),
		32
	);
	ghostAIControllerComp.SetSpawnCell(14, 23);

	// Add msPacMan and mrPacMan to the scene
	scene->Add(msPacMan);
	scene->Add(ghostBlinky);
	scene->Add(msPacManScoreText);
	scene->Add(msPacManHealthText);
	scene->Add(mrPacManScoreText);
	scene->Add(mrPacManHealthText);
	scene->Add(pMap);

	//// Create observer for health and score for msPacMan
	auto msPacManHealthObserver = msPacMan->AddObserver(std::make_unique<dae::HealthObserver>());
	msPacManHealthComp.m_Subject.AddObserver(msPacManHealthObserver);
	msPacManHealthObserver->OnNotify(*msPacMan, dae::Event::PLAYER_HIT);

	auto msPacManScoreObserver = msPacMan->AddObserver(std::make_unique<dae::ScoreObserver>());
	msPacManScoreComp.m_Subject.AddObserver(msPacManScoreObserver);
	msPacManScoreComp.m_Subject.Notify(*msPacMan, dae::Event::PLAYER_SCORED);

	//// Create observer for health and score for mrPacMan
	auto mrPacManHealthObserver = ghostBlinky->AddObserver(std::make_unique<dae::HealthObserver>());
	blinkyHealthComp.m_Subject.AddObserver(mrPacManHealthObserver);
	mrPacManHealthObserver->OnNotify(*ghostBlinky, dae::Event::PLAYER_HIT);

	auto mrPacManScoreObserver = ghostBlinky->AddObserver(std::make_unique<dae::ScoreObserver>());
	blinkyScoreComp.m_Subject.AddObserver(mrPacManScoreObserver);
	blinkyScoreComp.m_Subject.Notify(*ghostBlinky, dae::Event::PLAYER_SCORED);*/
}

int main(int, char* [])
{
	//SteamAPI_Init();
	//
	//if (!SteamAPI_Init())
	//{
	//	std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
	//	std::cerr << "Steam path: " << SteamAPI_GetSteamInstallPath() << std::endl;
	//	return 1;
	//}
	//else
	//{
	//	std::cout << "Successfully initialized steam." << std::endl;
	//}

	dae::Minigin engine("../Data/");
	engine.Run(load);

	//SteamAPI_Shutdown();

	return 0;
}

// Couldn't get the steam part to properly works and don't really know how to fix it yet.

