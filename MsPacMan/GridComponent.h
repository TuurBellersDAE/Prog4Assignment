#pragma once
#include <Component.h>
#include "Renderer.h"

enum class CellType
{
	Empty,
	Wall,
	Pellet,
	PowerPellet,
	GhostSpawn,
};

namespace dae
{
	class GridComponent : public Component
	{
	public:
		GridComponent(GameObject* pOwner);
		virtual ~GridComponent() override;

		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;

		void Update() override;
		void Render() const override;

		bool LoadGrid(const std::string& gridFilePath);


		void SetCell(int x, int y, CellType type);
		void SetCellSize(int size) { m_CellSize = size; }

		CellType GetCell(int x, int y) const;
		int GetGridWidth() const { return m_GridWidth; }
		int GetGridHeight() const { return m_GridHeight; }
		int GetCellSize() const { return m_CellSize; }

		bool IsCellWalkable(int x, int y) const;

	private:
		std::vector<std::vector<CellType>> m_Cells{};

		int m_GridWidth;
		int m_GridHeight;
		int m_CellSize;
		glm::vec3 m_GridColor;


		void DrawGrid() const;
		void DrawPellets() const;

		void SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius) const;
		void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) const;

	};
}

