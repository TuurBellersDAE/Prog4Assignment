#include "Transform.h"
#include "Timer.h"

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::Transform::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

void dae::Transform::Move(float dx, float dy, float dz)
{
	auto deltaTime = Timer::GetInstance().GetDeltaTime();
	m_position.x += dx * deltaTime * m_movementSpeed;
	m_position.y += dy * deltaTime * m_movementSpeed;
	m_position.z += dz * deltaTime * m_movementSpeed;
}

const glm::vec3& dae::Transform::GetPosition() const
{
	return m_position;
}
