#define WIN32_LEAN_AND_MEAN
#include <SDL.h>
#include "InputManager.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream> // For debug output

#pragma comment(lib, "Xinput9_1_0.lib")

void dae::InputManager::RegisterKeyCommand(SDL_Keycode key, std::unique_ptr<Command> command)
{
	m_commands[key] = std::move(command);
}

void dae::InputManager::RegisterControllerCommand(WORD button, std::unique_ptr<Command> command)
{
	m_controllerCommands[button] = std::move(command);
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

	ProcessControllerInput();

	return true;
}

void dae::InputManager::ProcessControllerInput()
{
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Get the state of the controller from XInput
		dwResult = XInputGetState(i, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
			std::cout << "Controller " << i << " connected" << std::endl;
			std::cout << "Buttons pressed: " << state.Gamepad.wButtons << std::endl;
			for (const auto& [button, command] : m_controllerCommands)
			{
				if (state.Gamepad.wButtons & button)
				{
					std::cout << "Button pressed: " << button << std::endl;
					command->Execute();
				}
				else
				{
					command->Stop();
				}
			}
		}
		else
		{
			// Controller is not connected
			//std::cout << "Controller " << i << " not connected, error code: " << dwResult << std::endl;
		}
	}
}
