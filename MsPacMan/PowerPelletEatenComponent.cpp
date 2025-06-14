#include "PowerPelletEatenComponent.h"

dae::PowerPelletEatenComponent::PowerPelletEatenComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

void dae::PowerPelletEatenComponent::OnNotify(const GameObject& gameObject, const Event& event)
{
	if (event == Event::GHOST_FRIGHTENED)
	{
		// Notify observers that a power pellet has been eaten
		m_Subject.Notify(gameObject, event);
	}
}
