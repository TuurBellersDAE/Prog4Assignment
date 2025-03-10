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
#include "MovementComponent.h"
#include "InputManager.h"
#include "GameObjectCommand.h"


void load()
{
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

	auto msPacMan = std::make_shared<dae::GameObject>();
	msPacMan->AddComponent<dae::RenderComponent>("msPacMan.png", glm::vec3(80, 150, 0));
	//auto& rotationComp = msPacMan->AddComponent<dae::RotationComponent>();
	//rotationComp.SetPivot(glm::vec3(120, 150, 0));
	//rotationComp.SetAngleSpeed(360.f);
	auto& movementComp = msPacMan->AddComponent<dae::MovementComponent>();
	movementComp.SetSpeed(100.f);
	scene.Add(msPacMan);

	auto mrPacMan = std::make_shared<dae::GameObject>();
	mrPacMan->AddComponent<dae::RenderComponent>("mrPacMan.png", glm::vec3(80, 180, 0));
	//mrPacMan->SetParent(msPacMan);
	//auto& rotationComp2 = mrPacMan->AddComponent<dae::RotationComponent>();
	//rotationComp2.SetPivot(msPacMan->GetWorldPosition());
	//rotationComp2.SetAngleSpeed(360.f);
	auto& movementComp2 = mrPacMan->AddComponent<dae::MovementComponent>();
	movementComp2.SetSpeed(200.f);
	scene.Add(mrPacMan);

	// Register commands with the InputManager singleton
	auto& inputManager = dae::InputManager::GetInstance();
	//inputManager.RegisterCommand(SDLK_LEFT, std::make_unique<dae::MoveLeftCommand>(msPacMan.get()));								//
	//inputManager.RegisterCommand(SDLK_RIGHT, std::make_unique<dae::MoveRightCommand>(msPacMan.get()));							//
	//inputManager.RegisterCommand(SDLK_UP, std::make_unique<dae::MoveUpCommand>(msPacMan.get()));									//
	//inputManager.RegisterCommand(SDLK_DOWN, std::make_unique<dae::MoveDownCommand>(msPacMan.get()));								//
																																	// BOTH WORK KINDA BUT I PREFER THE TOP ONE
	inputManager.RegisterCommand(SDLK_a, std::make_unique<dae::MoveCommand>(msPacMan.get(), glm::vec3(-1.f, 0.f, 0.f)));			//
	inputManager.RegisterCommand(SDLK_d, std::make_unique<dae::MoveCommand>(msPacMan.get(), glm::vec3(1.f, 0.f, 0.f)));			    //
	inputManager.RegisterCommand(SDLK_w, std::make_unique<dae::MoveCommand>(msPacMan.get(), glm::vec3(0.f, -1.f, 0.f)));			//
	inputManager.RegisterCommand(SDLK_s, std::make_unique<dae::MoveCommand>(msPacMan.get(), glm::vec3(0.f, 1.f, 0.f)));			    //

	// Register controller commands for mrPacMan
	inputManager.RegisterCommand(XINPUT_GAMEPAD_DPAD_LEFT, std::make_unique<dae::MoveCommand>(mrPacMan.get(), glm::vec3(-1.f, 0.f, 0.f)));
	inputManager.RegisterCommand(XINPUT_GAMEPAD_DPAD_RIGHT, std::make_unique<dae::MoveCommand>(mrPacMan.get(), glm::vec3(1.f, 0.f, 0.f)));
	inputManager.RegisterCommand(XINPUT_GAMEPAD_DPAD_UP, std::make_unique<dae::MoveCommand>(mrPacMan.get(), glm::vec3(0.f, -1.f, 0.f)));
	inputManager.RegisterCommand(XINPUT_GAMEPAD_DPAD_DOWN, std::make_unique<dae::MoveCommand>(mrPacMan.get(), glm::vec3(0.f, 1.f, 0.f)));
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}
