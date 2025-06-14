#pragma once
#include "Component.h"
#include "GhostState.h"
#include <memory>
#include "Observer.h"

namespace dae
{
	class IGhostBehavior;
	class GridComponent;

	class GhostAIControllerComponent : public Component, public Observer
	{
	public:
		GhostAIControllerComponent(GameObject* pOwner, GridComponent* pGrid, GameObject* pPlayer, std::unique_ptr<IGhostBehavior> behavior, int spriteSize);
		void Update() override;
		void Render() const override;
		void SetSpawnCell(int cellX, int cellY);

		GhostState* GetCurrentState() const { return m_CurrentState.get(); }

		void OnNotify(const GameObject& gameObject, const Event& event) override;

		GridComponent* GetGrid() const { return m_pGrid; }
		GameObject* GetPlayer() const { return m_pPlayer; }
		IGhostBehavior* GetBehavior() const { return m_Behavior.get(); }
		int GetSpriteSize() const { return m_SpriteSize; }
		GameObject* GetOwner() const { return Component::GetOwner(); }
		glm::ivec2 GetLastDirection() const { return m_LastDirection; }

		void SetLastDirection(const glm::ivec2& dir) { m_LastDirection = dir; }
		void SetCurrentTargetCell(const glm::ivec2& cell) { m_CurrentTargetCell = cell; }
		glm::ivec2 GetCurrentTargetCell() const { return m_CurrentTargetCell; }


	private:
		GridComponent* m_pGrid;
		GameObject* m_pPlayer;
		std::unique_ptr<IGhostBehavior> m_Behavior;
		int m_SpriteSize;

		std::unique_ptr<GhostState> m_CurrentState;
		glm::ivec2 m_LastDirection{ 0, 0 };
		glm::ivec2 m_CurrentTargetCell{ -1, -1 };

		float m_StateTimer = 0.0f;
		float m_FrightenedDuration = 7.0f;

		void SetState(std::unique_ptr<GhostState> newState)
		{
			if (m_CurrentState)
				m_CurrentState->OnExit();
			m_CurrentState = std::move(newState);
			m_StateTimer = 0.0f; // Reset timer on state change
			if (m_CurrentState)
				m_CurrentState->OnEnter();
		}
	};
}