// InputManager.h
#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <memory>
#include "Command.h"

#include <SDL.h> // Keep SDL dependency for keyboard input

namespace dae
{
	// Enum for controller buttons
	enum class ControllerButton : unsigned int
	{
		ButtonA = 0x1000, // XINPUT_GAMEPAD_A
		ButtonB = 0x2000, // XINPUT_GAMEPAD_B
		ButtonX = 0x4000, // XINPUT_GAMEPAD_X
		ButtonY = 0x8000, // XINPUT_GAMEPAD_Y
		DPadUp = 0x0001,  // XINPUT_GAMEPAD_DPAD_UP
		DPadDown = 0x0002, // XINPUT_GAMEPAD_DPAD_DOWN
		DPadLeft = 0x0004, // XINPUT_GAMEPAD_DPAD_LEFT
		DPadRight = 0x0008, // XINPUT_GAMEPAD_DPAD_RIGHT
		Start = 0x0010,    // XINPUT_GAMEPAD_START
		Back = 0x0020,     // XINPUT_GAMEPAD_BACK
		LeftThumb = 0x0040, // XINPUT_GAMEPAD_LEFT_THUMB
		RightThumb = 0x0080, // XINPUT_GAMEPAD_RIGHT_THUMB
		LeftShoulder = 0x0100, // XINPUT_GAMEPAD_LEFT_SHOULDER
		RightShoulder = 0x0200  // XINPUT_GAMEPAD_RIGHT_SHOULDER
	};

	class InputManagerImpl; // Forward declaration of the implementation class

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		void RegisterKeyCommand(SDL_Keycode key, std::unique_ptr<Command> command); 
		void RegisterControllerCommand(int controllerId, ControllerButton button, std::unique_ptr<Command> command);
		void UnregisterCommand(SDL_Keycode key);
		bool ProcessInput();

	private:
		std::unique_ptr<InputManagerImpl> m_pImpl; // Pointer to the implementation
	};
}
