/*
 * vl6180x_platform.c

 *      Author: h13
 */

#include "vl6180x_platform.h"
#include "VL6180X_GPIO_Ports.h"

void VL6180x_Reset(VL6180x_Dev_t* dev)
{
	VL6180x_GPIO_Reset(&(dev -> XshutIO));
}

void VL6180x_Shutdown(VL6180x_Dev_t* dev)
{
	VL6180x_GPIO_Shutdown(&(dev -> XshutIO));
}

void VL6180x_StartUp(VL6180x_Dev_t* dev)
{
	VL6180x_GPIO_StartUp(&(dev -> XshutIO));
}
