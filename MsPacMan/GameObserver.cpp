#include "GameObserver.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include <string>
#include <iostream>

void dae::HealthObserver::OnNotify(const GameObject& gameObject, const Event& event)
{
	HealthComponent* healthComponent = gameObject.GetComponentPtr<HealthComponent>();
	TextComponent* textComponent = nullptr;

	if (healthComponent)
	{
		textComponent = healthComponent->GetTextComponent();
		if (textComponent)
		{
			std::string healthText = "Health: " + std::to_string(healthComponent->GetHealth());
			textComponent->SetText(healthText);
		}

		switch (event)
		{
		case Event::PLAYER_HIT:
			std::cout << "HealthObserver: Player hit\n";
			if (textComponent)
			{
				std::string healthText = "Health: " + std::to_string(healthComponent->GetHealth());
				textComponent->SetText(healthText);
			}
			break;
		case Event::PLAYER_DIED:
			std::cout << "HealthObserver: Player died\n";
			break;
		case Event::PLAYER_SCORED:
			std::cout << "HealthObserver: Player scored\n";
			break;
		default:
			break;
		}
	}
}

void dae::ScoreObserver::OnNotify(const GameObject& gameObject, const Event& event)
{
	ScoreComponent* scoreComponent = gameObject.GetComponentPtr<ScoreComponent>();
	TextComponent* textComponent = nullptr;

	if (scoreComponent)
	{
		textComponent = scoreComponent->GetTextComponent();
		if (textComponent)
		{
			std::string scoreText = "Score: " + std::to_string(scoreComponent->GetScore());
			textComponent->SetText(scoreText);
		}

		switch (event)
		{
		case Event::PLAYER_SCORED:
			std::cout << "ScoreObserver: Player scored\n";
			if (textComponent)
			{
				std::string scoreText = "Score: " + std::to_string(scoreComponent->GetScore());
				textComponent->SetText(scoreText);
			}
			break;
		case Event::PLAYER_DIED:
			std::cout << "ScoreObserver: Player died\n";
			break;
		default:
			break;
		}
	}
}