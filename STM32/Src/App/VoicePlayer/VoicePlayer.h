/*
 * VoicePlayer.h
 *
 *  Created on: 2021Äê10ÔÂ14ÈÕ
 *      Author: h13
 */

#ifndef APP_VOICEPLAYER_VOICEPLAYER_H_
#define APP_VOICEPLAYER_VOICEPLAYER_H_

void VoicePlayerInit(void);

typedef enum
{
	BroadcastFruitDetectResult = 1,
	BroadcastApple = 2,
	BroadcastOne = 3
} Voice_t;

void VoicePlayerPlayByID(Voice_t Voice);

void VoicePlayerPlayByIDS(Voice_t *VoicePtr, uint8_t Length);

#endif /* APP_VOICEPLAYER_VOICEPLAYER_H_ */
