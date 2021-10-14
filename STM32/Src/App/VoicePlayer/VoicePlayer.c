/*
 * VoicePlayer.c
 *
 *  Created on: 2021Äê10ÔÂ14ÈÕ
 *      Author: h13
 */
#include "RobotConfigs.h"
#include "VoicePlayer.h"
#include "DF_PlayerMini.h"

static DF_PlayerMini_t player = VoicePlayer;

void VoicePlayerInit(void)
{
	DF_PlayerInit(&player);
}

void VoicePlayerReset(void)
{

}

void VoicePlayerPlayByID(Voice_t Voice)
{

}
