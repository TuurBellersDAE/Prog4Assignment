#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <memory>
#include "Command.h"

#include <windows.h>
#include <XInput.h>


namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		void RegisterKeyCommand(SDL_Keycode key, std::unique_ptr<Command> command);
		void RegisterControllerCommand(WORD button, std::unique_ptr<Command> command);
		void UnregisterCommand(SDL_Keycode key);
		bool ProcessInput();

	private:
		void ProcessControllerInput();
		std::unordered_map<SDL_Keycode, std::unique_ptr<Command>> m_commands;
		std::unordered_map<WORD, std::unique_ptr<Command>> m_controllerCommands;
	};

}
