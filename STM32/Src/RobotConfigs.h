/*
 * RobotConfigs.h
 *
 *  Created on: 2021Äê8ÔÂ9ÈÕ
 *      Author: h13
 */

#ifndef ROBOTCONFIGS_H_
#define ROBOTCONFIGS_H_

/**
 * @group: Module configs
 */

/******************Direction Sensor Config******************/
//List of available models.
#define WT101_InSerialMode   0
#define WT101_InI2C_Mode     1
//#define MPU9250_InI2C_Mode   2      //Not recommended.

//Model selection.
#define DirectionSensorModel WT101_InI2C_Mode

//I2C Configs.
//I2C address please use 7-bit address.
#if(DirectionSensorModel == WT101_InI2C_Mode)
#define WT101_I2C_Address
#endif

/*********************TOF Sensor Config*********************/
//List of available models.
#define SK_Z_TOF             0
#define TF_LunaInSerialMode  1
#define TF_LunaInI2C_Mode    2
#define VL6180X_InI2C_Mode   3

//Model selection.
#define MainTofSensorModel    SK_Z_TOF
#define LeftTofSensorModel    TF_LunaInI2C_Mode
#define RightTofSensorModel   TF_LunaInI2C_Mode
#define ClawSuspendedTofModel VL6180X_InI2C_Mode

//I2C Configs.
//I2C address please use 7-bit address.
#if(MainTofSensorModel == TF_LunaInI2C_Mode) || (MainTofSensorModel == VL6180X_InI2C_Mode)
#define MainTofI2C_Address
#endif

#if(LeftTofSensorModel == TF_LunaInI2C_Mode) || (LeftTofSensorModel == VL6180X_InI2C_Mode)
#define LeftTofI2C_Address
#endif

#if(RightTofSensorModel == TF_LunaInI2C_Mode) || (RightTofSensorModel == VL6180X_InI2C_Mode)
#define RightTofI2C_Address
#endif

#if(ClawSuspendedTofModel == TF_LunaInI2C_Mode) || (ClawSuspendedTofModel == VL6180X_InI2C_Mode)
#define ClawSuspendedTofI2C_Address
#endif

/***********************Servos Config***********************/


#endif /* ROBOTCONFIGS_H_ */
