#pragma once
#include <Component.h>
#include "GridComponent.h"
#include "Observer.h"

namespace dae
{
	class PowerPelletEatenComponent : public Component, public Observer
	{
	public:
		PowerPelletEatenComponent(GameObject* pOwner);
		~PowerPelletEatenComponent() = default;

		PowerPelletEatenComponent(const PowerPelletEatenComponent& other) = delete;
		PowerPelletEatenComponent(PowerPelletEatenComponent&& other) = delete;
		PowerPelletEatenComponent& operator=(const PowerPelletEatenComponent& other) = delete;
		PowerPelletEatenComponent& operator=(PowerPelletEatenComponent&& other) = delete;

		void OnNotify(const GameObject& gameObject, const Event& event) override;

		Subject m_Subject;
	private:
	};
}

