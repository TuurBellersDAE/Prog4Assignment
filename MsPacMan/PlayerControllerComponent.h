#pragma once
#include <Component.h>
#include "GridComponent.h"

namespace dae
{
	class PlayerControllerComponent : public Component
	{
	public:
		PlayerControllerComponent(GameObject* pOwner, int controllerId, GridComponent* pGrid, int spriteSize = 16);
		virtual ~PlayerControllerComponent() override;

		PlayerControllerComponent(const PlayerControllerComponent& other) = delete;
		PlayerControllerComponent(PlayerControllerComponent&& other) = delete;
		PlayerControllerComponent& operator=(const PlayerControllerComponent& other) = delete;
		PlayerControllerComponent& operator=(PlayerControllerComponent&& other) = delete;

		void Update() override;

		void SetPendingDirection(const glm::vec3& dir) { m_PendingDirection = dir; }

		void SetSpawnCell(int cellX, int cellY);

	private:
		int m_ControllerId;
		GridComponent* m_pGrid = nullptr;
		int m_SpriteSize = 16;
		glm::vec3 m_PendingDirection{ 0.f, 0.f, 0.f };
		CellType m_LastCellType = CellType::Empty;
		float m_PelletSoundGraceTimer = 0.0f;
		const float m_PelletSoundGraceDuration = 0.5f; // seconds, adjust as needed
		bool m_PelletSoundActive = false;

	};
}


