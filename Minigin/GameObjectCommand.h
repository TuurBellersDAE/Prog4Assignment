#pragma once
#include "GameObject.h"
#include "Command.h"

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

	class MoveLeftCommand final : public GameObjectCommand {
	public:
		MoveLeftCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
		void Execute() override 
		{
			GetGameObject()->Move(-1000.f, 0.f, 0.f);
		}
	};

	class MoveRightCommand final : public GameObjectCommand {
	public:
		MoveRightCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
		void Execute() override 
		{
			GetGameObject()->Move(1000.f, 0.f, 0.f);
		}
	};

	class MoveUpCommand final : public GameObjectCommand {
	public:
		MoveUpCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
		void Execute() override 
		{
			GetGameObject()->Move(0.f, -1000.f, 0.f);
		}
	};

	class MoveDownCommand final : public GameObjectCommand {
	public:
		MoveDownCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
		void Execute() override 
		{
			GetGameObject()->Move(0.f, 1000.f, 0.f);
		}
	};
}
