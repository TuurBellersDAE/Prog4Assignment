#include "MovementComponent.h"
#include "GameObject.h"
#include "Timer.h"

#include <algorithm>

namespace dae
{
	MovementComponent::MovementComponent(GameObject* pOwner)
		: Component(pOwner)
	{
	}

	void MovementComponent::Update()
	{
		auto deltaTime = Timer::GetInstance().GetDeltaTime();
		glm::vec3 movement = m_Direction * m_Speed * deltaTime;
		GetOwner()->Move(movement.x, movement.y, movement.z);
	}

    void MovementComponent::SetDirection(const glm::vec3& direction)
    {
        m_Direction.x = std::clamp(direction.x, -1.0f, 1.0f);
        m_Direction.y = std::clamp(direction.y, -1.0f, 1.0f);
        m_Direction.z = std::clamp(direction.z, -1.0f, 1.0f);
    }

	void MovementComponent::SetSpeed(float speed)
	{
		m_Speed = speed;
	}
}
