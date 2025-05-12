#define _CRT_SECURE_NO_WARNINGS
#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"

#include <thread>
#include <iostream>

#include <SDL_syswm.h>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"   //Don`t know why i cant just #include "imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"      //Don`t know why i cant just #include "imgui_impl_sdl2.h
//#include "../../imgui-plot-master/include/imgui_plot.h"

//#include <numeric>

SDL_Window* g_window{};
SDL_GLContext g_context;

//#include <steam_api.h>

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
		printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string& dataPath)
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	g_context = SDL_GL_CreateContext(g_window);
	if (g_context == nullptr)
	{
		throw std::runtime_error(std::string("SDL_GL_CreateContext Error: ") + SDL_GetError());
	}

	int majorVersion, minorVersion;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion);

	std::cout << "OpenGl Version: " << majorVersion << "." << minorVersion << std::endl;

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}


int GetOptimalFPS()
{
	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
	{
		throw std::runtime_error(std::string("SDL_GetCurrentDisplayMode Error: ") + SDL_GetError());
	}

	// Assuming the refresh rate is a good target FPS
	return displayMode.refresh_rate;
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& timer = Timer::GetInstance();

	const int targetFPS = GetOptimalFPS();
	const auto frameDuration = std::chrono::milliseconds(1000 / targetFPS);

	// imgui 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(g_window, g_context);
	ImGui_ImplOpenGL3_Init();

	bool doContinue = true;
	while (doContinue)
	{
		const auto frameStartTime = std::chrono::high_resolution_clock::now();

		timer.Update();

		doContinue = input.ProcessInput();

		//DisplayTrashTheCache();
		
		// Call SteamAPI_RunCallbacks() every frame
		//SteamAPI_RunCallbacks();

		sceneManager.Update();
		renderer.Render();

		const auto frameEndTime = std::chrono::high_resolution_clock::now();
		const auto frameTime = frameEndTime - frameStartTime;

		if (frameTime < frameDuration)
		{
			std::this_thread::sleep_for(frameDuration - frameTime);
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
