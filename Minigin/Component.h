#pragma once
#include "GameObject.h"

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		//Component() = default;
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update() = 0;
		virtual void Render() const = 0;

		bool IsDestroyed() const { return m_IsDestroyed; };

		void SetDestroyed(bool isDestroyed) { m_IsDestroyed = isDestroyed; };

	protected:
		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {}

		GameObject* GetOwner() const { return m_pOwner; }

		bool m_IsDestroyed = false;

	private:
		GameObject* m_pOwner;

	};
}

