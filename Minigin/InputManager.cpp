#include <SDL.h>
#include "InputManager.h"
#include <windows.h>
#include <XInput.h>
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

void dae::InputManager::RegisterCommand(SDL_Keycode key, std::unique_ptr<Command> command) 
{
    m_commands[key] = std::move(command);
}

void dae::InputManager::UnregisterCommand(SDL_Keycode key) 
{
    m_commands.erase(key);
}

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		if (e.type == SDL_KEYDOWN)
		{
			auto it = m_commands.find(e.key.keysym.sym);
			if (it != m_commands.end())
			{
				it->second->Execute();
			}
		}
		if (e.type == SDL_KEYUP)
		{
			auto it = m_commands.find(e.key.keysym.sym);
			if (it != m_commands.end())
			{
				it->second->Stop(); // Assuming you have a Stop method in your Command class
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			// Handle mouse button down events if needed
		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

