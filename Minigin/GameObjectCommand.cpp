#include "GameObjectCommand.h"

dae::GameObjectCommand::GameObjectCommand(GameObject* pGameObject)
	: m_pGameObject(pGameObject)
{
}

dae::GameObjectCommand::~GameObjectCommand()
{
	m_pGameObject = nullptr;
}
