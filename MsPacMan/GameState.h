#pragma once
#include <memory>
#include <string>
#include "FSMBase.h"
#include "GameObject.h"

namespace dae
{
	class Scene;
	class GameObject;
	class TextComponent;

	class SinglePlayer : public FSMBase
	{
	public:
		explicit SinglePlayer(const std::string& sceneName)
			: m_SceneName(sceneName)
		{
		}

		void Update() override;
		void Render() const override;

		void OnEnter() override;
		void OnEnter(const std::string& previousState) override; 
		void OnExit() override;

		void Reset() override;

	private:
		std::shared_ptr<Scene> m_Scene;
		const std::string m_SceneName;

		std::shared_ptr<dae::GameObject> m_Player;
		std::vector<std::shared_ptr<dae::GameObject>> m_Ghosts;
		std::pair<int, int> m_PlayerSpawnCell{ 14, 23 };
		std::vector<std::pair<int, int>> m_GhostSpawnCells{ {14, 11}, {12, 14}, {14, 14}, {15, 14} };

		void LoadSinglePlayerAssets();
	};

	class MultiPlayer : public FSMBase
	{
	public:
		explicit MultiPlayer(const std::string& sceneName)
			: m_SceneName(sceneName)
		{
		}
		void Update() override;
		void Render() const override;
		void OnEnter() override;
		void OnEnter(const std::string& previousState) override; 
		void OnExit() override;

		void Reset() override;
	private:
		std::shared_ptr<Scene> m_Scene;
		const std::string m_SceneName;

		std::vector<std::shared_ptr<dae::GameObject>> m_Players;
		std::vector<std::shared_ptr<dae::GameObject>> m_Ghosts;
		std::vector<std::pair<int, int>> m_PlayerSpawnCells;
		std::vector<std::pair<int, int>> m_GhostSpawnCells;

		void LoadMultiPlayerAssets();
	};

	class GameOverState : public FSMBase
	{
	public:
		void Update() override;
		void Render() const override;

		void OnEnter() override;
		void OnExit() override;

	private:
		void LoadGameOverAssets();
	};

	class PausedState : public FSMBase
	{
	public:
		void Update() override;
		void Render() const override;

		void OnEnter() override;
		void OnExit() override;

		void Reset() override {}
	private:
		void LoadPauseAssets();
	};

	class MenuState : public FSMBase
	{
	public:
		explicit MenuState(const std::string& sceneName)
			: m_SceneName(sceneName)
		{
		}
		void Update() override;
		void Render() const override;

		void OnEnter() override;
		void OnExit() override;

		void Reset() override {}
	private:
		std::shared_ptr<Scene> m_Scene;
		const std::string m_SceneName;

		void LoadMenuAssets();

		int m_SelectedIndex{ 0 };
		std::vector<std::shared_ptr<GameObject>> m_MenuItems;

		void UpdateMenuVisuals();
	};

}

