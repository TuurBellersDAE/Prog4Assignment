#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

namespace dae
{
	enum class Event
	{
		PLAYER_HIT,
		PLAYER_DIED,
		PLAYER_SCORED
	};

	class GameObject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const GameObject& gameObject, const Event& event) = 0;

	protected:
		Observer() = default;

	private:
		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) = delete;

	};

	class Subject {
	public:
		void AddObserver(Observer* observer) 
		{
			m_Observers.push_back(observer);
		}

		void RemoveObserver(Observer* observer) 
		{
			m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
		}

		void Notify(const GameObject& gameObject, const Event& event)
		{
			for (Observer* observer : m_Observers) 
			{
				observer->OnNotify(gameObject, event);
			}
		}

	private:
		std::vector<Observer*> m_Observers;
	};

#pragma region Observers
	/*class HealthObserver final : public Observer
	{
	public:
		virtual ~HealthObserver() = default;

		void OnNotify(const GameObject& gameObject, const Event& event) override;

	};

	class ScoreObserver final : public Observer
	{
	public:
		virtual ~ScoreObserver() = default;

		void OnNotify(const GameObject& gameObject, const Event& event) override;
	};*/
#pragma endregion
}

