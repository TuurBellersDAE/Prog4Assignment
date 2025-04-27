#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class ScoreComponent : public Component
	{
	public:
		ScoreComponent(GameObject* pOwner);
		~ScoreComponent() = default;
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		void BindTextComponent(TextComponent* textComponent);
		TextComponent* GetTextComponent() const;

		int GetScore() const;

		void SetScore(int score);
		void AddScore(int score);

		Subject m_Subject;
	private:
		int m_Score{ 0 };
		TextComponent* m_TextComponent;

	};
}

