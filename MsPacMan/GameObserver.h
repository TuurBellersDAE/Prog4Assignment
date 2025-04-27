#pragma once
#include "Observer.h"

namespace dae
{

class HealthObserver final : public Observer
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
};

};


