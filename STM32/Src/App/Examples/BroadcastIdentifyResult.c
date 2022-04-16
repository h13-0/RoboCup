/*
 * BroadcastIdentifyResult.c
 *
 *  Created on: 2022Äê4ÔÂ8ÈÕ
 *      Author: h13
 */
#include "BroadcastIdentifyResult.h"
#include <stdint.h>
#include "VoicePlayer.h"

static Voice_t convertNumberToVoiceID(uint8_t Number)
{
	switch(Number)
	{
	case 1:
		return BroadcastOne;
	case 2:
		return BroadcastTwo;
	case 3:
		return BroadcastThree;
	case 4:
		return BroadcastFour;
	case 5:
		return BroadcastFive;
	default:
		return BroadcastOne;
	}
}

void BroadcastIdentifyResult(FruitIdentifyResult_t *Result)
{
	VoicePlayerReset();
	//Apple
	if(Result -> AppleNumber > 0)
	{
		VoicePlayerSequentialPlay(BroadcastFruitDetectResult, BroadcastApple, convertNumberToVoiceID(Result -> AppleNumber));
	} else {
		VoicePlayerSequentialPlay(BroadcastFruitDetectResult, BroadcastApple, BroadcastOne);
	}

	//Banana
	if(Result -> BananaNumber > 0)
	{
		VoicePlayerSequentialPlay(BroadcastBanana, convertNumberToVoiceID(Result -> BananaNumber));
	}

	//KiwiFruit
	if(Result -> KiwiFruitNumber > 0)
	{
		VoicePlayerSequentialPlay(BroadcastKiwiFruit, convertNumberToVoiceID(Result -> KiwiFruitNumber));
	}

	//Lemon
	if(Result -> LemonNumber > 0)
	{
		VoicePlayerSequentialPlay(BroadcastLemon, convertNumberToVoiceID(Result -> LemonNumber));
	}

	//Orange
	if(Result -> OrangeNumber > 0)
	{
		VoicePlayerSequentialPlay(BroadcastOrange, convertNumberToVoiceID(Result -> OrangeNumber));
	}

	//Peach
	if(Result -> PeachNumber > 0)
	{
		VoicePlayerSequentialPlay(BroadcastPeach, convertNumberToVoiceID(Result -> PeachNumber));
	}

	//Pear
	if(Result -> PearNumber > 0)
	{
		VoicePlayerSequentialPlay(BroadcastPear, convertNumberToVoiceID(Result -> PearNumber));
	}

	//Pitaya
	if(Result -> PitayaNumber > 0)
	{
		VoicePlayerSequentialPlay(BroadcastPitaya, convertNumberToVoiceID(Result -> PitayaNumber));
	}

	//SnowPear
	if(Result -> SnowPearNumber > 0)
	{
		VoicePlayerSequentialPlay(BroadcastSnowPear, convertNumberToVoiceID(Result -> SnowPearNumber));
	}
}
