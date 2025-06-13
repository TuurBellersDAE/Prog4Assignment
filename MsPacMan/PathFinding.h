#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <glm.hpp>
#include "GridComponent.h"

namespace dae
{
	struct IVec2Hash
	{
		std::size_t operator()(const glm::ivec2& v) const 
		{
			return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
		}
	};

	inline std::vector<glm::ivec2> FindPathBFS(const GridComponent* grid, glm::ivec2 start, glm::ivec2 goal)
	{
		// Defensive: check if start and goal are in bounds
		int w = grid->GetGridWidth();
		int h = grid->GetGridHeight();
		if (start.x < 0 || start.x >= w || start.y < 0 || start.y >= h ||
			goal.x < 0 || goal.x >= w || goal.y < 0 || goal.y >= h)
		{
			return {}; // No path if out of bounds
		}

		std::queue<glm::ivec2> frontier;
		std::unordered_map<glm::ivec2, glm::ivec2, IVec2Hash> cameFrom;
		frontier.push(start);
		cameFrom[start] = start;

		const glm::ivec2 directions[] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

		while (!frontier.empty()) 
		{
			glm::ivec2 current = frontier.front();
			frontier.pop();

			if (current == goal)
				break;

			for (const auto& dir : directions)
			{
				glm::ivec2 next = current + dir;
				if (next.x >= 0 && next.x < w && next.y >= 0 && next.y < h) 
				{
					if (grid->IsCellWalkable(next.x, next.y) && cameFrom.find(next) == cameFrom.end()) 
					{
						frontier.push(next);
						cameFrom[next] = current;
					}
				}
			}
		}

		std::vector<glm::ivec2> path;
		glm::ivec2 current = goal;
		if (cameFrom.find(goal) == cameFrom.end())
			return path; // No path found

		while (current != start) 
		{
			path.push_back(current);
			current = cameFrom[current];
		}
		std::reverse(path.begin(), path.end());
		return path;
	}
}
