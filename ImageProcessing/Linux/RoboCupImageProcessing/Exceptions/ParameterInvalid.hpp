#pragma once
#include <stdexcept>
namespace RoboCup
{
	class ParameterInvalid : public std::runtime_error
	{
	public:
		ParameterInvalid(const std::string& ErrorMessage) :
			std::runtime_error(ErrorMessage) { };
	};
}
