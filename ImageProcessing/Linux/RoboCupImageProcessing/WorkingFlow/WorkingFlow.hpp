#pragma once
#include "Configs.hpp"

namespace RoboCup
{
	class WorkingFlow
	{
	public:
		WorkingFlow(const Configs &Configs) : configs(Configs) { };
		~WorkingFlow() { };

		virtual void Run(void) = 0;

	protected:
		Configs configs;
	};
}