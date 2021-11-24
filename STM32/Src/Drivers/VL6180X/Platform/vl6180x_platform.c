/*
 * vl6180x_platform.c

 *      Author: h13
 */

#include "vl6180x_platform.h"

void VL6180x_Reset(VL6180x_Dev_t* dev)
{
	GPIO_PullDown(&(dev -> XshutIO));
}

void VL6180x_Shutdown(VL6180x_Dev_t* dev)
{
	GPIO_PullDown(&(dev -> XshutIO));
}

void VL6180x_StartUp(VL6180x_Dev_t* dev)
{
	GPIO_PullUp(&(dev -> XshutIO));
}
