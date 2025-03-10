#pragma once
#include <glm.hpp>

namespace dae
{
	class Transform final {
	public:
		const glm::vec3& GetPosition() const;
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);
		void Move(float dx, float dy, float dz);
		void SetMovementSpeed(float speed) { m_movementSpeed = speed; }
		float GetMovementSpeed() const { return m_movementSpeed; }

	private:
		glm::vec3 m_position;
		float m_movementSpeed{ 100.0f }; // Default movement speed
	};
}
