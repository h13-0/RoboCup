/*
 * Luat.h
 *
 *  Created on: 2021年8月18日
 *      Author: h13
 */

#ifndef DRIVERS_LUAT_LUAT_H_
#define DRIVERS_LUAT_LUAT_H_

#ifndef COUNT_ARGS
#define COUNT_ARGS(X...) __COUNT_ARGS(, ##X, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __COUNT_ARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _n, X...) _n
#endif

#include <stdint.h>

typedef struct
{
	uint8_t       Index;
	const uint8_t BufferSize;
	char*         BufferPtr;
} LuatMessage_t;

/**
 * @brief: Create new message for Luat.
 * @usage:
 * 		char messageBuffer[128] = { 0 };
 * 		LuatMessage_t message = LuatCreatNewMessage(messageBuffer, 128);
 */
LuatMessage_t LuatCreatNewMessage(char *BufferPtr, uint8_t BufferSize);

typedef enum
{
	//English characters.
	UnicodeEngOne   = 0x0031,     //English 1

	//Chinese characters.
	UnicodeChsPing  = 0x82f9,     //Chinese 苹
	UnicodeChsGuo   = 0x679c,     //Chinese 果
	UnicodeChsXiang = 0x9999,     //Chinese 香
	UnicodeChsJiao  = 0x8549,     //Chinese 蕉

	UnicodeChsGe    = 0x4e2a,     //Chinese 个
} Unicode_t;

/**
 * @brief: Add one character to message.
 * @param:
 * 		Msg:       LuatMessage_t, The Appended target.
 * 		Character: Unicode_t, The characters you want to add.
 */
int LuatAddMessageContent(LuatMessage_t *Msg, Unicode_t Character);

/**
 * @brief: Add contents to LuatMessage_t.
 * @param:
 * 		Msg: LuatMessage_t, The Appended target.
 * 		...: Unicode_t, List of characters you want to add.
 * @usage:
 * 		LuatAddMessageContents(&message, UnicodeChsPing, UnicodeChsGuo);                              //苹果
 * 		LuatAddMessageContents(&message, UnicodeChsPing, UnicodeChsGuo, UnicodeEngOne, UnicodeChsGe); //苹果1个
 * @note:
 * 		The number of parameters should be less than 14.
 */
#define LuatAddMessageContents(Msg, ...)  __luatAddMessageContents(Msg, COUNT_ARGS(Msg ##__VA_ARGS__), ##__VA_ARGS__)
int __luatAddMessageContents(LuatMessage_t *Msg, uint8_t Length, ...);

/**
 * @brief: Send message to target phone.
 * @param:
 * 		LuatMessage_t *Msg:            The message you want to send.
 * 		const char* TargetPhoneNumber: Target Phone Number.
 * @note:
 * 		The phone number is a string, not an integer.
 * 		Such as 0371-66667777
 */
void LuatSendMessage(LuatMessage_t *Msg, const char* TargetPhoneNumber);

#endif /* DRIVERS_LUAT_LUAT_H_ */
