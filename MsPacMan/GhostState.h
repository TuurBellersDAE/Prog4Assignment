#pragma once
#include <FSMBase.h>
#include <glm.hpp>

namespace dae
{
	class GhostAIControllerComponent;
	class GameObject;

	void GhostMoveAndAnimate(
		dae::GhostAIControllerComponent* controller,
		const glm::ivec2& targetCell);

	class GhostState : public FSMBase
	{
	public:
		explicit GhostState(GhostAIControllerComponent* controller) : m_Controller(controller) {}
		virtual ~GhostState() = default;
		GhostState(const GhostState&) = delete;
		GhostState(GhostState&&) = delete;
		GhostState& operator=(const GhostState&) = delete;
		GhostState& operator=(GhostState&&) = delete;

		virtual void Update() override = 0;
		virtual void Render() const override = 0;

		virtual void OnEnter() override = 0;
		virtual void OnExit() override = 0;

		void Reset() override {}
	protected:
		GhostAIControllerComponent* m_Controller;
	};

	class GhostChaseState : public GhostState
	{
	public:
		explicit GhostChaseState(GhostAIControllerComponent* controller) : GhostState(controller) {}
		virtual ~GhostChaseState() override = default;
		void Update() override;
		void Render() const override;
		void OnEnter() override;
		void OnExit() override;
		void Reset() override {}
	};

	class GhostScatterState : public GhostState
	{
	public:
		explicit GhostScatterState(GhostAIControllerComponent* controller) : GhostState(controller) {}
		virtual ~GhostScatterState() override = default;
		void Update() override;
		void Render() const override;
		void OnEnter() override;
		void OnExit() override;
		void Reset() override {}
	};

	class GhostFrightenedState : public GhostState
	{
	public:
		explicit GhostFrightenedState(GhostAIControllerComponent* controller) : GhostState(controller) {}
		virtual ~GhostFrightenedState() override = default;
		void Update() override;
		void Render() const override;
		void OnEnter() override;
		void OnExit() override;
		void Reset() override {}
	};

	class GhostEatenState : public GhostState
	{
	public:
		explicit GhostEatenState(GhostAIControllerComponent* controller) : GhostState(controller) {}
		virtual ~GhostEatenState() override = default;
		void Update() override;
		void Render() const override;
		void OnEnter() override;
		void OnExit() override;
		void Reset() override {}
	};

	class GhostReswpawnState : public GhostState
	{
	public:
		explicit GhostReswpawnState(GhostAIControllerComponent* controller) : GhostState(controller) {}
		virtual ~GhostReswpawnState() override = default;
		void Update() override;
		void Render() const override;
		void OnEnter() override;
		void OnExit() override;
		void Reset() override {}
	};

}

