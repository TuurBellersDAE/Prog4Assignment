#include "GridComponent.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <SDL_ttf.h>

dae::GridComponent::GridComponent(GameObject* pOwner)
	: Component(pOwner),
	m_GridWidth(0),
	m_GridHeight(0),
	m_CellSize(32),
	m_GridColor(1.0f, 1.0f, 1.0f)
{
	// Initialize SDL_ttf if not already done
	if (TTF_WasInit() == 0) {
		if (TTF_Init() == -1) {
			throw std::runtime_error("Failed to initialize SDL_ttf");
		}
	}
}

dae::GridComponent::~GridComponent()
{
}

void dae::GridComponent::Update()
{
}

void dae::GridComponent::Render() const
{
#ifdef _DEBUG
	if (m_GridWidth > 0 && m_GridHeight > 0)
	{
		DrawGrid();
	}
	else
	{
		SDL_Log("Grid not loaded or empty.");
	}
#endif

	if (m_GridWidth > 0 && m_GridHeight > 0)
	{
		DrawPellets();
	}
	else
	{
		SDL_Log("Grid not loaded or empty.");
	}
}

bool dae::GridComponent::LoadGrid(const std::string& gridFilePath)
{
	std::ifstream file(gridFilePath);
	if (!file.is_open()) return false;

	m_Cells.clear();
	std::string line;
	while (std::getline(file, line))
	{
		std::vector<CellType> row;
		for (char c : line)
		{
			switch (c)
			{
			case '#': row.push_back(CellType::Wall); break;
			case '.': row.push_back(CellType::Pellet); break;
			case 'o': row.push_back(CellType::PowerPellet); break;
			default:  row.push_back(CellType::Empty); break;
			}
		}
		m_Cells.push_back(row);
	}
	m_GridHeight = static_cast<int>(m_Cells.size());
	m_GridWidth = m_GridHeight > 0 ? static_cast<int>(m_Cells[0].size()) : 0;
	return true;
}

void dae::GridComponent::SetCell(int x, int y, CellType type)
{
	if (x < 0 || y < 0 || y >= m_GridHeight || x >= m_GridWidth)
	{
		return;
	}

	if (m_Cells[y][x] == CellType::Empty || m_Cells[y][x] == CellType::Wall)
	{
		return;
	}
	m_Cells[y][x] = type;
}

CellType dae::GridComponent::GetCell(int x, int y) const
{
	if (x < 0 || x >= m_GridWidth || y < 0 || y >= m_GridHeight)
		return CellType::Wall; // or a safe default
	return m_Cells[y][x];
}

bool dae::GridComponent::IsCellWalkable(int x, int y) const
{
    if (x < 0 || x >= m_GridWidth || y < 0 || y >= m_GridHeight)
        return false;
    return GetCell(x, y) != CellType::Wall;
}

void dae::GridComponent::DrawGrid() const
{
	SDL_Renderer* sdlRenderer = Renderer::GetInstance().GetSDLRenderer();
	if (!sdlRenderer) return;

	// Optionally set the grid color (light gray)
	SDL_SetRenderDrawColor(sdlRenderer, 180, 180, 180, 100);

	// Get the grid's world position (top-left corner)
	glm::vec3 gridOrigin = GetOwner() ? GetOwner()->GetWorldPosition() : glm::vec3(0, 0, 0);

	// Get mouse position
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	// Convert mouse position to grid cell coordinates
	int hoveredX = (mouseX - static_cast<int>(gridOrigin.x)) / m_CellSize;
	int hoveredY = (mouseY - static_cast<int>(gridOrigin.y)) / m_CellSize;

	// Load a font (ensure the path and font file exist in your project)
	static TTF_Font* font = nullptr;
	if (!font) {
		font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 16); // Adjust path/size as needed
		if (!font) {
			SDL_Log("Failed to load font: %s", TTF_GetError());
			return;
		}
	}

	for (int y = 0; y < m_GridHeight; ++y)
	{
		for (int x = 0; x < m_GridWidth; ++x)
		{
			SDL_Rect cellRect;
			cellRect.x = static_cast<int>(gridOrigin.x) + x * m_CellSize;
			cellRect.y = static_cast<int>(gridOrigin.y) + y * m_CellSize;
			cellRect.w = m_CellSize;
			cellRect.h = m_CellSize;

			SDL_RenderDrawRect(sdlRenderer, &cellRect);

			// Only render the coordinates if this cell is hovered
			if (x == hoveredX && y == hoveredY &&
				x >= 0 && x < m_GridWidth && y >= 0 && y < m_GridHeight)
			{
				std::string coordStr = std::to_string(x + 1) + "," + std::to_string(y + 1);
				SDL_Color textColor = { 255, 0, 0, 255 };
				SDL_Surface* textSurface = TTF_RenderText_Blended(font, coordStr.c_str(), textColor);
				if (textSurface) {
					SDL_Texture* textTexture = SDL_CreateTextureFromSurface(sdlRenderer, textSurface);
					if (textTexture) {
						int textW = 0, textH = 0;
						SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
						SDL_Rect textRect;
						textRect.x = cellRect.x + (cellRect.w - textW) / 2;
						textRect.y = cellRect.y + (cellRect.h - textH) / 2;
						textRect.w = textW;
						textRect.h = textH;
						SDL_RenderCopy(sdlRenderer, textTexture, nullptr, &textRect);
						SDL_DestroyTexture(textTexture);
					}
					SDL_FreeSurface(textSurface);
				}
			}
		}
	}
}

void dae::GridComponent::DrawPellets() const
{
	SDL_Renderer* sdlRenderer = Renderer::GetInstance().GetSDLRenderer();
	if (!sdlRenderer) return;

	for (int y = 0; y < m_GridHeight; ++y)
	{
		for (int x = 0; x < m_GridWidth; ++x)
		{
			int centerX = x * m_CellSize + static_cast<int>(GetOwner()->GetWorldPosition().x) + m_CellSize / 2;
			int centerY = y * m_CellSize + static_cast<int>(GetOwner()->GetWorldPosition().y) + m_CellSize / 2;

			if (m_Cells[y][x] == CellType::Pellet)
			{
				SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 0, 255); // Yellow
				int radius = m_CellSize / 8;
				SDL_RenderFillCircle(sdlRenderer, centerX, centerY, radius);
			}
			else if (m_Cells[y][x] == CellType::PowerPellet)
			{
				SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255); // White
				int radius = m_CellSize / 4;
				SDL_RenderFillCircle(sdlRenderer, centerX, centerY, radius);
			}
		}
	}
}

void dae::GridComponent::SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius) const
{
	const int diameter = (radius * 2);
	int offsetX = radius - 1;
	int offsetY = 0;
	int tx = 1;
	int ty = 1;
	int error = tx - diameter;

	while (offsetX >= offsetY)
	{
		SDL_RenderDrawPoint(renderer, x + offsetX, y - offsetY);
		SDL_RenderDrawPoint(renderer, x + offsetX, y + offsetY);
		SDL_RenderDrawPoint(renderer, x - offsetX, y - offsetY);
		SDL_RenderDrawPoint(renderer, x - offsetX, y + offsetY);
		SDL_RenderDrawPoint(renderer, x + offsetY, y - offsetX);
		SDL_RenderDrawPoint(renderer, x + offsetY, y + offsetX);
		SDL_RenderDrawPoint(renderer, x - offsetY, y - offsetX);
		SDL_RenderDrawPoint(renderer, x - offsetY, y + offsetX);

		if (error <= 0)
		{
			++offsetY;
			error += ty;
			ty += 2;
		}
		if (error > 0)
		{
			--offsetX;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

void dae::GridComponent::SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) const
{
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
}


