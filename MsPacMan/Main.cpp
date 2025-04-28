#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
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

#include "InputManager.h"
#include "GameObjectCommand.h"

//#include "Observer.h"
#include "GameObserver.h"

#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

//#include <steam_api.h>

void load()
{
	dae::ServiceLocator::GetInstance().RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	dae::ServiceLocator::GetInstance().GetSoundSystem().LoadSound("../Data/ms_eat_dot.wav");
	dae::ServiceLocator::GetInstance().GetSoundSystem().LoadSound("../Data/ms_death.wav");

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto backGroundImage = std::make_shared<dae::GameObject>();
	backGroundImage->AddComponent<dae::RenderComponent>("background.tga", glm::vec3(0, 0, 0));
	scene.Add(backGroundImage);

	auto logo = std::make_shared<dae::GameObject>();
	logo->AddComponent<dae::RenderComponent>("logo.tga", glm::vec3(216, 180, 0));
	scene.Add(logo);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto title = std::make_shared<dae::GameObject>();
	title->AddComponent<dae::TextComponent>("Programming 4 Assignment", font, glm::vec3(80, 20, 0));
	scene.Add(title);

	auto fps = std::make_shared<dae::GameObject>();
	auto& fpsComp = fps->AddComponent<dae::FPSComponent>();
	auto& fpsTextComp = fps->AddComponent<dae::TextComponent>("", font, glm::vec3(0, 0, 0));
	fpsTextComp.SetText("FPS: " + fpsComp.GetStringFPS());
	scene.Add(fps);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//Keybinds text objects
	auto keyboarKeybindsText = std::make_shared<dae::GameObject>();
	keyboarKeybindsText->AddComponent<dae::TextComponent>("WASD: Move msPacMan, C inflict self harm, Z and X to score points", font, glm::vec3(0, 60, 0));
	scene.Add(keyboarKeybindsText);

	auto controllerKeybindsText = std::make_shared<dae::GameObject>();
	controllerKeybindsText->AddComponent<dae::TextComponent>("D-Pad: Move mrPacMan, X inflict self harm, A and B to score points", font, glm::vec3(0, 80, 0));
	scene.Add(controllerKeybindsText);

	auto soundKeybindsText = std::make_shared<dae::GameObject>();
	soundKeybindsText->AddComponent<dae::TextComponent>("Score and Damage plays a sound", font, glm::vec3(0, 100, 0));
	scene.Add(soundKeybindsText);


	// Create a text object for msPacMan and mrPacMan
	auto msPacManScoreText = std::make_shared<dae::GameObject>();
	msPacManScoreText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 120, 0));
	auto msPacManHealthText = std::make_shared<dae::GameObject>();
	msPacManHealthText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 140, 0));

	auto mrPacManScoreText = std::make_shared<dae::GameObject>();
	mrPacManScoreText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 160, 0));
	auto mrPacManHealthText = std::make_shared<dae::GameObject>();
	mrPacManHealthText->AddComponent<dae::TextComponent>(" ", font, glm::vec3(0, 180, 0));

	// Create msPacMan and mrPacMan
	auto msPacMan = std::make_shared<dae::GameObject>();
	msPacMan->AddComponent<dae::RenderComponent>("msPacMan.png", glm::vec3(80, 150, 0));
	msPacMan->SetSpeed(100.f);
	auto& msPacManHealthComp = msPacMan->AddComponent<dae::HealthComponent>();
	auto& msPacManScoreComp = msPacMan->AddComponent<dae::ScoreComponent>();
	msPacManScoreComp.BindTextComponent(msPacManScoreText->GetComponentPtr<dae::TextComponent>());
	msPacManHealthComp.BindTextComponent(msPacManHealthText->GetComponentPtr<dae::TextComponent>());

	auto mrPacMan = std::make_shared<dae::GameObject>();
	mrPacMan->AddComponent<dae::RenderComponent>("mrPacMan.png", glm::vec3(80, 180, 0));
	msPacMan->SetSpeed(200.f);
	auto& mrPacManHealthComp = mrPacMan->AddComponent<dae::HealthComponent>();
	auto& mrPacManScoreComp = mrPacMan->AddComponent<dae::ScoreComponent>();
	mrPacManScoreComp.BindTextComponent(mrPacManScoreText->GetComponentPtr<dae::TextComponent>());
	mrPacManHealthComp.BindTextComponent(mrPacManHealthText->GetComponentPtr<dae::TextComponent>());

	// Add msPacMan and mrPacMan to the scene
	scene.Add(msPacMan);
	scene.Add(mrPacMan);
	scene.Add(msPacManScoreText);
	scene.Add(msPacManHealthText);
	scene.Add(mrPacManScoreText);
	scene.Add(mrPacManHealthText);

	//// Create observer for health and score for msPacMan
	auto msPacManHealthObserver = msPacMan->AddObserver(std::make_unique<dae::HealthObserver>());
	msPacManHealthComp.m_Subject.AddObserver(msPacManHealthObserver);
	msPacManHealthObserver->OnNotify(*msPacMan, dae::Event::PLAYER_HIT);

	auto msPacManScoreObserver = msPacMan->AddObserver(std::make_unique<dae::ScoreObserver>());
	msPacManScoreComp.m_Subject.AddObserver(msPacManScoreObserver);
	msPacManScoreComp.m_Subject.Notify(*msPacMan, dae::Event::PLAYER_SCORED);

	//// Create observer for health and score for mrPacMan
	auto mrPacManHealthObserver = mrPacMan->AddObserver(std::make_unique<dae::HealthObserver>());
	mrPacManHealthComp.m_Subject.AddObserver(mrPacManHealthObserver);
	mrPacManHealthObserver->OnNotify(*mrPacMan, dae::Event::PLAYER_HIT);

	auto mrPacManScoreObserver = mrPacMan->AddObserver(std::make_unique<dae::ScoreObserver>());
	mrPacManScoreComp.m_Subject.AddObserver(mrPacManScoreObserver);
	mrPacManScoreComp.m_Subject.Notify(*mrPacMan, dae::Event::PLAYER_SCORED);

	// Register commands with the InputManager singleton
	auto& inputManager = dae::InputManager::GetInstance();
	inputManager.RegisterKeyCommand(SDLK_a, std::make_unique<dae::MoveCommand>(msPacMan.get(), glm::vec3(-1.f, 0.f, 0.f)));
	inputManager.RegisterKeyCommand(SDLK_d, std::make_unique<dae::MoveCommand>(msPacMan.get(), glm::vec3(1.f, 0.f, 0.f)));
	inputManager.RegisterKeyCommand(SDLK_w, std::make_unique<dae::MoveCommand>(msPacMan.get(), glm::vec3(0.f, -1.f, 0.f)));
	inputManager.RegisterKeyCommand(SDLK_s, std::make_unique<dae::MoveCommand>(msPacMan.get(), glm::vec3(0.f, 1.f, 0.f)));
	inputManager.RegisterKeyCommand(SDLK_z, std::make_unique<dae::ScoreCommand>(msPacMan.get(), 10));
	inputManager.RegisterKeyCommand(SDLK_x, std::make_unique<dae::ScoreCommand>(msPacMan.get(), 10));
	inputManager.RegisterKeyCommand(SDLK_c, std::make_unique<dae::DamageCommand>(msPacMan.get(), 10));

	// Register controller commands for mrPacMan
	inputManager.RegisterControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT, std::make_unique<dae::MoveCommand>(mrPacMan.get(), glm::vec3(-1.f, 0.f, 0.f)));
	inputManager.RegisterControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT, std::make_unique<dae::MoveCommand>(mrPacMan.get(), glm::vec3(1.f, 0.f, 0.f)));
	inputManager.RegisterControllerCommand(XINPUT_GAMEPAD_DPAD_UP, std::make_unique<dae::MoveCommand>(mrPacMan.get(), glm::vec3(0.f, -1.f, 0.f)));
	inputManager.RegisterControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN, std::make_unique<dae::MoveCommand>(mrPacMan.get(), glm::vec3(0.f, 1.f, 0.f)));
	inputManager.RegisterControllerCommand(XINPUT_GAMEPAD_A, std::make_unique<dae::ScoreCommand>(mrPacMan.get(), 10));
	inputManager.RegisterControllerCommand(XINPUT_GAMEPAD_B, std::make_unique<dae::ScoreCommand>(mrPacMan.get(), 10));
	inputManager.RegisterControllerCommand(XINPUT_GAMEPAD_X, std::make_unique<dae::DamageCommand>(mrPacMan.get(), 10));
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