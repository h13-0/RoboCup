#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class PitayaDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		PitayaDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(210, 255, 95, 255, 125, 255) };
			negativeThresholds = {  };
			reflectiveThresholds = {  };
			minimumSize = 3000;
		};

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		PitayaDetectorConfigs(const YAML::Node& Node) noexcept : PitayaDetectorConfigs()
		{
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, minimumSize);
			erodeKernelSize = GetIntegerValue(Node["ErodeKernelSize"], erodeKernelSize);
		};

		~PitayaDetectorConfigs() noexcept { };

		int GetErodeKernelSize(void) const noexcept
		{
			return erodeKernelSize;
		}

	private:
		int erodeKernelSize = 5;
	};
}
