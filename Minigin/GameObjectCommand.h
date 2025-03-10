#pragma once
#include "GameObject.h"
#include "Command.h"
#include "MovementComponent.h"

#include <iostream>

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

	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* pGameObject, const glm::vec3& direction)
			: GameObjectCommand(pGameObject), m_Direction(direction) {
		}

		void Execute() override
		{
			auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
			if (movementComponent)
			{
				auto currentDirection = movementComponent->GetDirection();
				movementComponent->SetDirection(currentDirection + m_Direction);
			}
            else
            {
				std::cout << "No MovementComponent found on GameObject" << std::endl;
            }
		}

		void Stop() override
		{
			auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
			if (movementComponent)
			{
				auto currentDirection = movementComponent->GetDirection();
				//movementComponent->SetDirection(currentDirection - m_Direction);

				if (m_Direction.x != 0.f)
				{
					if (currentDirection.x > 0.f && m_Direction.x > 0.f)
						movementComponent->SetDirection(glm::vec3(0.f, currentDirection.y, currentDirection.z));
					else if (currentDirection.x < 0.f && m_Direction.x < 0.f)
						movementComponent->SetDirection(glm::vec3(0.f, currentDirection.y, currentDirection.z));
				}
				else if (m_Direction.y != 0.f)
				{
					if (currentDirection.y > 0.f && m_Direction.y > 0.f)
						movementComponent->SetDirection(glm::vec3(currentDirection.x, 0.f, currentDirection.z));
					else if (currentDirection.y < 0.f && m_Direction.y < 0.f)
						movementComponent->SetDirection(glm::vec3(currentDirection.x, 0.f, currentDirection.z));
				}
				else if (m_Direction.z != 0.f)
				{
					if (currentDirection.z > 0.f && m_Direction.z > 0.f)
						movementComponent->SetDirection(glm::vec3(currentDirection.x, currentDirection.y, 0.f));
					else if (currentDirection.z < 0.f && m_Direction.z < 0.f)
						movementComponent->SetDirection(glm::vec3(currentDirection.x, currentDirection.y, 0.f));
				}
			}
            else
            {
				std::cout << "No MovementComponent found on GameObject" << std::endl;
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
            auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
            if (movementComponent)
            {
                auto direction = movementComponent->GetDirection();
                if (direction.x > 0.f)
                    movementComponent->SetDirection(glm::vec3(0.f, direction.y, direction.z));
                else
                    movementComponent->SetDirection(glm::vec3(-1.f, direction.y, direction.z));
            }
        }

        void Stop() override
        {
            auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
            if (movementComponent)
            {
                auto direction = movementComponent->GetDirection();
                if (direction.x < 0.f)
                    movementComponent->SetDirection(glm::vec3(0.f, direction.y, direction.z));
            }
        }
    };

    class MoveRightCommand final : public GameObjectCommand {
    public:
        MoveRightCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
        void Execute() override
        {
            auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
            if (movementComponent)
            {
                auto direction = movementComponent->GetDirection();
                if (direction.x < 0.f)
                    movementComponent->SetDirection(glm::vec3(0.f, direction.y, direction.z));
                else
                    movementComponent->SetDirection(glm::vec3(1.f, direction.y, direction.z));
            }
        }

        void Stop() override
        {
            auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
            if (movementComponent)
            {
                auto direction = movementComponent->GetDirection();
                if (direction.x > 0.f)
                    movementComponent->SetDirection(glm::vec3(0.f, direction.y, direction.z));
            }
        }
    };

    class MoveUpCommand final : public GameObjectCommand {
    public:
        MoveUpCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
        void Execute() override
        {
            auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
            if (movementComponent)
            {
                auto direction = movementComponent->GetDirection();
                if (direction.y > 0.f)
                    movementComponent->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
                else
                    movementComponent->SetDirection(glm::vec3(direction.x, -1.f, direction.z));
            }
        }

        void Stop() override
        {
            auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
            if (movementComponent)
            {
                auto direction = movementComponent->GetDirection();
                if (direction.y < 0.f)
                    movementComponent->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
            }
        }
    };

    class MoveDownCommand final : public GameObjectCommand {
    public:
        MoveDownCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
        void Execute() override
        {
            auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
            if (movementComponent)
            {
                auto direction = movementComponent->GetDirection();
                if (direction.y < 0.f)
                    movementComponent->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
                else
                    movementComponent->SetDirection(glm::vec3(direction.x, 1.f, direction.z));
            }
        }

        void Stop() override
        {
            auto movementComponent = GetGameObject()->GetComponentPtr<MovementComponent>();
            if (movementComponent)
            {
                auto direction = movementComponent->GetDirection();
                if (direction.y > 0.f)
                    movementComponent->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
            }
        }
    };
}
