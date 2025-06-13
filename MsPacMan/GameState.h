#pragma once
#include <memory>
#include <string>
#include "FSMBase.h"

namespace dae
{
	class Scene;
	class GameObject;
	class TextComponent;

	class PlayingState : public FSMBase
	{
	public:
		// Add this constructor:
		explicit PlayingState(const std::string& sceneName)
			: m_SceneName(sceneName)
		{
		}

		void Update() override;
		void Render() const override;

		void OnEnter() override;
		void OnExit() override;

	private:
		std::shared_ptr<Scene> m_Scene;
		const std::string m_SceneName;

		void LoadPlayingAssets();
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

	private:
		void LoadPauseAssets();
	};

	class MenuState : public FSMBase
	{
	public:
		void Update() override;
		void Render() const override;

		void OnEnter() override;
		void OnExit() override;

	private:
		void LoadMenuAssets();
	};
}

