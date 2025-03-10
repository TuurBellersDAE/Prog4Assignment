#pragma once
#include "Component.h"
#include "glm.hpp"

namespace dae
{
	class MovementComponent : public Component
	{
	public:
		MovementComponent(GameObject* pOwner);
		~MovementComponent() = default;
		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;

		void Update() override;
		void Render() const override {};

		void SetDirection(const glm::vec3& direction);
		void SetSpeed(float speed);

		glm::vec3 GetDirection() const { return m_Direction; }

	private:
		glm::vec3 m_Direction{ 0.0f, 0.0f, 0.0f };
		float m_Speed{ 100.0f };
	};
}
