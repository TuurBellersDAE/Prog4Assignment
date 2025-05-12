#include "MsPacManState.h"
#include "SpriteAnimatorComponent.h"

void dae::MsPacManIdleState::Enter()
{
	m_Animator->Texture("MsPacMan_Idle.png");
}

void dae::MsPacManIdleState::Exit()
{
	// Logic to handle exiting the idle state, if any
}

void dae::MsPacManMovingLeftState::Enter()
{
	m_Animator->Texture("MsPacMan_Left.png");
}

void dae::MsPacManMovingLeftState::Exit()
{
	// Logic to handle exiting the moving left state, if any
}

void dae::MsPacManMovingRightState::Enter()
{
	m_Animator->Texture("MsPacMan_Right.png");
}

void dae::MsPacManMovingRightState::Exit()
{
	// Logic to handle exiting the moving right state, if any
}

void dae::MsPacManMovingUpState::Enter()
{
	m_Animator->Texture("MsPacMan_Up.png");
}

void dae::MsPacManMovingUpState::Exit()
{
	// Logic to handle exiting the moving up state, if any
}

void dae::MsPacManMovingDownState::Enter()
{
	m_Animator->Texture("MsPacMan_Down.png");
}

void dae::MsPacManMovingDownState::Exit()
{
	// Logic to handle exiting the moving down state, if any
}
