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
//#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotationComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//auto go = std::make_shared<dae::GameObject>();
	//go->SetTexture("background.tga");
	//scene.Add(go);
	//
	//go = std::make_shared<dae::GameObject>();
	//go->SetTexture("logo.tga");
	//go->SetPosition(216, 180);
	//scene.Add(go);
	//
	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = std::make_shared<dae::TextObject>("Programming 4 Assignment", font);
	//to->SetPosition(80, 20);

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
	//fpsComp.SetTextComponent(&fpsTextComp);
	scene.Add(fps);


	auto msPacMan = std::make_shared<dae::GameObject>();
	msPacMan->AddComponent<dae::RenderComponent>("msPacMan.png", glm::vec3(80, 150, 0));
	auto& rotationComp = msPacMan->AddComponent<dae::RotationComponent>();
	rotationComp.SetPivot(glm::vec3(120, 150, 0));
	rotationComp.SetAngleSpeed(360.f);
	scene.Add(msPacMan);

	auto mrPacMan = std::make_shared<dae::GameObject>();
	mrPacMan->AddComponent<dae::RenderComponent>("mrPacMan.png", glm::vec3(80, 180, 0));
	mrPacMan->SetParent(msPacMan);
	auto& rotationComp2 = mrPacMan->AddComponent<dae::RotationComponent>();
	rotationComp2.SetPivot(msPacMan->GetWorldPosition());
	rotationComp2.SetAngleSpeed(360.f);
	scene.Add(mrPacMan);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}