// InputManager.cpp
#define WIN32_LEAN_AND_MEAN
#include "InputManager.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream> // For debug output
#include <windows.h>
#include <XInput.h>

namespace dae
{
	// Private implementation class
	class InputManagerImpl
	{
	public:
		void RegisterKeyCommand(SDL_Keycode key, std::unique_ptr<Command> command)
		{
			m_Commands[key] = std::move(command);
		}

		void RegisterControllerCommand(int controllerId, ControllerButton button, std::unique_ptr<Command> command)
		{
			m_ControllerCommands[controllerId][static_cast<WORD>(button)] = std::move(command);
		}

		void UnregisterKeyCommand(SDL_Keycode key)
		{
			m_Commands.erase(key);
		}

		void ProcessKeyInput(const SDL_Event& e)
		{
			if (e.type == SDL_KEYDOWN)
			{
				auto it = m_Commands.find(e.key.keysym.sym);
				if (it != m_Commands.end())
				{
					it->second->Execute();
				}
			}
			else if (e.type == SDL_KEYUP)
			{
				auto it = m_Commands.find(e.key.keysym.sym);
				if (it != m_Commands.end())
				{
					it->second->Stop();
				}
			}
		}

		void ProcessControllerInput()
		{
			DWORD dwResult;
			for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) // Loop through all possible controllers
			{
				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));

				// Get the state of the controller from XInput
				dwResult = XInputGetState(i, &state);

				if (dwResult == ERROR_SUCCESS)
				{
					// Process commands for this controller
					auto it = m_ControllerCommands.find(i);
					if (it != m_ControllerCommands.end())
					{
						for (const auto& [button, command] : it->second)
						{
							if (state.Gamepad.wButtons & button)
							{
								command->Execute();
							}
							else
							{
								command->Stop();
							}
						}
					}
				}
			}
		}

	private:
		std::unordered_map<SDL_Keycode, std::unique_ptr<Command>> m_Commands; // Key commands
		std::unordered_map<int, std::unordered_map<WORD, std::unique_ptr<Command>>> m_ControllerCommands; // Controller commands by controller ID
	};


	// InputManager implementation
	InputManager::InputManager()
		: m_pImpl(std::make_unique<InputManagerImpl>())
	{
	}

	InputManager::~InputManager() = default;

	void InputManager::RegisterKeyCommand(SDL_Keycode key, std::unique_ptr<Command> command)
	{
		m_pImpl->RegisterKeyCommand(key, std::move(command)); // Delegate to the implementation
	}

	void InputManager::RegisterControllerCommand(int controllerId, ControllerButton button, std::unique_ptr<Command> command)
	{
		m_pImpl->RegisterControllerCommand(controllerId, button, std::move(command)); // Delegate to the implementation
	}

	void InputManager::UnregisterCommand(SDL_Keycode key)
	{
		m_pImpl->UnregisterKeyCommand(key); // Delegate to the implementation
	}

	bool InputManager::ProcessInput()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return false;
			}

			// Delegate key input processing to the implementation
			m_pImpl->ProcessKeyInput(e);

			ImGui_ImplSDL2_ProcessEvent(&e);
		}

		// Delegate controller input processing to the implementation
		m_pImpl->ProcessControllerInput();

		return true;
	}
}
