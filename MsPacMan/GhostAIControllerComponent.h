#pragma once
#include "Component.h"
#include "GhostState.h"
#include <memory>
#include "Observer.h"
#include <queue>
#include <vector>

namespace dae
{
	class IGhostBehavior;
	class GridComponent;

	class GhostAIControllerComponent : public Component, public Observer
	{
	public:
		GhostAIControllerComponent(
			GameObject* pOwner,
			GridComponent* pGrid,
			const std::vector<std::shared_ptr<GameObject>>& players,
			std::unique_ptr<IGhostBehavior> behavior,
			int spriteSize);

		void Update() override;
		void Render() const override;
		void SetSpawnCell(int cellX, int cellY);

		GhostState* GetCurrentState() const { return m_CurrentState.get(); }

		void OnNotify(const GameObject& gameObject, const Event& event) override;

		GridComponent* GetGrid() const { return m_pGrid; }
		const std::vector<std::shared_ptr<GameObject>>& GetPlayers() const { return m_pPlayers; }
		std::shared_ptr<GameObject> GetPrimaryPlayer() const { return m_pPlayers.empty() ? nullptr : m_pPlayers[0]; }
		IGhostBehavior* GetBehavior() const { return m_Behavior.get(); }
		int GetSpriteSize() const { return m_SpriteSize; }
		GameObject* GetOwner() const { return Component::GetOwner(); }
		glm::ivec2 GetLastDirection() const { return m_LastDirection; }

		void SetLastDirection(const glm::ivec2& dir) { m_LastDirection = dir; }
		void SetCurrentTargetCell(const glm::ivec2& cell) { m_CurrentTargetCell = cell; }
		glm::ivec2 GetCurrentTargetCell() const { return m_CurrentTargetCell; }

		void SetState(std::unique_ptr<GhostState> newState)
		{
			if (m_CurrentState)
				m_CurrentState->OnExit();
			m_CurrentState = std::move(newState);
			m_StateTimer = 0.0f;
			m_WaitingInHouse = false; // Reset when leaving EatenState
			if (m_CurrentState)
				m_CurrentState->OnEnter();
		}
	private:
		GridComponent* m_pGrid;
		std::vector<std::shared_ptr<GameObject>> m_pPlayers;
		std::unique_ptr<IGhostBehavior> m_Behavior;
		int m_SpriteSize;

		std::unique_ptr<GhostState> m_CurrentState;
		glm::ivec2 m_LastDirection{ 0, 0 };
		glm::ivec2 m_CurrentTargetCell{ -1, -1 };

		float m_StateTimer = 0.0f;
		float m_FrightenedDuration = 7.0f;

		static std::queue<GhostAIControllerComponent*> s_RespawnQueue;
		bool m_WaitingInHouse = false;
		float m_RespawnWaitTime = 10.0f; // seconds to wait in house

		static int s_GhostsEatenInFrightened;

	};
}
