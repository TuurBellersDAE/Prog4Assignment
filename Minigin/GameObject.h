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
		GameObject() = default;
		GameObject(GameObject* pParent);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update();
		void Render() const;

		void SetDestroyed(bool isDestroyed) { m_IsDestroyed = isDestroyed; }
		void SetPositionDirty(bool isDirty) { m_IsPositionDirty = isDirty; }

		bool IsDestroyed() const { return m_IsDestroyed; }

#pragma region Position Functions
		void SetWorldPosition(float x, float y, float z = 0.f);
		void SetWorldPosition(const glm::vec3& position);

		void SetLocalPosition(float x, float y, float z = 0.f);
		void SetLocalPosition(const glm::vec3& position);

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const;

#pragma endregion

#pragma region Template Implementations
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template <typename T>
		void RemoveComponent();

		template <typename T>
		T* GetComponentPtr() const;

		template <typename T>
		bool HasComponent() const;
#pragma endregion

#pragma region ParentChild Functions
		void SetParent(GameObject* pParent, bool worldPositionStays = true);

		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);

		bool IsChild(const GameObject* pChild) const;

		GameObject* GetParentPtr() const { return m_pParent; }
		GameObject* GetChildPtr(unsigned int index) const { return m_Children[index]; }
		unsigned int GetChildCount() const { return static_cast<unsigned int>(m_Children.size()); }
#pragma endregion

#pragma region Move Functions
		void UpdateMovement();

		void MoveLeft();
		void MoveRight();
		void MoveUp();
		void MoveDown();

		void SetSpeed(float speed);
		void SetDirection(const glm::vec3& direction);
		void SetIsMoving(bool isMoving);

		glm::vec3 GetDirection() const;
		float GetSpeed() const;
		bool IsMoving() const;
#pragma endregion

	private:
		Transform m_WorldTransform{};
		Transform m_LocalTransform{};

		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;

		GameObject* m_pParent = nullptr;
		std::vector<GameObject*> m_Children;

		bool m_IsDestroyed = false;
		bool m_IsPositionDirty = false;

		glm::vec3 m_Direction{ 0.f, 0.f, 0.f };
		glm::vec3 m_CombinedDirection{ 0.f, 0.f, 0.f };
		float m_Speed = 100.f;
		bool m_IsMoving = false;
	};

#pragma region Template Implementations
	template <typename T, typename... Args>
	T& dae::GameObject::AddComponent(Args&&... args)
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
	void dae::GameObject::RemoveComponent()
	{
		auto typeIndex = std::type_index(typeid(T));
		if (m_Components.find(typeIndex) == m_Components.end())
		{
			throw std::runtime_error("Component does not exist.");
		}

		m_Components.erase(typeIndex);
	}

	template <typename T>
	T* dae::GameObject::GetComponentPtr() const
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
	bool dae::GameObject::HasComponent() const
	{
		return m_Components.find(std::type_index(typeid(T))) != m_Components.end();
	}
#pragma endregion
}
