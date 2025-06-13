#pragma once

namespace dae
{
	class FSMBase
	{
	public:
		virtual ~FSMBase() = default;

		virtual void Update() = 0;
		virtual void Render() const = 0;

		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;

	protected:
	};
}

