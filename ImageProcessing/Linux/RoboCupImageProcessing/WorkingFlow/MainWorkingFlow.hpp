#pragma once
#include "WorkingFlow.hpp"
namespace RoboCup
{
	class MainWorkingFlow : public WorkingFlow
	{
	public:
		MainWorkingFlow(const Configs& Configs) :
		WorkingFlow(Configs){ };
		~MainWorkingFlow() { };

		void Run();

	private:

	};
}