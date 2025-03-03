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
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "../../imgui-plot-master/include/imgui_plot.h"

#include <numeric>

SDL_Window* g_window{};
SDL_GLContext g_context;

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
		//std::cout << "DeltaTime: " << timer.GetDeltaTime() << std::endl;

		doContinue = input.ProcessInput();

		// ThrashTheCacheAssignment::ThrashTheCache();
		DisplayTrashTheCache();

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

void dae::Minigin::DisplayTrashTheCache()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(50, 100), ImGuiCond_Once);
	ImGui::Begin("Exercise 1", NULL, ImGuiWindowFlags_None);
	ImGui::InputInt("# samples", &m_IntSampleSize);

	if (ImGui::Button("Trash the cache"))
	{
		m_IntTrashCasheDataCalculated = false;
		std::vector<int> g(1000000);
		ImGui::Text("Wait for it...");
		TrashTheCache(m_IntSampleSize, g, m_IntTrashCasheData, m_IntTrashCasheDataAverage);

		for (const auto& num : m_IntTrashCasheDataAverage)
		{
			std::wstringstream ws;
			ws << num;
			ws << "\n";
			OutputDebugStringW(ws.str().c_str());
		}
		OutputDebugStringW(L"\n");
		m_IntTrashCasheDataCalculated = true;
	}


	if (m_IntTrashCasheDataCalculated)
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_IntTrashCasheDataAverage.data();
		conf.values.count = static_cast<int>(m_IntTrashCasheDataAverage.size());
		conf.scale.min = 0;
		conf.scale.max = *std::max_element(m_IntTrashCasheDataAverage.begin(), m_IntTrashCasheDataAverage.end());
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%1.f, y=%1.f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color = ImColor(255, 165, 0);

		ImGui::Plot("IntTrashCache Data", conf);
	}

	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(300, 100), ImGuiCond_Once);
	ImGui::Begin("Exercise 2", NULL, ImGuiWindowFlags_None);
	if (m_GameObject3DSampleSize < 1) m_GameObject3DSampleSize = 1;
	ImGui::InputInt("# samples", &m_GameObject3DSampleSize);

	if (ImGui::Button("Trash the cashe with GameObject3D"))
	{
		m_GameObject3DTrashCasheDataCalculated = false;
		std::vector<GameObject3D> g(1000000);
        ImGui::Text("Wait for it...");
		TrashTheCache(m_GameObject3DSampleSize, g, m_GameObject3DTrashCasheData, m_GameObject3DTrashCasheDataAverage);
		for (const auto& num : m_GameObject3DTrashCasheDataAverage)
		{
			std::wstringstream ws;
			ws << num;
			ws << "\n";
			OutputDebugStringW(ws.str().c_str());
		}
		OutputDebugStringW(L"\n");
		m_GameObject3DTrashCasheDataCalculated = true;
	}

	if (m_GameObject3DTrashCasheDataCalculated)
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_GameObject3DTrashCasheDataAverage.data();
		conf.values.count = static_cast<int>(m_GameObject3DTrashCasheDataAverage.size());
		conf.scale.min = 0;
		conf.scale.max = *std::max_element(m_GameObject3DTrashCasheDataAverage.begin(), m_GameObject3DTrashCasheDataAverage.end());
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%1.f, y=%1.f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color = ImColor(0, 165, 0);
		ImGui::Plot("GameObject3DTrashCache Data", conf);
	}

	if (ImGui::Button("Trash the cashe with GameObject3DAlt"))
	{
		m_GameObject3DAltTrashCasheDataCalculated = false;
		std::vector<GameObject3DAlt> g(1000000);
		for (auto& obj : g)
		{
			obj.transform = new TransformTrashTheCache();
		}
		ImGui::Text("Wait for it...");
		TrashTheCache(m_GameObject3DSampleSize, g, m_GameObject3DAltTrashCasheData, m_GameObject3DAltTrashCasheDataAverage);
		for (const auto& num : m_GameObject3DAltTrashCasheDataAverage)
		{
			std::wstringstream ws;
			ws << num;
			ws << "\n";
			OutputDebugStringW(ws.str().c_str());
		}
		OutputDebugStringW(L"\n");
		m_GameObject3DAltTrashCasheDataCalculated = true;

		// Clean up allocated memory
		for (auto& obj : g)
		{
			delete obj.transform;
			obj.transform = nullptr;
		}
	}

	if (m_GameObject3DAltTrashCasheDataCalculated)
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_GameObject3DAltTrashCasheDataAverage.data();
		conf.values.count = static_cast<int>(m_GameObject3DAltTrashCasheDataAverage.size());
		conf.scale.min = 0;
		conf.scale.max = *std::max_element(m_GameObject3DAltTrashCasheDataAverage.begin(), m_GameObject3DAltTrashCasheDataAverage.end());
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%1.f, y=%1.f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color = ImColor(0, 165, 255);
		ImGui::Plot("GameObject3DAltTrashCache Data", conf);
	}

	ImGui::End();

	ImGui::Render();
}

template<typename T>
void dae::Minigin::TrashTheCache(int sampleSize, std::vector<T>& dataset, std::vector<float>& cacheData, std::vector<float>& cacheDataAverage)
{
	cacheData.clear();
	cacheDataAverage.clear();

	

	const size_t bufferSize = dataset.size();
	const int numRuns = sampleSize;

	for (int step = 1; step <= 1024; step *= 2)
	{
		std::vector<long long> timings;

		for (int run = 0; run < numRuns; ++run)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (size_t i = 0; i < bufferSize; i += step)
			{
				if constexpr (std::is_same_v<T, int>)
				{
					dataset[i] *= 2;
				}
				else if constexpr (std::is_same_v<T, GameObject3D>)
				{
					dataset[i].transform.matrix[0] *= 2;
				}
				else if constexpr (std::is_same_v<T, GameObject3DAlt>)
				{
					dataset[i].transform->matrix[0] *= 2;
				}
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			timings.push_back(duration);
		}

		// Remove top and bottom outliers if there are more than 2 runs
		if (numRuns > 2)
		{
			std::sort(timings.begin(), timings.end());
			timings.erase(timings.begin()); 
			timings.pop_back(); 
		}

		// Calculate average time
		long long total = 0;
		for (const auto& time : timings)
		{
			total += time;
		}
		double averageTime = static_cast<double>(total) / timings.size();

		cacheDataAverage.emplace_back(static_cast<float>(averageTime / 100.0));
	}
}

