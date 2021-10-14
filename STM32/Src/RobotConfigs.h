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
/**
 * @group: Motion control configs.
 * @param:
 * 		angleAccuracy:   Angle accuracy limit in angle.
 * 		forwardAccuracy: Forward accuracy limit in millimeter.
 * 		stableTimeLimit: Stable Time Limit in milliseconds.
 */
#define MaxSpeed                                     (2500)  //
//DeadZone
#define ForwardDeadZone                              (0.275)
#define BackwardDeadZone                             (0.275)
//Accurary
#define AngleAccurary                                (2.0)   //degree
#define AngleAdjustTimeLimit                         (3000)  //milliseconds
#define AngleStableTimeThreshold                     (500)   //milliseconds
#define ForwardAccuracy                              (20)    //millimeters
#define ForwardAdjustTimeLimit                       (15000)
#define ForwardStableTimeThreshold                   (500)
//PID
//Speed PID
#define SpeedPID_Proportion                          (0.00000123)
#define SpeedPID_Integration                         (0.0000005159)
#define SpeedPID_Differention                        (0.0)

/************************Arm Control************************/
#define GrabHeight                                   (245)                 //millimeters
#define ApproachHeight                               (GrabHeight + 75)     //millimeters
#define RetractionHeight                             (ApproachHeight + 50) //millimeters
#define BaseAxialLength                              (110)                 //millimeters
#define ElongationDistance                           (55)                  //millimeters
#define WashAppleHeight                              (330)                 //millimeters
#define WashAppleAxialLength                         (245)                 //millimeters
#define ThrowAppleAxialLength                        (0)
#define ThrowAppleHeight                             (0)
#define AppleAimCenterX                              (0.5)
#define AppleAimCenterY                              (0.8)
#define TargetAimCenterX                             (0.5)
#define TargetAimCenterY                             (0.5)
#define AimToleranceErrorX                           (0.1)
#define AimToleranceErrorY                           (0.1)
#define ClawStableTimesLimit                         (90)

/*************************Arm Config************************/
//List of available arm types.
#define MechanicalArm                                0
#define LiftingPlatform                              1

//Arm type selection.
#define ArmType                                      LiftingPlatform
#define MaximumRotationAngleOfGraspingApple          (107)

#if(ArmType == MechanicalArm)
//Arm size configs.
#define ArmNode0_Height                              (121)
#define ArmNode0_RotationRange                       (180)
#define ArmNode1_Length                              (242)
#define ArmNode2_Length                              (105)
#define ArmNode3_Length                              (ArmNode1_Length - ArmNode2_Length)
#define MaximumAxialLength                           (411)

#elif(ArmType == LiftingPlatform)
//Arm size configs.
/**
 * @param:
 * 	Z_AxisZeroingStep:
 * 		Z-axis zeroing method. This parameter determines the number of steps to zero each time.
 * 		When it is negative, it means zero down.
 * 		example:
 * 			Z_AxisZeroingStep == 1 means: When the z-axis looks for zero point, one step up at a time.
 * 			Z_AxisZeroingStep == -3 means: When the z-axis looks for zero point, three steps down at a time.
 *
 * 	Z_AxisZeroingLogicExpression:
 *
 */
#define ArmNode2_Length                              (140)
#define ArmNode3_Length                              (140)

#define Z_AxisMinimumHeight                          (320)
#define Z_AxisMaximumHeight                          (445)
#define Z_AxisZeroPoint                              Z_AxisMaximumHeight
#define Z_AxisZeroingStep                            (1)
#define Z_AxisZeroingLogicExpression                 (1)
#define Z_AxisZeroingDelayPerStep                    (10)

#define AL_AxisMinimumLength                         (80)
#define AL_AxisMaximumLength                         (215)
#define AL_AxisZeroPoint                             AL_AxisMinimumLength
#define AL_AxisZeroingLogicExpression                (1)
#define AL_AxisZeroingDelayPerStep                   (10)

#endif //ArmType

/**************Image Processing Module Configs**************/
#define AppleDetectionAverageFPS                     (18)
#define MaximumFPS_Fluctuation                       (1.5)
#define ImageProcessingSerialBufferLength            (32)

/*********************Bluetooth Configs********************/
#define BluetoothTargetNonSignificantAddress         (0x2020)
#define BluetoothTargetUpperAddress                  (0x04)
#define BluetoothTargetLowerAddress                  (0x030495)
#define SendBluetoothMessageContent()                BluetoothPrintf("a")

/**
 * @group: Module configs
 */
/**********************H-Bridge Config**********************/
/**
 * @configs:Channel reverse settings.
 * @param:
 * 		ReverseLeftRight: Switch left and right channels (default: channel 1 -> Left, channel 2 -> Right).
 * 		ReverseChannel1:  Reverse H-bridge channel 1.
 * 		ReverseChannel2:  Reverse H-bridge channel 2.
 */
#define ReverseLeftRight                             0
#define ReverseChannel1                              0
#define ReverseChannel2                              1

/**
 * @platform configs.
 * @note:
 * 		The frequency and resolution of the two PWM channels of the motor must be consistent.
 */
#define PWM_ChannelD0                                 { .Frequency = 1000, .TIMx = TIM3, .Channel = LL_TIM_CHANNEL_CH1, .ReloadValue = 1000 }
#define PWM_ChannelD1                                 { .Frequency = 1000, .TIMx = TIM3, .Channel = LL_TIM_CHANNEL_CH2, .ReloadValue = 1000 }
#define PWM_ChannelD2                                 { .Frequency = 1000, .TIMx = TIM3, .Channel = LL_TIM_CHANNEL_CH3, .ReloadValue = 1000 }
#define PWM_ChannelD3                                 { .Frequency = 1000, .TIMx = TIM3, .Channel = LL_TIM_CHANNEL_CH4, .ReloadValue = 1000 }

/***********************Servos Config***********************/
//**When the servo needs to be installed in reverse, please set `Proportion` to - 1.0 and `Offset` to 180.**
/**
 * @group:  Configs of mechanical arm servos.
 * @servos:
 * 		ArmRotationServo:   The servo that rotates arm.
 *		ArmElongationServo: The servo that elongate arm.
 *		ClawParallelServo:  The servo that makes the claws parallel to the ground.
 *		ClawRotationServo:  The servo that rotates the claw.
 *		ClawGrabServo:      The servo that closes the claw.
 * @note:
 * 		**The picture of servos definition is './Images/MachineArmNodesDefinition.jpg' and './Images/LiftingPlatformNodesDefinition.jpg'.**
 */

#if(ArmType == MechanicalArm)
/**
 * @platform configs.
 */
#define ArmRotationServo                             { .Frequency = 50, .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH1, .ReloadValue = 2000 }
#define ArmElongationServo0                          { .Frequency = 50, .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH2, .ReloadValue = 2000 }
#define ArmElongationServo1                          { .Frequency = 50, .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH3, .ReloadValue = 2000 }
#define ClawParallelServo                            { .Frequency = 50, .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH4, .ReloadValue = 2000 }
#define ClawRotationServo                            { .Frequency = 50, .TIMx = TIM2, .Channel = LL_TIM_CHANNEL_CH1, .ReloadValue = 2000 }
#define ClawGrabServo                                { .Frequency = 50, .TIMx = TIM2, .Channel = LL_TIM_CHANNEL_CH2, .ReloadValue = 2000 }

#define ArmRotationServoMaximumRotationAngle         (180)
#define ArmRotationServoProportion                   (0.867)
#define ArmRotationServoOffset                       (0.0)

#define ArmElongationServo0_MaximumRotationAngle     (180)
#define ArmElongationServo0_Proportion               (0.857)
#define ArmElongationServo0_Offset                   (0.0)

#define ArmElongationServo1_MinimumRotationAngle     (180)
#define ArmElongationServo1_MaximumRotationAngle     (180)
#define ArmElongationServo1_Proportion               (-1.0)
#define ArmElongationServo1_Offset                   (180.0)

#define ClawParallelServoMaximumRotationAngle        (180)
#define ClawParallelServoProportion                  (1.0)
#define ClawParallelServoOffset                      (13.0)

#define ClawGrabServoMaximumRotationAngle            (180)
#define ClawGrabServoProportion                      (1.0)
#define ClawGrabServoOffset                          (0.0)

#elif(ArmType == LiftingPlatform)
/**
 * @platform configs.
 */
#define ArmRotationServo                             { .Frequency = 50, .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH1, .ReloadValue = 2000 }
#define ArmElongationServo                           { .Frequency = 50, .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH2, .ReloadValue = 2000 }
#define ArmParallelServo                             { .Frequency = 50, .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH3, .ReloadValue = 2000 }
#define ClawRotationServo                            { .Frequency = 50, .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH4, .ReloadValue = 2000 }
#define ClawGrabServo                                { .Frequency = 50, .TIMx = TIM5, .Channel = LL_TIM_CHANNEL_CH3, .ReloadValue = 2000 }

#define ArmRotationServoMaximumRotationAngle         (270)
#define ArmRotationServoProportion                   (0.867)
#define ArmRotationServoOffset                       (0.0)

#define ArmElongationServoMaximumRotationAngle       (180)
#define ArmElongationServoProportion                 (0.857)
#define ArmElongationServoOffset                     (5.0)

#define ArmParallelServoMaximumRotationAngle         (180)
#define ArmParallelServoProportion                   (1.0)
#define ArmParallelServoOffset                       (6.0)

#define ClawRotationServoOffset                      (10.0)
#endif


/**
 * @group: Driver configs
 */
/******************Direction Sensor Config******************/
//List of available models.
#define WT101_InSerialMode                           0
#define WT101_InI2C_Mode                             1
//#define MPU9250_InI2C_Mode                         2      //Not recommended.

//Model selection.
#define DirectionSensorModel                         WT101_InI2C_Mode

#if(DirectionSensorModel == WT101_InSerialMode)
//UART Configs.
#define WT101_Ports                                  USART2
#else
//I2C Configs.
//I2C address please use 7-bit address.
#define WT101_I2C_Address                            (0x50)
#endif

/*********************TOF Sensor Config*********************/
//List of available models.
#define SK_Z_TOF                                     0
#define TF_LunaInSerialMode                          1
#define TF_LunaInI2C_Mode                            2
#define VL6180X_InI2C_Mode                           3

//Model selection.
#define MainTofSensorModel                           SK_Z_TOF
#define LeftTofSensorModel                           TF_LunaInI2C_Mode
#define RightTofSensorModel                          TF_LunaInI2C_Mode
#define ClawSuspendedTofModel                        VL6180X_InI2C_Mode

/**********************Stepper Config**********************/
#if(ArmType == LiftingPlatform)
/**
 * @platform configs.
 */
#define StepperNumbers                               (2)
#define Z_AxisStepperStepPort                        GPIOB
#define Z_AxisStepperStepPin                         LL_GPIO_PIN_12
#define Z_AxisStepperDirPort                         GPIOB
#define Z_AxisStepperDirPin                          LL_GPIO_PIN_13
#define AL_AxisStepperStepPort                       GPIOB
#define AL_AxisStepperStepPin                        LL_GPIO_PIN_14
#define AL_AxisStepperDirPort                        GPIOB
#define AL_AxisStepperDirPin                         LL_GPIO_PIN_15

#define Z_AxisStepper                                { .StepperIO = (GPIO_t){ .Port = Z_AxisStepperStepPort, .Pin = Z_AxisStepperStepPin }, .DirectionIO = (GPIO_t){ .Port = Z_AxisStepperDirPort, .Pin = Z_AxisStepperDirPin}, .CurrentSteps = 0, .TargetSteps = 0 }
#define AL_AxisStepper                               { .StepperIO = (GPIO_t){ .Port = AL_AxisStepperStepPort, .Pin = AL_AxisStepperStepPin }, .DirectionIO = (GPIO_t){ .Port = AL_AxisStepperDirPort, .Pin = AL_AxisStepperDirPin}, .CurrentSteps = 0, .TargetSteps = 0 }

#define Z_AxisStepsPerMillimeter                     (25)
#define AL_AxisStepsPerMillimeter                    (6.25)

#endif

/**********************Encoder Config**********************/
#define LeftEncoder                                  { .TimPort = TIM8, .MaximumSpeed = 32678 }
#define RightEncoder                                 { .TimPort = TIM2, .MaximumSpeed = 32678 }


/*******************Voice Module Config********************/
#define VoicePlayerBusyPort                          GPIOC
#define VoicePlayerBusyPin                           LL_GPIO_PIN_2
#define VoiceDeviceNumber                            (1)
#define VoiceQueueSize                               (32)
#define VoicePlayer                                  { .VoiceQueue = { 0 }, .USART_Port = UART5, .BusyIO = (GPIO_t){ .Port = VoicePlayerBusyPort, .Pin = VoicePlayerBusyPin } }

/**
 * @group: Platform Configs
 */
/************************GPIO Configs***********************/
#define GPIO_Ports_t                                 GPIO_TypeDef
#define GPIO_Pin_t                                   uint32_t

#define BT05_Port                                    GPIOC
#define BT05_Pin                                     LL_GPIO_PIN_3

/************************PWM Configs************************/
#define TIM_Ports_t                                  TIM_TypeDef
#define TIM_Channel_t                                uint32_t


/************************I2C Configs************************/
#define I2C_Port                                     hi2c1
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
#define USART_t                                      USART_TypeDef*
#define DebugPorts                                   USART1
#define ImageProcessingModulePorts                   UART4
#define LuatPorts                                    UART5
#define BluetoothPorts                               UART5

#endif /* ROBOTCONFIGS_H_ */
