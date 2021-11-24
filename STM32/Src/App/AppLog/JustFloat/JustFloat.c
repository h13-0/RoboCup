#include "JustFloat.h"
#include "RobotConfigs.h"
#include "ports.h"

void SendJustFloatFrame(float data[], uint8_t len) {
	uint8_t *hexPtr = (uint8_t*) data;
	for (uint8_t index = 0; index < len * 4; index++) {
		SerialSend(DebugPort, *hexPtr);
		hexPtr++;
	}

	SerialSend(DebugPort, 0x00);
	SerialSend(DebugPort, 0x00);
	SerialSend(DebugPort, 0x80);
	SerialSend(DebugPort, 0x7f);
}
