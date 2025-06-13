#pragma once
#include <glm.hpp>

namespace dae
{
	class GameObject;
	class GridComponent;

	glm::ivec2 FindNearestWalkableCell(const GridComponent* grid, glm::ivec2 cell);

	class IGhostBehavior 
	{
	public:
		virtual ~IGhostBehavior() = default;
		virtual glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid) = 0;
	};

	class BlinkyBehavior : public IGhostBehavior
	{
	public:
		glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid) override;
	};

	class PinkyBehavior : public IGhostBehavior
	{
	public:
		glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid) override;
	};

	class InkyBehavior : public IGhostBehavior
	{
	public:
		glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid) override;
	};

	class ClydeBehavior : public IGhostBehavior
	{
	public:
		glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid) override;
	};
}

