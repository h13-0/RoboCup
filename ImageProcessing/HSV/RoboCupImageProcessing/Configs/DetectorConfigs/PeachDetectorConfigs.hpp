#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class PeachDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		PeachDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(0, 18, 40, 190, 135, 255), h13::HSV_Threshold(220, 255, 15, 140, 130, 255) };
			negativeThresholds = { h13::HSV_Threshold(9, 202, 0, 61, 0, 255) };
			reflectiveThresholds = {  };
			minimumSize = 1300;
		};

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		PeachDetectorConfigs(const YAML::Node& Node) noexcept : PeachDetectorConfigs()
		{
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, minimumSize);
			erodeKernelSize = GetIntegerValue(Node["ErodeKernelSize"], erodeKernelSize);
			minimumLengthWidthRatio = GetFloatValue(Node["MinimumLengthWidthRatio"], minimumLengthWidthRatio);
		};

		~PeachDetectorConfigs() noexcept { };

		int GetErodeKernelSize(void) const noexcept
		{
			return erodeKernelSize;
		}

		float GetMinimumLengthWidthRatio(void) const noexcept
		{
			return minimumLengthWidthRatio;
		}

	private:
		int erodeKernelSize = 10;
		float minimumLengthWidthRatio = 0.5;
	};
}
