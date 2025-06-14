#include "SceneManager.h"
#include "Scene.h"
#include <sstream>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <SDL_syswm.h>
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "../../imgui-plot-master/include/imgui_plot.h"

void dae::SceneManager::Update()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Update();
	}
}

void dae::SceneManager::Render()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Render();
	}

	if (m_isIMGUIActive)
	{
		DisplayTrashTheCache();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

std::shared_ptr<dae::Scene> dae::SceneManager::CreateScene(const std::string& name)
{
	// Check for duplicate scene name
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
		[&name](const std::shared_ptr<Scene>& scene)
		{
			return scene->GetName() == name;
		});

	if (it != m_scenes.end())
	{
		// Scene with this name already exists, return it and set as active
		m_pActiveScene = *it;
		return *it;
	}

	// No duplicate found, create new scene
	auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	m_pActiveScene = scene; // Make the new scene active by default
	return scene;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetActiveScene() const
{
	if (!m_pActiveScene)
	{
		throw std::runtime_error("No active scene found.");
	}
	return m_pActiveScene;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetScene(const std::string& name) const
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
		{
			return scene;
		}
	}
	throw std::runtime_error("Scene with name '" + name + "' not found.");
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
		{
			m_pActiveScene = scene;
			return;
		}
	}
	throw std::runtime_error("Scene with name '" + name + "' not found.");
}

void dae::SceneManager::SetActiveScene(Scene* scene)
{
	if (!scene)
	{
		throw std::runtime_error("Cannot set active scene to a null pointer.");
	}
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
		[scene](const std::shared_ptr<Scene>& s) { return s.get() == scene; });
	if (it != m_scenes.end())
	{
		m_pActiveScene = *it;
	}
	else
	{
		throw std::runtime_error("Scene not found in the manager.");
	}
}

void dae::SceneManager::DisplayTrashTheCache()
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
void dae::SceneManager::TrashTheCache(int sampleSize, std::vector<T>& dataset, std::vector<float>& cacheData, std::vector<float>& cacheDataAverage)
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
					dataset[i].ID *= 2;
				}
				else if constexpr (std::is_same_v<T, GameObject3DAlt>)
				{
					dataset[i].ID *= 2;
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


