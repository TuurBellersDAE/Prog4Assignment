#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <memory>
#include "Command.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		void RegisterCommand(SDL_Keycode key, std::unique_ptr<Command> command);
		void UnregisterCommand(SDL_Keycode key);
		bool ProcessInput();

	private:

		std::unordered_map<SDL_Keycode, std::unique_ptr<Command>> m_commands;
	};

}
