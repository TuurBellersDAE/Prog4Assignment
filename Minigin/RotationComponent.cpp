#include "RotationComponent.h"
#include "GameObject.h"
#include "Timer.h"
#include <iostream>

namespace dae
{
	RotationComponent::RotationComponent(GameObject* pOwner)
		: Component(pOwner), m_Angle(0.0f), m_AngleSpeed(0.0f), m_Pivot(0.0f, 0.0f, 0.0f)
	{
		m_Offset = GetOwner()->GetWorldPosition() - m_Pivot; // Store the initial offset
	}

	void RotationComponent::Update()
	{
		// Update the angle based on the angle speed
		m_Angle += m_AngleSpeed * Timer::GetInstance().GetDeltaTime();

		// Ensure the angle stays within 0-360 degrees
		if (m_Angle >= 360.0f) m_Angle -= 360.0f;
		if (m_Angle < 0.0f) m_Angle += 360.0f;

		// Calculate the new position based on the original offset
		glm::vec3 newPosition = RotateAroundPoint(m_Offset + m_Pivot, m_Pivot, m_Angle);

		// Set the new position of the GameObject
		GetOwner()->SetWorldPosition(newPosition);

		// If the GameObject has a parent, update the pivot to the parent's world position
		if (auto parent = GetOwner()->GetParentPtr())
		{
			m_Pivot = parent->GetWorldPosition();
		}
	}

	void RotationComponent::SetPivot(const glm::vec3& pivot)
	{
		m_Pivot = pivot;
		m_Offset = GetOwner()->GetWorldPosition() - m_Pivot;
	}

	glm::vec3 RotationComponent::RotateAroundPoint(const glm::vec3& rotatingPoint, const glm::vec3& center, float rotationAngle)
	{
		float angleRad = glm::radians(rotationAngle);

		float translatedX = rotatingPoint.x - center.x;
		float translatedY = rotatingPoint.y - center.y;

		float rotatedX = translatedX * cos(angleRad) - translatedY * sin(angleRad);
		float rotatedY = translatedX * sin(angleRad) + translatedY * cos(angleRad);

		return { rotatedX + center.x, rotatedY + center.y, rotatingPoint.z };
	}
}
