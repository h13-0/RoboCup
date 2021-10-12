#pragma once
#include "Configs.hpp"

namespace RoboCup
{
	class WorkingFlow
	{
	public:
		WorkingFlow(const Configs &Configs) { };
		virtual ~WorkingFlow() { };
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		virtual int Run(void) = 0;
	};
}