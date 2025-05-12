#include "PlayerControllerComponent.h"
#include "InputManager.h"
#include "GameObject.h"
#include "GameObjectCommand.h"
#include "SpriteAnimatorComponent.h"

dae::PlayerControllerComponent::PlayerControllerComponent(GameObject* pOwner, int controllerId)
	: Component(pOwner), m_ControllerId(controllerId)
{
	// Register keyboard commands
	auto& inputManager = InputManager::GetInstance();

	if (pOwner)
	{
		// Keyboard movement commands
		//inputManager.RegisterKeyCommand(SDLK_a, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(-1.f, 0.f, 0.f)));
		//inputManager.RegisterKeyCommand(SDLK_d, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(1.f, 0.f, 0.f)));
		//inputManager.RegisterKeyCommand(SDLK_w, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(0.f, -1.f, 0.f)));
		//inputManager.RegisterKeyCommand(SDLK_s, std::make_unique<dae::MoveCommand>(pOwner, glm::vec3(0.f, 1.f, 0.f)));

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
	if(GetOwner()->GetComponentPtr<SpriteAnimatorComponent>())
	{
		auto animator = GetOwner()->GetComponentPtr<SpriteAnimatorComponent>();
		if (GetOwner()->GetDirection() == glm::vec3(-1.f, 0.f, 0.f))
		{
			animator->Texture("MsPacMan_Left.png");
		}
		else if (GetOwner()->GetDirection() == glm::vec3(1.f, 0.f, 0.f))
		{
			animator->Texture("MsPacMan_Right.png");
		}
		else if (GetOwner()->GetDirection() == glm::vec3(0.f, -1.f, 0.f))
		{
			animator->Texture("MsPacMan_Up.png");
		}
		else if (GetOwner()->GetDirection() == glm::vec3(0.f, 1.f, 0.f))
		{
			animator->Texture("MsPacMan_Down.png");
		}
	}
}

