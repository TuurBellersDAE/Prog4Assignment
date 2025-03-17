#pragma once
#include "GameObject.h"
#include "Command.h"

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
			auto gameObject = GetGameObject();
			if (gameObject)
			{
                gameObject->SetDirection(m_Direction);
			}
		}


		void Stop() override
		{
			auto gameObject = GetGameObject();
			if (gameObject)
			{
				auto currentDirection = gameObject->GetDirection();
            
				if (m_Direction.x != 0.f && currentDirection.x == m_Direction.x)
				{
					gameObject->SetDirection(glm::vec3(0.f, currentDirection.y, currentDirection.z));
				}
				else if (m_Direction.y != 0.f && currentDirection.y == m_Direction.y)
				{
					gameObject->SetDirection(glm::vec3(currentDirection.x, 0.f, currentDirection.z));
				}
				else if (m_Direction.z != 0.f && currentDirection.z == m_Direction.z)
				{
					gameObject->SetDirection(glm::vec3(currentDirection.x, currentDirection.y, 0.f));
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

        void Stop() override
        {
			auto gameObject = GetGameObject();
            if (gameObject)
            {
                auto direction = gameObject->GetDirection();
                if (direction.x < 0.f)
                    gameObject->SetDirection(glm::vec3(0.f, direction.y, direction.z));
            }
        }
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

        void Stop() override
        {
			auto gameObject = GetGameObject();
            if (gameObject)
            {
                auto direction = gameObject->GetDirection();
                if (direction.x > 0.f)
                    gameObject->SetDirection(glm::vec3(0.f, direction.y, direction.z));
            }
        }
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

        void Stop() override
		{
			auto gameObject = GetGameObject();
            if (gameObject)
            {
                auto direction = gameObject->GetDirection();
                if (direction.y < 0.f)
                    gameObject->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
            }
        }
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

        void Stop() override
		{
			auto gameObject = GetGameObject();
            if (gameObject)
            {
                auto direction = gameObject->GetDirection();
                if (direction.y > 0.f)
                    gameObject->SetDirection(glm::vec3(direction.x, 0.f, direction.z));
            }
        }
    };
}
