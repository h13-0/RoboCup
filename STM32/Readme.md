# STM32
本部分负责机器人除图像处理以外的所有流程控制部分。  
本程序采用LL库, 底层使用CubeMX生成, 其他层由CubeIDE编写。  
如需复现此项目, 请自行安装[STM32CubeMX](https://www.st.com/zh/development-tools/stm32cubemx.html)和[STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)。  

## 为什么不用STP和Keil?
Keil的编译器过于老旧, 在实际使用中遇到了一些BUG, 并且stp(标准库)不再支持STM32F7、STM32G0等新产品。
而LL效率和stp相当甚至更优, 写法相似, 故迁移至LL进行编写。

## 程序移植
你只需要重新实现 `Platform` 文件夹中全部的函数即可。  
即使你使用的是Hal库或者标准库, 只要STM32和C语言过关, 就不存在任何问题。  
~~即如果您无法从LL移植到Hal或者标库，不是本项目的问题。~~

<details>
<summary>代码结构</summary>
程序分为应用层, 驱动层, 底层。各层之间解耦合。  
应用层只能通过 `Platform/ports.h` 调用驱动层。  
驱动层只能通过 `Platform/xx.h` 调用底层。  
为了考虑整体移植性, 降低了部分驱动层使用简洁性。  

如无特殊需求, 只需要修改 `App.c` 文件即可实现比赛全流程。  

程序结构:  
```
STM32:
│  .cproject 
│  .project                                 -> 项目文件, 使用STM32CubeIDE打开本文件即可。
│  STM32.ioc                                -> CubeMX配置文件, 使用CubeMX打开本文件进行底层配置。
│  STM32F103RCTX_FLASH.ld
│
├─Drivers                                   -> [ST库文件]
│  ├─CMSIS                                  -> [ST库文件] CMSIS
│  │
│  └─STM32F1xx_HAL_Driver                   -> [ST库文件] STM32的Hal和LL库文件
│
├─Images
│  ├─ChineseFonts                           -> [资源文件] 隶书汉字字模, 字模转换用
│  │      *.bmp
│  │
│  └─SchoolMotto                            -> [资源文件] 校训图, 字模转换用
│          SchoolMotto.bmp
│
├─Inc
│
├─Src
│  │  main.c
│  │  PrivateConfigs.h                      -> [配置文件] 隐私相关配置文件, 如电话号码, 已gitignore。
│  │  RobotConfigs.h                        -> [配置文件] 机器人全局配置文件。
│  │  stm32f1xx_hal_msp.c
│  │  stm32f1xx_it.c                        -> [中断管理] 中断表。
│  │  syscalls.c
│  │  sysmem.c
│  │  system_stm32f1xx.c
│  │
│  ├─App                                    -> [应用层]
│  │  │  App.c                              -> **比赛主要逻辑流程**
│  │  │  App.h
│  │  │
│  │  ├─AppLog                              -> Log库实现
│  │  │  │  AppLog.c
│  │  │  │  AppLog.h
│  │  │  │
│  │  │  └─JustFloat                        -> VOFA 上位机的 `JustFloat` 协议实现
│  │  │          JustFloat.c
│  │  │          JustFloat.h
│  │  │
│  │  ├─ArmControl                          -> 机械臂控制库
│  │  │      ArmControl.h
│  │  │      LiftingPlatform.c              -> 升降台机械臂控制实现
│  │  │      LiftingPlatform.h
│  │  │      MechanicalArm.c                -> 传统机械臂控制实现
│  │  │
│  │  ├─Debug
│  │  │  │  Debug.c                         -> 将串口中断信息整理到`ParameterAdjust`中
│  │  │  │  Debug.h
│  │  │  │
│  │  │  └─ParameterAdjust                  -> 参数调节器
│  │  │          ParameterAdjust.c
│  │  │          ParameterAdjust.h
│  │  │
│  │  ├─Examples                            -> [Examples] 本文件夹下为调用App、Driver、Platform层的函数Demo以及赛题要求的子步骤函数实现。
│  │  │      AllInit.c                      -> [赛题实现] 初始化所有软硬件的Demo
│  │  │      AllInit.h
│  │  │      AngleSensorTest.c              -> [Demo] 测试陀螺仪偏航角的实现
│  │  │      AngleSensorTest.h
│  │  │      BluetoothTest.c                -> [赛题实现] 赛题所要求的蓝牙相关功能
│  │  │      BluetoothTest.h
│  │  │      BroadcastIdentifyResult.c      -> 使用语音播报水果识别结果的实现
│  │  │      BroadcastIdentifyResult.h
│  │  │      CatchApple.c                   -> [赛题实现] 抓取苹果的实现
│  │  │      CatchApple.h
│  │  │      CatchTrashCan.c                -> [赛题实现] 抓取垃圾桶的实现
│  │  │      CatchTrashCan.h
│  │  │      DisplayResult.c                -> [赛题实现] 将识别结果显示在显示屏上的实现
│  │  │      DisplayResult.h
│  │  │      DisplaySchoolMotto.c           -> [赛题实现] 显示校训
│  │  │      DisplaySchoolMotto.h
│  │  │      EmptyTrash.c                   -> [赛题实现] 清空垃圾桶的实现
│  │  │      EmptyTrash.h
│  │  │      Examples.h                     -> [接口文件] 所有Example的头文件
│  │  │      IdentifyFruit.c                -> [赛题实现] 识别水果的实现
│  │  │      IdentifyFruit.h
│  │  │      PlaceApple.c                   -> [赛题实现] 放置水果的实现
│  │  │      PlaceApple.h
│  │  │      PlaceTrashCan.c                -> [赛题实现] 放置垃圾桶的实现
│  │  │      PlaceTrashCan.h
│  │  │      PrepareArmPosition.c           -> [赛题实现] 使机械臂进入准备状态
│  │  │      PrepareArmPosition.h
│  │  │      SendResultViaMessage.c         -> [赛题实现] 将识别结果通过短信发送到指定手机
│  │  │      SendResultViaMessage.h
│  │  │      ThrowApple.c                   -> [赛题实现] 扔苹果
│  │  │      ThrowApple.h
│  │  │      WashApple.c                    -> [赛题实现] 洗苹果
│  │  │      WashApple.h
│  │  │
│  │  ├─FastMatch                           -> [协议解析] 一个简单协议解析器
│  │  │      FastMatch.h
│  │  │
│  │  ├─MotionControl                       -> [运动控制] 运动控制实现
│  │  │      MotionControl.c
│  │  │      MotionControl.h
│  │  │
│  │  ├─PID                                 -> [PID] PID算法实现
│  │  │      PID.c
│  │  │      PID.h
│  │  │
│  │  ├─Timer                               -> [定时器] 系统框架定时器
│  │  │      Timer.c
│  │  │      Timer.h
│  │  │
│  │  ├─TOF
│  │  ├─UI                                  -> [UI] 简单UI实现
│  │  │      DrawPicture.c                  -> 绘制图像
│  │  │      DrawPicture.h
│  │  │      DrawText.c                     -> 绘制文本
│  │  │      DrawText.h
│  │  │      OfficialScriptChineseFont.c    -> 隶书汉语字体库
│  │  │      OfficialScriptChineseFont.h
│  │  │      SchoolMotto.c                  -> 校训图片库
│  │  │      SchoolMotto.h
│  │  │      UI.h                           -> UI接口文件
│  │  │
│  │  └─VoicePlayer                         -> [VoicePlayer] 语音播报框架
│  │          VoicePlayer.c
│  │          VoicePlayer.h
│  │
│  ├─Drivers                                -> [驱动层]
│  │  │  Drivers.c                          -> 驱动层初始化API实现
│  │  │  Drivers.h
│  │  │
│  │  ├─Bluetooth                           -> [Driver] HC-05蓝牙驱动
│  │  │      Bluetooth.c
│  │  │      Bluetooth.h
│  │  │
│  │  ├─DF_PlayerMini                       -> [Driver] DF Player Mini 语音播放器驱动
│  │  │      DF_PlayerMini.c
│  │  │      DF_PlayerMini.h
│  │  │
│  │  ├─Encoder                             -> [Driver] 编码器驱动
│  │  │      Encoder.c
│  │  │      Encoder.h
│  │  │
│  │  ├─ImageProcessingModule               -> [Driver] 图像处理模块驱动(即协议解析器)
│  │  │      ImageProcessingModule.c
│  │  │      ImageProcessingModule.h
│  │  │
│  │  ├─INA219                              -> [Driver] INA219电流传感器驱动
│  │  │      INA219.c
│  │  │      INA219.h
│  │  │
│  │  ├─Luat                                -> [Driver] Air724UG 4G 模块AT驱动
│  │  │      Luat.c
│  │  │      Luat.h
│  │  │
│  │  ├─Motor                               -> [Driver] H桥电机驱动板驱动
│  │  │      Motor.c
│  │  │      Motor.h
│  │  │
│  │  ├─Servo                               -> [Driver] 舵机驱动
│  │  │      Servo.c
│  │  │      Servo.h
│  │  │
│  │  ├─SimpleProtocolPraise                -> [Driver] 简单协议解析器
│  │  │      SimpleProtocolPraise.h
│  │  │
│  │  ├─SKTOF                               -> [Driver] SK TOF 驱动
│  │  │      SKTOF.c
│  │  │      SKTOF.h
│  │  │
│  │  ├─SSD1283                             -> [Driver] SSD1283 屏幕驱动
│  │  │      SSD1283.c
│  │  │      SSD1283.h
│  │  │
│  │  ├─Stepper                             -> [Driver] 步进驱动
│  │  │      Stepper.c
│  │  │      Stepper.h
│  │  │
│  │  ├─VL6180X                             -> [Driver] VL6180X 短距离TOF 驱动
│  │  │  ├─Core
│  │  │  │      vl6180x_api.c
│  │  │  │      vl6180x_api.h
│  │  │  │      vl6180x_cfg.h
│  │  │  │      vl6180x_def.h
│  │  │  │      vl6180x_i2c.c
│  │  │  │      vl6180x_i2c.h
│  │  │  │
│  │  │  └─Platform
│  │  │          vl6180x_easy_api.c         -> [Driver] VL6180X 驱动简易接口实现
│  │  │          vl6180x_easy_api.h
│  │  │          vl6180x_platform.c
│  │  │          vl6180x_platform.h
│  │  │          vl6180x_types.h
│  │  │
│  │  └─WT101                               -> [Driver] WT101 陀螺仪 驱动
│  │          WT101.c
│  │          WT101.h
│  │
│  └─Platform                               -> 底层, 用于适配LL库。如果想要移植到Hal库或标准库, 请关注本文件夹。
│      │  ports.c                           -> [Platform] 底层初始化API实现
│      │  ports.h
│      │
│      ├─Clock                              -> [Platform] 系统时钟实现。
│      │      Clock.c
│      │      Clock.h
│      │      Readme.md
│      │
│      ├─EncoderPorts                       -> [Platform] 编码器底层接口。
│      │      Encoder.md
│      │      EncoderPorts.c
│      │      EncoderPorts.h
│      │
│      ├─GPIO                               -> [Platform] GPIO底层接口。
│      │      GPIO.c
│      │      GPIO.h
│      │
│      ├─I2C                                -> [Platform] I2C底层接口。
│      │      I2C.c
│      │      I2C.h
│      │
│      ├─Interrupts                         -> [Platform] 中断底层接口。
│      │  │  Interrupts.h
│      │  │
│      │  ├─ExternalInterrupts              -> [Platform] 外部中断
│      │  ├─TimerInterrupts                 -> [Platform] 定时器中断
│      │  │      TimerInterrupts.c
│      │  │      TimerInterrupts.h
│      │  │
│      │  └─USARTInterrupts                 -> [Platform] 串口中断
│      │          USARTInterrupts.c
│      │          USARTInterrupts.h
│      │
│      ├─PWM                                -> [Platform] PWM底层实现
│      │      PWM.c
│      │      PWM.h
│      │
│      ├─SPI                                -> [Platform] SPI底层实现
│      │      SPI.c
│      │      SPI.h
│      │
│      └─USART                              -> [Platform] 串口底层实现
│              USART.c
│              USART.h
│
├─Startup
│      startup_stm32f103rctx.s
│
└─Voices                                    -> [资源文件] 语音播报原素材, 请使用本文件下的素材文件进行播报。
        00*.mp3
```
</details>

# 电路图  
等待队友上传。  

# 硬件选择
如您需要复现本项目, 建议使用STM32F103RC以上的型号, 由于串口数量原因, 不建议使用本项目所使用的的STM32F103RCT6。  
本项目所有重要模块的开发文档均已上传至[Docs](../Docs/)  
## 陀螺仪
陀螺仪选型为WT101, 经过测试, MPU9250不满足需求。  

## 激光测距
### 主激光
SK-Z-20 串口激光测距。  
### 机械臂闭环激光
由于本结构机械臂坐标系解算控制精度已达标, 因此项目中未使用, 推荐VL6180X。  

## 显示屏
SSD1283半反半透屏, 特性是阳光下清晰可见。  

## 4G模块
[合宙Air724UG](https://www.openluat.com/product_center/product_detail?id=12)  
2020年年初价格: 19.9元一张核心板一张开发板。  

## 电流检测
通过检测机械爪舵机电流来检测机械爪是否成功抓到物体。  
可以使用图像识别模块代替, 本项目中未使用。  

## 蓝牙模块
HC-05  

## 电机驱动(H桥)
298NAD  

## 电机
1:30减速比, 100线正交光电码盘, 转速280。  

## 定时器  
定时器选择请参考STM32CubeMX配置文件。  

## 舵机
舵机命名参考: 
![LiftingPlatformAnnotationPicture2.jpg](../Images/LiftingPlatformAnnotationPicture2.jpg)  
舵机选型(以`LiftingPlatform`为准):  
| Servo ID           | Model         |
| ------------------ | ------------- |
| ArmRotationServo   | TD8120MG 270° |
| ArmElongationServo | DS3235MG 180° |
| ArmParallelServo   | DS3230MG 180° |
| ClawRotationServo  | TD8120MG 180° |
| ClawGrabServo      | TBSN-K15 180° |
MG为数字舵机, SG为模拟舵机。  


## 串口调参
接口: USART1  
波特率: 1152000  
上位机使用[VOFA+](https://www.vofa.plus/)  
参考: [HostComputer](../HostComputer/VOFAConfig/Readme.md)  

### 图像处理模块  
使用RK3328+RK1808S0计算棒, 推荐替换为RK3568.  
#### 图像处理部分参考    
[图像处理](../ImageProcessing/Readme.md)  
[通信协议](../ImageProcessing/Protocol.md)  
