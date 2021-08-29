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
#define MaxSpeed                      (27.5)
//DeadZone
#define ForwardDeadZone               (500)
#define BackwardDeadZone              (500)
//Accurary
#define AngleAccurary                 (2.0)   //degree
#define AngleAdjustTimeLimit          (3000)  //milliseconds
#define AngleStableTimeThreshold      (500)   //milliseconds
#define ForwardAccuracy               (20)    //millimeters
#define ForwardAdjustTimeLimit        (15000)
#define ForwardStableTimeThreshold    (500)
//PID



/************************Arm Control************************/
#define GrabHeight                      (245)                 //millimeters
#define ApproachHeight                  (GrabHeight + 75)     //millimeters
#define RetractionHeight                (ApproachHeight + 50) //millimeters
#define BaseAxialLength                 (110)                 //millimeters
#define ElongationDistance              (55)                  //millimeters
#define WashAppleHeight                 (330)                 //millimeters
#define WashAppleAxialLength            (245)                 //millimeters
#define AppleAimCenterX                 (0.5)
#define AppleAimCenterY                 (0.8)
#define TargetAimCenterX                (0.5)
#define TargetAimCenterY                (0.5)
#define AimToleranceErrorX              (0.1)
#define AimToleranceErrorY              (0.1)
#define ClawStableTimesLimit            (90)

/*************************Arm Config************************/
//List of available arm types.
#define MechanicalArm          0
#define LiftingPlatform        1

//Arm type selection.
#define ArmType                LiftingPlatform
#define MaximumRotationAngleOfGraspingApple  (107)

#if(ArmType == MechanicalArm)
//Arm size configs.
#define ArmNode0_Height        (121)
#define ArmNode0_RotationRange (180)
#define ArmNode1_Length        (242)
#define ArmNode2_Length        (105)
#define ArmNode3_Length        (ArmNode1_Length - ArmNode2_Length)
#define MaximumAxialLength     (411)

/***********************Servos Config***********************/


#elif(ArmType == LiftingPlatform)
/***********************Servos Config***********************/
#define Node0_ServoMinimumRotationAngle  (0)
#define Node0_ServoMaximumRotationAngle  (270)
#define Node0_ServoProportion            (1.0)
#define Node0_ServoOffset                (0.0)

#define Node1_ServoMinimumRotationAngle  (0)
#define Node1_ServoMaximumRotationAngle  (180)
#define Node1_ServoProportion            (0.857)
#define Node1_ServoOffset                (0.0)

#define Node2_ServoMinimumRotationAngle  (0)
#define Node2_ServoMaximumRotationAngle  (180)
#define Node2_ServoProportion            (1.0)
#define Node2_ServoOffset                (0.0)

#endif //ArmType


/**************Image Processing Module Configs**************/
#define AppleDetectionAverageFPS            (18)
#define MaximumFPS_Fluctuation              (1.5)
#define ImageProcessingSerialBufferLength   (32)

/*********************Bluetooth Configs********************/
#define BluetoothTargetNonSignificantAddress         (0x2020)
#define BluetoothTargetUpperAddress                  (0x04)
#define BluetoothTargetLowerAddress                  (0x030495)
#define SendBluetoothMessageContent()                BluetoothPrintf("A")

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
#define ReverseLeftRight  0
#define ReverseChannel1   0
#define ReverseChannel2   1

#define LeftMotorPWM               { .TIMx = TIM3, .Channel = LL_TIM_CHANNEL_CH1 }
#define LeftMotorIO                { .Port = GPIOC, .Pin = LL_GPIO_PIN_13 }
#define RightMotorPWM              { .TIMx = TIM3, .Channel = LL_TIM_CHANNEL_CH2 }
#define RightMotorIO               { .Port = GPIOC, .Pin = LL_GPIO_PIN_14 }
#define LeftMotorPWM_Frequence     (1000)
#define LeftMotorPWM_ReloadValue   (1000)
#define RightMotorPWM_Frequence    (1000)
#define RightMotorPWM_ReloadValue  (1000)

/***********************Servos Config***********************/
#if(ArmType == MechanicalArm)
#define ArmRotationServo                             { .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH1 }
#define ArmElongationServo0                          { .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH2 }
#define ArmElongationServo1                          { .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH3 }
#define ArmElongationServo2                          { .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH4 }
#define ClawRotationServo                            { .TIMx = TIM2, .Channel = LL_TIM_CHANNEL_CH1 }
#define ClawGrabServo                                { .TIMx = TIM2, .Channel = LL_TIM_CHANNEL_CH2 }

#define ArmRotationServoPWM_Frequence                (50)
#define ArmRotationServoPWM_ReloadValue              (1999)

#define ArmElongationServo0_PWM_Frequence            (50)
#define ArmElongationServo0_PWM_ReloadValue          (1999)

#define ArmElongationServo1_PWM_Frequence            (50)
#define ArmElongationServo1_PWM_ReloadValue          (1999)

#define ArmElongationServo2_PWM_Frequence            (50)
#define ArmElongationServo2_PWM_ReloadValue          (1999)

#define ClawRotationServoPWM_Frequence               (50)
#define ClawRotationServoPWM_ReloadValue             (1999)

#define ClawGrabServoPWM_Frequence                   (50)
#define ClawRotationServoPWM_ReloadValue             (1999)

#define ArmRotationServoMinimumRotationAngle         (0)
#define ArmRotationServoMaximumRotationAngle         (180)
#define ArmRotationServoProportion                   (0.867)
#define ArmRotationServoOffset                       (0.0)

#define ArmElongationServo0_MinimumRotationAngle     (0)
#define ArmElongationServo0_MaximumRotationAngle     (180)
#define ArmElongationServo0_Proportion               (0.857)
#define ArmElongationServo0_Offset                   (0.0)

#define ArmElongationServo1_MinimumRotationAngle     (0)
#define ArmElongationServo1_MaximumRotationAngle     (180)
#define ArmElongationServo1_Proportion               (1.0)//(0.908386)
#define ArmElongationServo1_Offset                   (0.0)//(-1.218346)

#define ArmElongationServo2_MinimumRotationAngle     (-90)
#define ArmElongationServo2_MaximumRotationAngle     (90)
#define ArmElongationServo2_Proportion               (1.0)
#define ArmElongationServo2_Offset                   (9.0)//(4.4)

#elif(ArmType == LiftingPlatform)
#define ArmRotationServo                             { .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH1 }
#define ArmElongationServo0                          { .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH2 }
#define ArmElongationServo1                          { .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH3 }
#define ClawRotationServo                            { .TIMx = TIM1, .Channel = LL_TIM_CHANNEL_CH4 }
#define ClawGrabServo                                { .TIMx = TIM3, .Channel = LL_TIM_CHANNEL_CH3 }

#define ArmRotationServoPWM_Frequence                (50)
#define ArmRotationServoPWM_ReloadValue              (1999)

#define ArmElongationServo0_PWM_Frequence            (50)
#define ArmElongationServo0_ReloadValue              (1999)

#define ArmElongationServo1_PWM_Frequence            (50)
#define ArmElongationServo1_PWM_ReloadValue          (1999)

#define ClawRotationServoPWM_Frequence               (50)
#define ClawRotationServoPWM_ReloadValue             (1999)

#define ClawGrabServoPWM_Frequence                   (1000)
#define ClawRotationServoPWM_ReloadValue             (1999)

#define ArmRotationServoMinimumRotationAngle         (0)
#define ArmRotationServoMaximumRotationAngle         (180)
#define ArmRotationServoProportion                   (0.867)
#define ArmRotationServoOffset                       (0.0)

#define ArmElongationServo0_MinimumRotationAngle     (0)
#define ArmElongationServo0_MaximumRotationAngle     (180)
#define ArmElongationServo0_Proportion               (0.857)
#define ArmElongationServo0_Offset                   (0.0)

#define ArmElongationServo1_MinimumRotationAngle     (0)
#define ArmElongationServo1_MaximumRotationAngle     (180)
#define ArmElongationServo1_Proportion               (1.0)//(0.908386)
#define ArmElongationServo1_Offset                   (0.0)//(-1.218346)

#endif


/******************Direction Sensor Config******************/
//List of available models.
#define WT101_InSerialMode   0
//#define WT101_InI2C_Mode     1
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

/**********************Stepper Config**********************/
#if(ArmType == LiftingPlatform)
#define StepperNumbers              (2)
#define Z_AxisStepperStepPort       GPIOB
#define Z_AxisStepperStepPin        LL_GPIO_PIN_12
#define Z_AxisStepperDirPort        GPIOB
#define Z_AxisStepperDirPin         LL_GPIO_PIN_13
#define AL_AxisStepperStepPort      GPIOB
#define AL_AxisStepperStepPin       LL_GPIO_PIN_14
#define AL_AxisStepperDirPort       GPIOB
#define AL_AxisStepperDirPin        LL_GPIO_PIN_15

#define Z_AxisStepper               { .StepperIO = (GPIO_t){.Port = Z_AxisStepperStepPort, .Pin = Z_AxisStepperStepPin}, .DirectionIO = (GPIO_t){.Port = Z_AxisStepperDirPort, .Pin = Z_AxisStepperDirPin}, .CurrentSteps = 0, .TargetSteps = 0}
#endif

/**********************Encoder Config**********************/
#define LeftEncoder


/**
 * @group: Platform Configs
 */
/************************GPIO Configs***********************/
#define GPIO_Ports_t                GPIO_TypeDef
#define GPIO_Pin_t                  uint32_t

#define BT05_Port                   GPIOC
#define BT05_Pin                    LL_GPIO_PIN_3

/************************PWM Configs************************/
#define TIM_Ports_t                 TIM_TypeDef
#define TIM_Channel_t               uint32_t


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



#endif /* ROBOTCONFIGS_H_ */
