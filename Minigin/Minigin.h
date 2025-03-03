#pragma once
#include <string>
#include <functional> 
#include "ThrashTheCacheAssignment.h"

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:

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
