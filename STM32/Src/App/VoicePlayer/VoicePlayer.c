/*
 * VoicePlayer.c
 *
 *  Created on: 2021Äê10ÔÂ14ÈÕ
 *      Author: h13
 */
#include "RobotConfigs.h"
#include <stdarg.h>

#include "Ports.h"
#include "VoicePlayer.h"
#include "DF_PlayerMini.h"

static DF_PlayerMini_t player = VoicePlayer;

void VoicePlayerInit(void)
{
	DF_PlayerInit(&player);
	//DF_SetVolume(&player, 29);
}

void VoicePlayerPlayByID(Voice_t Voice)
{
	DF_PlayerJoinQueue(&player, Voice);
}

void VoicePlayerPlayByIDS(Voice_t *VoicePtr, uint8_t Length)
{
	for(uint8_t i = 0; i < Length; i++)
	{
		DF_PlayerJoinQueue(&player, *(VoicePtr + i));
	}
}

int __voicePlayerSequentialPlay(uint8_t Length, ...)
{
	va_list args;
	va_start(args, Length);

	for(int i = 0; i < Length; i++)
	{
		VoicePlayerPlayByID(va_arg(args, int));
	}

	va_end(args);
	return 0;
}
