#pragma once
#include <glm.hpp>

namespace dae
{
	class GameObject;
	class GridComponent;
	class GhostAIControllerComponent;

	glm::ivec2 FindNearestWalkableCell(const GridComponent* grid, glm::ivec2 cell);
	glm::ivec2 GetRandomNeighbor(const GridComponent* grid, int cellX, int cellY, const glm::ivec2& lastDirection);


	class IGhostBehavior 
	{
	public:
		virtual ~IGhostBehavior() = default;
		virtual glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid, 
			GhostAIControllerComponent* controller) = 0;
	};

	class BlinkyBehavior : public IGhostBehavior
	{
	public:
		glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid, 
			GhostAIControllerComponent* controller) override;
	};

	class PinkyBehavior : public IGhostBehavior
	{
	public:
		glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid, 
			GhostAIControllerComponent* controller) override;
	};

	class InkyBehavior : public IGhostBehavior
	{
	public:
		glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid, 
			GhostAIControllerComponent* controller) override;
	};

	class ClydeBehavior : public IGhostBehavior
	{
	public:
		glm::ivec2 GetTargetCell(const GameObject* ghost, const GameObject* player, const GridComponent* grid, 
			GhostAIControllerComponent* controller) override;
	};
}

