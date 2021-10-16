#pragma once
#include "yaml-cpp/yaml.h"
#include "DetectorConfigs.hpp"
namespace RoboCup
{
	class BananaDetectorConfigs : public DetectorConfigs
	{
	public:
		/// <summary>
		/// Construct with default parameters.
		/// </summary>
		/// <param name=""></param>
		BananaDetectorConfigs(void) noexcept
		{
			positiveThresholds = { h13::HSV_Threshold(26, 55, 60, 175, 174, 255) };
			negativeThresholds = { };
			reflectiveThresholds = { };
			minimumSize = 2000;
		};

		/// <summary>
		/// Use profile construction.
		/// note: The default construction is used when the configuration file is wrong.
		/// </summary>
		/// <param name="Node">Config node.</param>
		BananaDetectorConfigs(const YAML::Node& Node) noexcept : BananaDetectorConfigs()
		{
			GetParameters(Node, positiveThresholds, negativeThresholds, reflectiveThresholds, minimumSize);
			erodeKernelSize = GetIntegerValue(Node["ErodeKernelSize"], erodeKernelSize);
			dilateKernelSize = GetIntegerValue(Node["DilateKernelSize"], dilateKernelSize);
			maximumLengthWidthRatio = GetFloatValue(Node["MaximumLengthWidthRatio"], maximumLengthWidthRatio);
		};

		int GetErodeKernelSize(void) const noexcept
		{
			return erodeKernelSize;
		}

		int GetDilateKernelSize(void) const noexcept
		{
			return erodeKernelSize;
		}

		float GetMaximumLengthWidthRatio(void) const noexcept
		{
			return maximumLengthWidthRatio;
		}

		~BananaDetectorConfigs() noexcept { };

	private:
		int erodeKernelSize = 5;
		int dilateKernelSize = 7;
		float maximumLengthWidthRatio = 0.5;
	};
}
