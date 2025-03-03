#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae
{
	class RotationComponent : public Component
	{
	public:
		RotationComponent(GameObject* pOwner);
		~RotationComponent() = default;
		RotationComponent(const RotationComponent& other) = delete;
		RotationComponent(RotationComponent&& other) = delete;
		RotationComponent& operator=(const RotationComponent& other) = delete;
		RotationComponent& operator=(RotationComponent&& other) = delete;

		void Update() override;
		void Render() const override {};

		void SetRotationAngle(float angle) { m_Angle = angle; } // in degrees
		void SetAngleSpeed(float angleSpeed) { m_AngleSpeed = angleSpeed; } // in degrees per second
		void SetPivot(const glm::vec3& pivot); // in world coordinates

	private:
		glm::vec3 RotateAroundPoint(const glm::vec3& rotatingPoint, const glm::vec3& center, float rotationAngle);

		float m_Angle;
		float m_AngleSpeed;
		glm::vec3 m_Pivot;
		glm::vec3 m_Offset;
	};
}
