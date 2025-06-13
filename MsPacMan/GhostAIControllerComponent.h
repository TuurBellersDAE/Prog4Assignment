#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
	class IGhostBehavior;
	class GridComponent;

	class GhostAIControllerComponent : public Component 
	{
	public:
		GhostAIControllerComponent(GameObject* pOwner, GridComponent* pGrid, GameObject* pPlayer, std::unique_ptr<IGhostBehavior> behavior, int spriteSize);
		void Update() override;
		void SetSpawnCell(int cellX, int cellY);
	private:
		GridComponent* m_pGrid;
		GameObject* m_pPlayer;
		std::unique_ptr<IGhostBehavior> m_Behavior;
		int m_SpriteSize;
	};
}