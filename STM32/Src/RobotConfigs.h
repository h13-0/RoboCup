/*
 * RobotConfigs.h
 *
 *  Created on: 2021Äê8ÔÂ9ÈÕ
 *      Author: h13
 */

#ifndef ROBOTCONFIGS_H_
#define ROBOTCONFIGS_H_

/**
 * @group: App Configs.
 */
/***********************Motion Control**********************/
//DeadZone
#define ForwardDeadZone               (500)
#define BackwardDeadZone              (630)
//Accurary
#define AngleAccurary         (2.0)   //degree
#define AngleAdjustTimeLimit  (3000)  //milliseconds
#define ForwardAccuracy       (20)    //millimeters

//PID



/************************Arm Control************************/
#define GrabHeight            (245)                 //millimeters
#define ApproachHeight        (GrabHeight + 75)     //millimeters
#define RetractionHeight      (ApproachHeight + 50) //millimeters
#define BaseAxialLength       (110)                 //millimeters
#define ElongationDistance    (55)                  //millimeters
#define WashAppleHeight       (330)                 //millimeters
#define WashAppleAxialLength  (245)                 //millimeters

/*************************Arm Config************************/
//List of available arm types.
#define MechanicalArm          0
//#define LiftingPlatform        1

//Arm type selection.
#define ArmType                MechanicalArm

#if(ArmType == MechanicalArm)
//List of available arm control methods.
#define OpenLoopGeometricControl    0
#define ClosedLoopGeometricControl  1

//Arm size configs.
#define ArmNode0_Height        (121)
#define ArmNode0_RotationRange (180)
#define ArmNode1_Length        (242)
#define ArmNode2_Length        (105)
#define ArmNode3_Length        (ArmNode1_Length - ArmNode2_Length)
#define MaximumAxialLength     (411)

//Arm Z-Axis control method selection.
#define ArmZ_AxisControlMethod       OpenLoopGeometricControl

/***********************Servos Config***********************/
#define Node0_ServoMinimumRotationAngle  (0)
#define Node0_ServoMaximumRotationAngle  (180)
#define Node0_ServoProportion            (0.867)
#define Node0_ServoOffset                (0.0)

#define Node1_ServoMinimumRotationAngle  (0)
#define Node1_ServoMaximumRotationAngle  (180)
#define Node1_ServoProportion            (0.857)
#define Node1_ServoOffset                (0.0)

#define Node2_ServoMinimumRotationAngle  (0)
#define Node2_ServoMaximumRotationAngle  (180)
#define Node2_ServoProportion            (1.0)//(0.908386)
#define Node2_ServoOffset                (0.0)//(-1.218346)

#define Node3_ServoMinimumRotationAngle  (-90)
#define Node3_ServoMaximumRotationAngle  (90)
#define Node3_ServoProportion            (1.0)
#define Node3_ServoOffset                (9.0)//(4.4)

#define MaximumRotationAngleOfGraspingApple  (107)

#if(ArmControlMethod == ClosedLoopGeometricControl)

#endif //#if(ArmControlMethod == ClosedLoopGeometricControl)

#elif(ArmType == LiftingPlatform)

#endif //ArmType


/**************Image Processing Module Configs**************/
#define AppleDetectionAverageFPS            (18)
#define MaximumFPS_Fluctuation              (1.5)
#define ImageProcessingSerialBufferLength   (32)
#define AppleTargetCenterX                  (0.5)
#define AppleTargetCenterY                  (0.8)
#define AppleToleranceErrorX                (0.1)
#define AppleToleranceErrorY                (0.1)
#define ClawStableTimesLimit                (90)

/*********************Bluetooth Configs********************/
#define BluetoothTargetNonSignificantAddress         (0x2020)
#define BluetoothTargetUpperAddress                  (0x04)
#define BluetoothTargetLowerAddress                  (0x030495)
#define SendBluetoothMessageContent()                BluetoothPrintf("A")

/**
 * @group: Module configs
 */

/******************Direction Sensor Config******************/
//List of available models.
#define WT101_InSerialMode   0
#define WT101_InI2C_Mode     1
//#define MPU9250_InI2C_Mode   2      //Not recommended.

//Model selection.
#define DirectionSensorModel WT101_InSerialMode

//I2C Configs.
//I2C address please use 7-bit address.
#if(DirectionSensorModel == WT101_InI2C_Mode)
#define WT101_I2C_Address   0x50
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

/**
 * @group: Platform Configs
 */
/************************I2C Configs************************/
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



/***********************USART Configs***********************/
#define DebugPorts                  USART1
#define WT101_Ports                 USART2
#define ImageProcessingModulePorts  UART4
#define LuatPorts                   UART5
#define BluetoothPorts              UART5

/************************GPIO Configs***********************/
#define BT05_Port                   GPIOC
#define BT05_Pin                    LL_GPIO_PIN_3



#endif /* ROBOTCONFIGS_H_ */
