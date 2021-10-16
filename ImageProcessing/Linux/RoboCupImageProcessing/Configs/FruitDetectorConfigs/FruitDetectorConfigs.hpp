#pragma once
#include "yaml-cpp/yaml.h"
#include "BaseConfigs.hpp"
#include "AppleDetectorConfigs.hpp"
#include "BananaDetectorConfigs.hpp"
#include "KiwiFruitDetectorConfigs.hpp"
#include "LemonDetectorConfigs.hpp"
#include "OrangeDetectorConfigs.hpp"
#include "PeachDetectorConfigs.hpp"
#include "PearDetectorConfigs.hpp"
#include "PitayaDetectorConfigs.hpp"
#include "SnowPearDetectorConfigs.hpp"

namespace RoboCup
{
	class FruitDetectorConfigs : public BaseConfigs
	{
	public:
		FruitDetectorConfigs(const YAML::Node& Node) noexcept;
		~FruitDetectorConfigs() noexcept { };

		/// <summary>
		/// Get subdetector configs.
		/// </summary>
		/// <param name=""></param>
		/// <returns>configs.</returns>
		AppleDetectorConfigs GetAppleDetectorConfigs(void) const noexcept
		{
			return appleDetectorConfigs;
		};

		BananaDetectorConfigs GetBananaDetectorConfigs(void) const noexcept
		{
			return bananaDetectorConfigs;
		};

		KiwiFruitDetectorConfigs GetKiwiFruitDetectorConfigs(void) const noexcept
		{
			return kiwiFruitDetectorConfigs;
		};

		LemonDetectorConfigs GetLemonDetectorConfigs(void) const noexcept
		{
			return lemonDetectorConfigs;
		};

		OrangeDetectorConfigs GetOrangeDetectorConfigs(void) const noexcept
		{
			return orangeDetectorConfigs;
		};

		PeachDetectorConfigs GetPeachDetectorConfigs(void) const noexcept
		{
			return peachDetectorConfigs;
		};

		PearDetectorConfigs GetPearDetectorConfigs(void) const noexcept
		{
			return pearDetectorConfigs;
		};

		PitayaDetectorConfigs GetPitayaDetectorConfigs(void) const noexcept
		{
			return pitayaDetectorConfigs;
		};

		SnowPearDetectorConfigs GetSnowPearDetectorConfigs(void) const noexcept
		{
			return snowPearDetectorConfigs;
		};

		bool IsAppleDetectorEnabled(void) const noexcept
		{
			return isAppleDetectorEnabled;
		};
		
		bool IsBananaDetectorEnabled(void) const noexcept
		{
			return isBananaDetectorEnabled;
		};

		bool IsKiwiFruitDetectorEnabled(void) const noexcept
		{
			return isKiwiFruitDetectorEnabled;
		};

		bool IsLemonDetectorEnabled(void) const noexcept
		{
			return isLemonDetectorEnabled;
		};

		bool IsOrangeDetectorEnabled(void) const noexcept
		{
			return isOrangeDetectorEnabled;
		};

		bool IsPeachDetectorEnabled(void) const noexcept
		{
			return isPeachDetectorEnabled;
		};

		bool IsPearDetectorEnabled(void) const noexcept
		{
			return isPeachDetectorEnabled;
		};

		bool IsPitayaDetectorEnabled(void) const noexcept
		{
			return isPitayaDetectorEnabled;
		};

		bool IsSnowPearDetectorEnabled(void) const noexcept
		{
			return isSnowPearDetectorEnabled;
		};

	private:
		/// <summary>
		/// Detector configs.
		/// </summary>
		bool isAppleDetectorEnabled = true;
		int appleDetectorQuantityLimit = 4;
		AppleDetectorConfigs appleDetectorConfigs;

		bool isBananaDetectorEnabled = true;
		int bananaDetectorQuantityLimit = 4;
		BananaDetectorConfigs bananaDetectorConfigs;

		bool isKiwiFruitDetectorEnabled = true;
		int kiwiFruitDetectorQuantityLimit = 4;
		KiwiFruitDetectorConfigs kiwiFruitDetectorConfigs;

		bool isLemonDetectorEnabled = true;
		int lemonDetectorQuantityLimit = 4;
		LemonDetectorConfigs lemonDetectorConfigs;

		bool isOrangeDetectorEnabled = true;
		int orangeDetectorQuantityLimit = 4;
		OrangeDetectorConfigs orangeDetectorConfigs;

		bool isPeachDetectorEnabled = true;
		int peachDetectorQuantityLimit = 4;
		PeachDetectorConfigs peachDetectorConfigs;

		bool isPearDetectorEnabled = true;
		int pearDetectorQuantityLimit = 4;
		PearDetectorConfigs pearDetectorConfigs;

		bool isPitayaDetectorEnabled = true;
		int pitayaDetectorQuantityLimit = 4;
		PitayaDetectorConfigs pitayaDetectorConfigs;

		bool isSnowPearDetectorEnabled = true;
		int snowPearDetectorQuantityLimit = 4;
		SnowPearDetectorConfigs snowPearDetectorConfigs;
	};
}