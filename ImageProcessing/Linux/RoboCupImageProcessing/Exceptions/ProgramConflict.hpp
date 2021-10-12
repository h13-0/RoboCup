#pragma once
#include <stdexcept>
namespace RoboCup
{
	class ProgramConflict : public std::runtime_error
	{
	public:
		ProgramConflict(const std::string& ErrorMessage) :
			std::runtime_error(ErrorMessage) { };
	};
}
