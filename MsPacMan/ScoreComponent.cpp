#include "ScoreComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject* pOwner)
	: Component(pOwner)
	, m_Score(0)
	, m_TextComponent(nullptr)
{
}

void dae::ScoreComponent::BindTextComponent(TextComponent* textComponent)
{
	m_TextComponent = textComponent;
	if (m_TextComponent)
	{
		std::string scoreText = "Score: " + std::to_string(m_Score);
		m_TextComponent->SetText(scoreText);
	}
}

dae::TextComponent* dae::ScoreComponent::GetTextComponent() const
{
	return m_TextComponent;
}

int dae::ScoreComponent::GetScore() const
{
	return m_Score;
}

void dae::ScoreComponent::AddScore(int score)
{
	m_Score += score;
	m_Subject.Notify(*GetOwner(), Event::PLAYER_SCORED);
}

void dae::ScoreComponent::SetScore(int score)
{
	m_Score = score;
}
