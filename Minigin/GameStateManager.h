#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "FSMBase.h"

namespace dae
{
	class GameStateManager
	{
	public:
		static GameStateManager& GetInstance()
		{
			static GameStateManager instance;
			return instance;
		}

		void RegisterState(const std::string& name, std::unique_ptr<FSMBase> state)
		{
			m_States[name] = std::move(state);
		}

		void SetState(const std::string& name)
		{
			if (m_CurrentState)
				m_CurrentState->OnExit();

			auto it = m_States.find(name);
			if (it != m_States.end())
			{
				m_CurrentState = it->second.get();
				m_CurrentState->OnEnter();
			}
		}

		void Update()
		{
			if (m_CurrentState)
				m_CurrentState->Update();
		}

		void Render() const
		{
			if (m_CurrentState)
				m_CurrentState->Render();
		}

		FSMBase* GetCurrentState() const { return m_CurrentState; }

	private:
		GameStateManager() = default;
		std::unordered_map<std::string, std::unique_ptr<FSMBase>> m_States;
		FSMBase* m_CurrentState{ nullptr };
	};
}
