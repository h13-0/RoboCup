/*
 * Luat.h
 *
 *  Created on: 2021Äê8ÔÂ18ÈÕ
 *      Author: h13
 */

#ifndef DRIVERS_LUAT_LUAT_H_
#define DRIVERS_LUAT_LUAT_H_

#include <stdint.h>

typedef struct
{
	uint8_t Index;
	uint8_t BufferSize;
	char*   BufferPtr;
} Message_t;

Message_t LuatCreatNewMessage(char *BufferPtr, uint8_t BufferSize);

int LuatAddMessageContent(Message_t *Msg);

void SendNewMessage(Message_t *Msg);

#endif /* DRIVERS_LUAT_LUAT_H_ */
