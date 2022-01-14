#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class PearDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		PearDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(40, 61, 110, 255, 30, 135), h13::HSV_Threshold(50, 66, 150, 255, 40, 190) };
			negativeThresholds = {  };
			reflectiveThresholds = {  };
			minimumSize = 1000;
		};

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		PearDetectorConfigs(const YAML::Node& Node) noexcept : PearDetectorConfigs()
		{
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, minimumSize);
			erodeKernelSize = GetIntegerValue(Node["ErodeKernelSize"], erodeKernelSize);
			minimumLengthWidthRatio = GetFloatValue(Node["MinimumLengthWidthRatio"], minimumLengthWidthRatio);
		};

		~PearDetectorConfigs() noexcept { };

		int GetErodeKernelSize(void) const noexcept
		{
			return erodeKernelSize;
		}

		float GetMinimumLengthWidthRatio(void) const noexcept
		{
			return minimumLengthWidthRatio;
		}

	private:
		int erodeKernelSize = 5;
		float minimumLengthWidthRatio = 0.5;
	};
}
