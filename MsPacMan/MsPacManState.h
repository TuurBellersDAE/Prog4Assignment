#pragma once
#include <iostream>

namespace dae
{
	class SpriteAnimatorComponent;

	class MsPacManAnimationState
	{
	public:
		MsPacManAnimationState(SpriteAnimatorComponent* animator) : m_Animator(animator) {}
		virtual ~MsPacManAnimationState() = default;
		MsPacManAnimationState(const MsPacManAnimationState&) = delete;
		MsPacManAnimationState(MsPacManAnimationState&&) = delete;
		MsPacManAnimationState& operator=(const MsPacManAnimationState&) = delete;
		MsPacManAnimationState& operator=(MsPacManAnimationState&&) = delete;

		virtual void Enter() = 0;
		virtual void Exit() = 0;

	protected:
		SpriteAnimatorComponent* m_Animator; // Reference to the animator component
	};


	class MsPacManIdleState : public MsPacManAnimationState
	{
	public:
		MsPacManIdleState(SpriteAnimatorComponent* animator) : MsPacManAnimationState(animator) {}
		~MsPacManIdleState() override = default;

		void Enter() override;
		void Exit() override;
	};

	class MsPacManMovingLeftState : public MsPacManAnimationState
	{
	public:
		MsPacManMovingLeftState(SpriteAnimatorComponent* animator) : MsPacManAnimationState(animator) {}
		~MsPacManMovingLeftState() override = default;

		void Enter() override;
		void Exit() override;
	};

	class MsPacManMovingRightState : public MsPacManAnimationState
	{
	public:
		MsPacManMovingRightState(SpriteAnimatorComponent* animator) : MsPacManAnimationState(animator) {}
		~MsPacManMovingRightState() override = default;

		void Enter() override;
		void Exit() override;
	};

	class MsPacManMovingUpState : public MsPacManAnimationState
	{
	public:
		MsPacManMovingUpState(SpriteAnimatorComponent* animator) : MsPacManAnimationState(animator) {}
		~MsPacManMovingUpState() override = default;

		void Enter() override;
		void Exit() override;
	};

	class MsPacManMovingDownState : public MsPacManAnimationState
	{
	public:
		MsPacManMovingDownState(SpriteAnimatorComponent* animator) : MsPacManAnimationState(animator) {}
		~MsPacManMovingDownState() override = default;

		void Enter() override;
		void Exit() override;
	};
}

