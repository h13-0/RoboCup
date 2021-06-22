#include "JustFloat.h"
#include "ports.h"

void SendJustFloatFrame(float data[], uint8_t len) {
	uint8_t *hexPtr = (uint8_t*) data;
	for (uint8_t index = 0; index < len * 4; index++) {
		LogSerialSend(*hexPtr);
		hexPtr++;
	}

	LogSerialSend(0x00);
	LogSerialSend(0x00);
	LogSerialSend(0x80);
	LogSerialSend(0x7f);
}
