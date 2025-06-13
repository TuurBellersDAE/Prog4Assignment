#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "ThrashTheCacheAssignment.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		std::shared_ptr<Scene> CreateScene(const std::string& name);
		std::shared_ptr<Scene> GetActiveScene() const;
		std::shared_ptr<Scene> GetScene(const std::string& name) const;

		void SetActiveScene(const std::string& name);
		void SetActiveScene(Scene* scene);

		void Update();
		void Render();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_pActiveScene{ nullptr };

		bool m_isIMGUIActive{ false };

		void DisplayTrashTheCache();

		template<typename T>
		void TrashTheCache(int sampleSize, std::vector<T>& dataset, std::vector<float>& cacheData, std::vector<float>& cacheDataAverage);

		std::vector<float> m_IntTrashCasheData{};
		std::vector<float> m_IntTrashCasheDataAverage{};

		std::vector<float> m_GameObject3DTrashCasheData{};
		std::vector<float> m_GameObject3DTrashCasheDataAverage{};

		std::vector<float> m_GameObject3DAltTrashCasheData{};
		std::vector<float> m_GameObject3DAltTrashCasheDataAverage{};

		int m_IntSampleSize = 10;
		int m_GameObject3DSampleSize = 10;

		bool m_IntTrashCasheDataCalculated{};
		bool m_GameObject3DTrashCasheDataCalculated{};
		bool m_GameObject3DAltTrashCasheDataCalculated{};
	};
}

