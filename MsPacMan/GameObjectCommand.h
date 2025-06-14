#pragma once
#include "GameObject.h"
#include "Command.h"
#include "ScoreComponent.h"
#include "HealthComponent.h"
#include <iostream>
#include "ServiceLocator.h"
#include "PlayerControllerComponent.h"

namespace dae
{
	class GameObjectCommand : public Command
	{
	public:
		GameObjectCommand(GameObject* pGameObject);
		virtual ~GameObjectCommand();

	protected:
		GameObject* GetGameObject() const { return m_pGameObject; }

	private:
		GameObject* m_pGameObject;
	};

#pragma region MoveCommands
	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* pGameObject, const glm::vec3& direction)
			: GameObjectCommand(pGameObject), m_Direction(direction) {
		}

		void Execute() override
		{
			auto gameObject = GetGameObject();
			if (gameObject)
			{
				auto controller = gameObject->GetComponentPtr<PlayerControllerComponent>();
				if (controller)
				{
					controller->SetPendingDirection(m_Direction);
				}
			}
		}

	private:
		glm::vec3 m_Direction;
	};

	class MoveLeftCommand final : public GameObjectCommand {
	public:
		MoveLeftCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
		void Execute() override
		{
			auto gameObject = GetGameObject();
			if (gameObject)
			{
				auto direction = gameObject->GetDirection();
				if (direction.x > 0.f)
					gameObject->SetDirection(glm::vec3(0.f, direction.y, direction.z));
				else
					gameObject->SetDirection(glm::vec3(-1.f, direction.y, direction.z));
			}
		}

		//void Stop() override
		//{
		//	auto gameObject = GetGameObject();
		//    if (gameObject)
		//    {
		//        auto direction = gameObject->GetDirection();
		//        if (direction.x < 0.f)
		//            gameObject->SetDirection(glm::vec3(0.f, direction.y, direction.z));
		//    }
		//}
	};

	class MoveRightCommand final : public GameObjectCommand {
	public:
		MoveRightCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
		void Execute() override
		{
			auto gameObject = GetGameObject();
			if (gameObject)
			{
				auto direction = gameObject->GetDirection();
				if (direction.x < 0.f)
					gameObject->SetDirection(glm::vec3(0.f, direction.y, direction.z));
				else
					gameObject->SetDirection(glm::vec3(1.f, direction.y, direction.z));
			}
		}

		//void Stop() override
		//{
		//	auto gameObject = GetGameObject();
		//    if (gameObject)
		//    {
		//        auto direction = gameObject->GetDirection();
		//        if (direction.x > 0.f)
		//            gameObject->SetDirection(glm::vec3(0.f, direction.y, direction.z));
		//    }
		//}
	};

	class MoveUpCommand final : public GameObjectCommand {
	public:
		MoveUpCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
		void Execute() override
		{
			auto gameObject = GetGameObject();
			if (gameObject)
			{
				auto direction = gameObject->GetDirection();
				if (direction.y > 0.f)
					gameObject->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
				else
					gameObject->SetDirection(glm::vec3(direction.x, -1.f, direction.z));
			}
		}

		//void Stop() override
		//{
		//	auto gameObject = GetGameObject();
		//    if (gameObject)
		//    {
		//        auto direction = gameObject->GetDirection();
		//        if (direction.y < 0.f)
		//            gameObject->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
		//    }
		//}
	};

	class MoveDownCommand final : public GameObjectCommand {
	public:
		MoveDownCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
		void Execute() override
		{
			auto gameObject = GetGameObject();
			if (gameObject)
			{
				auto direction = gameObject->GetDirection();
				if (direction.y < 0.f)
					gameObject->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
				else
					gameObject->SetDirection(glm::vec3(direction.x, 1.f, direction.z));
			}
		}

		//void Stop() override
		//{
		//	auto gameObject = GetGameObject();
		//    if (gameObject)
		//    {
		//        auto direction = gameObject->GetDirection();
		//        if (direction.y > 0.f)
		//            gameObject->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
		//    }
		//}
	};
#pragma endregion

	class ScoreCommand final : public GameObjectCommand
	{
	public:
		ScoreCommand(GameObject* pGameObject, int score) : GameObjectCommand(pGameObject), m_Score(score) {}
		void Execute() override
		{
			auto gameObject = GetGameObject();
			if (gameObject)
			{
				auto scoreComponent = gameObject->GetComponentPtr<ScoreComponent>();
				if (scoreComponent)
				{
					scoreComponent->AddScore(m_Score);
					ServiceLocator::GetInstance().GetSoundSystem().Play("../Data/ms_eat_dot.wav", 0.5f, false);
				}
			}
		}

	private:
		int m_Score;
	};

	class DamageCommand final : public GameObjectCommand
	{
	public:
		DamageCommand(GameObject* pGameObject, int damage) : GameObjectCommand(pGameObject), m_Damage(damage) {}
		void Execute() override
		{
			auto gameObject = GetGameObject();
			if (gameObject)
			{
				auto healthComponent = gameObject->GetComponentPtr<HealthComponent>();
				if (healthComponent)
				{
					healthComponent->TakeDamage(m_Damage);
					ServiceLocator::GetInstance().GetSoundSystem().Play("../Data/ms_death.wav", 0.5f, false);
				}
			}
		}

	private:
		int m_Damage;
	};

}
