/*
 * DF_PlayerMini.h
 *
 *  Created on: 2021Äê10ÔÂ14ÈÕ
 *      Author: h13
 */

#ifndef DRIVERS_DF_PLAYERMINI_DF_PLAYERMINI_H_
#define DRIVERS_DF_PLAYERMINI_DF_PLAYERMINI_H_

#include "RobotConfigs.h"
#include <stdint.h>
#include "GPIO.h"
#include "USART.h"

void DF_PlayerVoiceQueueTimerHandler(void);

typedef uint16_t VoiceID_t;

typedef struct
{
	VoiceID_t VoiceQueue[VoiceQueueSize];
	/*********** USART Port ************/
	USART_t USART_Port;
	/************ Busy GPIO ************/
	GPIO_t BusyIO;
	/******** Private variables ********/
	uint8_t _playerID;
	uint8_t _latestVoicePositionInQueue;
} DF_PlayerMini_t;

typedef enum
{
	DF_PlayerOK = 0,
	DF_PlayerBufferIsFull = 1,
} DF_PlayerError_t;

DF_PlayerError_t DF_PlayerInit(DF_PlayerMini_t *Player);

DF_PlayerError_t DF_PlayerJoinQueue(DF_PlayerMini_t *Player, VoiceID_t VoiceID);

DF_PlayerError_t DF_PlayerReset(DF_PlayerMini_t *Player);

#endif /* DRIVERS_DF_PLAYERMINI_DF_PLAYERMINI_H_ */
