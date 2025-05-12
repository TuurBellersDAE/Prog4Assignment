#pragma once
#include <Component.h>

namespace dae
{
	class PlayerControllerComponent : public Component
	{
	public:
		PlayerControllerComponent(GameObject* pOwner, int controllerId);
		virtual ~PlayerControllerComponent() override;

		PlayerControllerComponent(const PlayerControllerComponent& other) = delete;
		PlayerControllerComponent(PlayerControllerComponent&& other) = delete;
		PlayerControllerComponent& operator=(const PlayerControllerComponent& other) = delete;
		PlayerControllerComponent& operator=(PlayerControllerComponent&& other) = delete;

		void Update() override;

	private:
		int m_ControllerId;

	};

}

