#pragma once
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <stdexcept>
#include "Transform.h"

namespace dae
{
	class Component;

	class GameObject final
	{
	public:
		void Update();
		void Render() const;

		void SetPosition(float x, float y);

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template <typename T>
		void RemoveComponent();

		template <typename T>
		T* GetComponent() const;

		template <typename T>
		bool HasComponent() const;

	private:
		Transform m_transform{};
		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;
	};

	template <typename T, typename... Args>
	T& GameObject::AddComponent(Args&&... args)
	{
		auto typeIndex = std::type_index(typeid(T));
		if (m_Components.find(typeIndex) != m_Components.end())
		{
			throw std::runtime_error("Component already exists.");
		}

		auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
		T& ref = *component;
		m_Components[typeIndex] = std::move(component);
		return ref;
	}

	template <typename T>
	void GameObject::RemoveComponent()
	{
		auto typeIndex = std::type_index(typeid(T));
		if (m_Components.find(typeIndex) == m_Components.end())
		{
			throw std::runtime_error("Component does not exist.");
		}

		m_Components.erase(typeIndex);
	}

	template <typename T>
	T* GameObject::GetComponent() const
	{
		auto typeIndex = std::type_index(typeid(T));
		auto it = m_Components.find(typeIndex);
		if (it != m_Components.end())
		{
			return static_cast<T*>(it->second.get());
		}
		return nullptr;
	}

	template <typename T>
	bool GameObject::HasComponent() const
	{
		return m_Components.find(std::type_index(typeid(T))) != m_Components.end();
	}
}
