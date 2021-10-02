#pragma once
#include "yaml-cpp/yaml.h"
#include "BaseConfigs.hpp"
#include "HSV_Filter.hpp"

#define __USE_DETECTOR_CONFIGS__

namespace RoboCup
{
	class DetectorConfigs : public BaseConfigs
	{
	public:
		DetectorConfigs(void) = default;
		DetectorConfigs(const YAML::Node& Node) { };
		~DetectorConfigs() { };

		std::vector<h13::HSV_Threshold_t> GetPositiveThresholds(void) const noexcept
		{
			return positiveThresholds;
		};

		std::vector<h13::HSV_Threshold_t> GetNegativeThresholds(void) const noexcept
		{
			return negativeThresholds;
		};

		std::vector<h13::HSV_Threshold_t> GetReflectiveThresholds(void) const noexcept
		{
			return reflectiveThresholds;
		};

	protected:
		void GetThresholds(const YAML::Node& Node, std::vector<h13::HSV_Threshold_t>& PositiveThresholds, std::vector<h13::HSV_Threshold_t>& NegativeThresholds, std::vector<h13::HSV_Threshold_t>& ReflectiveThresholds);

		std::vector<h13::HSV_Threshold_t> positiveThresholds;
		std::vector<h13::HSV_Threshold_t> negativeThresholds;
		std::vector<h13::HSV_Threshold_t> reflectiveThresholds;
	};
}
