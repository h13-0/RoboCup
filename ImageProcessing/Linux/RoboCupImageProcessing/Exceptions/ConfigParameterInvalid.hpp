#pragma once
#include <stdexcept>
namespace RoboCup
{
	class ConfigParameterInvalid : public std::runtime_error
	{
	public:
		ConfigParameterInvalid(const std::string& ErrorMessage) :
			std::runtime_error(ErrorMessage) { };
	};
}
