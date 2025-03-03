#include <SDL.h>
#include "InputManager.h"


#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"   //Don`t know why i cant just #include "imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"      //Don`t know why i cant just #include "imgui_impl_sdl2.h


bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) 
		{
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}
