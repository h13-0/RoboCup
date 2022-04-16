/*
 * VoicePlayer.h
 *
 *  Created on: 2021Äê10ÔÂ14ÈÕ
 *      Author: h13
 */

#ifndef APP_VOICEPLAYER_VOICEPLAYER_H_
#define APP_VOICEPLAYER_VOICEPLAYER_H_

#ifndef COUNT_ARGS
#define COUNT_ARGS(X...) __COUNT_ARGS(, ##X, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __COUNT_ARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _n, X...) _n
#endif

void VoicePlayerInit(void);

void VoicePlayerReset(void);

typedef enum
{
	BroadcastFruitDetectResult = 1,
	BroadcastApple = 2,
	BroadcastBanana = 3,
	BroadcastKiwiFruit = 4,
	BroadcastLemon = 5,
	BroadcastOrange = 6,
	BroadcastPeach = 7,
	BroadcastPear = 8,
	BroadcastPitaya = 9,
	BroadcastSnowPear = 10,
	BroadcastOne = 11,
	BroadcastTwo = 12,
	BroadcastThree = 13,
	BroadcastFour = 14,
	BroadcastFive = 15,
} Voice_t;

void VoicePlayerPlayByID(Voice_t Voice);

void VoicePlayerPlayByIDS(Voice_t *VoicePtr, uint8_t Length);

/**
 * @brief: Play voice in sequence.
 * @param:

 * @usage:

 * @note:
 * 		The number of parameters should be less than 14.
 */
#define VoicePlayerSequentialPlay(...)  __voicePlayerSequentialPlay(COUNT_ARGS(Voice ##__VA_ARGS__), ##__VA_ARGS__)
int __voicePlayerSequentialPlay(uint8_t Length, ...);

#endif /* APP_VOICEPLAYER_VOICEPLAYER_H_ */
