#pragma once
#include <string>

namespace dae
{
	class FSMBase {
	public:
		virtual ~FSMBase() = default;
		virtual void Update() = 0;
		virtual void Render() const = 0;
		virtual void OnEnter() {}
		virtual void OnEnter(const std::string& /*previousState*/) { OnEnter(); }
		virtual void OnExit() = 0;
		virtual void Reset() = 0;
	};

}

