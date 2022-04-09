# 程序框架
本程序采用LL库, 底层使用CubeMX生成, 其他层由CubeIDE编写。

## 为什么不用STP和Keil?
Keil的编译器过于老旧, 在实际使用中遇到了一些BUG, 并且stp(标准库)不再支持STM32F7、STM32G0等新产品。
而LL效率和stp相当甚至更优, 写法相似, 故迁移至LL进行编写。

## 程序移植
你只需要重新实现 `Platform` 文件夹中全部的函数即可。
即使你使用的是Hal库或者标准库, 只要STM32和C语言过关, 就不存在任何问题。
~~即如果您无法从LL移植到Hal或者标库，不是我的问题。~~

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
│  │
│  └─SchoolMotto                            -> [资源文件] 校训图, 字模转换用
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
```

</details>

# 硬件选择
## GPIO
### 码盘
```
PC0 1 2 3
```

## TIM
TIM需要实现H桥控制、舵机控制和定时中断的产生。  

定时器优先使用TIM6, TIM7。其为基本定时器, 可以满足定时中断需求, 但无法输出PWM。  
PWM输出优先选择TIM2 - TIM5, TIM9 - TIM14。其为通用定时器, 可以输出PWM。  
最后考虑占用TIM1, TIM8。因为其还可以实现高级TIM功能。  

最终选择牺牲一个高级定时器来减轻IO布局压力。

### Timer
TIM7设置为定时器1。  

### PWM
#### Servos
TIM  
> 分频系数   719
> 预填充系数 1999

2000 -> 20ms  
分辨率0.01ms, 预填充系数50-250  

第0级舵机, 复位点0.5ms  
第1级舵机, 复位点0.5ms  
第2级舵机, 复位点2.5ms  
第3级舵机, 复位点2.5ms  
第4级舵机, 复位点0.5ms  
第5级舵机, 复位点0.5ms, 闭合角度73°

TIM1: 
```
Channel 1 -> PA8 -> 机械臂0号舵机
Channel 2 -> PA9 -> 机械臂1号舵机
Channel 3 -> PA10 -> 机械臂2号舵机
Channel 4 -> PA11 -> 机械臂3号舵机
```

TIM2: 
```
Channel 1 -> PA0 -> 机械臂4号舵机
Channel 2 -> PA1 -> 机械臂5号舵机
```

#### Motor
TIM  
> 分频系数   71
> 预填充系数 999

TIM3:   
```
Channel 1 -> PA6 -> D0
-> PA7 -> D1
Channel 2 -> PA7 -> D2
 -> PB1 -> D4
```
H桥真值表:  
**Output 0**
| 引脚 | 电平  | 电平 | 电平 | 电平 |
| ---- | ---- | ---- | ---- | ---- |
| D0   | 1    | 0    | 1    | 0    |
| D1   | 0    | 1    | 1    | 0    |
| 状态 | 正转  | 反转 | 刹车 | 悬空 |
**Output 1**
| 引脚 | 电平  | 电平 | 电平 | 电平 |
| ---- | ---- | ---- | ---- | ---- |
| D2   | 1    | 0    | 1    | 0    |
| D3   | 0    | 1    | 1    | 0    |
| 状态 | 正转  | 反转 | 刹车 | 悬空 |

## I2C
I2C需要实现VL6180X数据的读取等。  

I2C1:  
```
PB8 -> SCL
PB9 -> SDA
```

I2C地址表  
| 模块    | 地址 |
| ------- | ---- |
| VL6180X | 0x52 |
| mpu6050 | 0x68 |
| INA219  | 0x40 |

## UART
```
USART3
USART2
```

UART需要完成:
1. 串口调参(debug)/串口屏(release, 移植LVGL虽然没有难度, 但是还得再浪费一个定时器, 除非上FreeRTOS)等 (收发)  
2. TOF数据的获取和处理 (只需要接收)  
3. 蓝牙模块数据的收发 (收发)  
4. OpenMV数据通信 (收发)  

### 串口调参
接口: USART1  
波特率: 1152000  
```
PB6 = TX
PB7 = RX
```

### 串口屏
接口: USART1  

### TOF数据获取
接口: USART3  
```
PB10 = TX
PB11 = RX
```


```
7E 30 31 30 33 30 31 30 30 30 33 39 30 34 34 41 41 0D 0A 
7E 30 31 30 33   30 31   30 30   30 31   42 34 34 35 44 31 0D 0A
```

### 图像处理模块  
接口: UART4  
```
PC10 = TX
PC11 = RX
```

### 蓝牙模块  
> 波特率: 115200

接口: UART5 
| STM32功能 | 映射引脚 | 蓝牙串口引脚   |
| --------- | ------- | -------------- |
| Power     | 5V      | VCC            |
| Power     | GND     | GND            |
| UART5 TX  | PC12    | RX             |
| UART5 RX  | PD2     | TX             |
| GPIO      | PC0     | EN(仅限于HC05) |

对于HC05:

#### 焊接前
串口上位机, 38400波特率, 按住EN按钮再上电(使EN为高, 进入强制AT模式, 波特率只能为38400)。
```
AT+ORGL
AT+UART=115200,0,0
```

#### 焊接后STM32对蓝牙初始化命令
上电EN默认低电平, 进入普通模式。
每条AT指令前需要使EN为高进入普通AT模式, 波特率为115200。
```
AT+ROLE=1
AT+CLASS=0
AT+INQM=1,9,48
AT+CMODE=1
AT+PSWD="2020"
AT+PAIR=0019,09,0314B9,20
```


```
AT+ORGL
AT+UART=115200,0,0
AT+ROLE=1
AT+CLASS=0
AT+INQM=1,9,48
AT+CMODE=1
AT+PSWD="2020"
AT+PAIR=0019,09,0314B9,20
AT+LINK=0019,09,0314B9
```


#### AT指令:
##### 发送中文短信
```
STM32 -> AT+CMGF=1
OK
AT+CSMP=17,167,0,8
OK
AT+CMGS="xxxx"
>
${Unicode后四位的HEX}${0x1A}
+CMGS:
```

```
4E‌2D 65‌87 6D‌4B 8B‌D5 0‌0‌48 0‌0‌65 0‌0‌6C ‌0‌0‌6C 0‌0‌6F 0‌0‌57 0‌‌0‌6F 0‌0‌72 0‌0‌‌6C 0‌0‌‌64 0‌0‌‌21 1A
```

## SPI
SPI1  
> BaudRate: 18Mbits/s
> DMA:

| STM32功能 | 映射引脚 |
| --------- | ------- |
| SPI1 SCK  | PB3     |
| SPI1 MISO | PB4     |
| SPI1 MOSI | PB5     |

### SSD1283 半反半透屏
> 分辨率: 130*130
> 特点:   阳光下可以看清
> SPI相关配置:
>   First Bit: MSB
>   CPOL:      随意.
>   CPHA:      随意, 1 Eege / 2 Edge 均可.
>   CRC:       随意, 可以Disable.

接口: SPI1
接线:
| STM32功能 | 映射引脚 | SSD1283引脚 |
| --------- | ------- | ----------- |
| Power     | 5V      | VCC         |
| Power     | GND     | GND         |
| SPI1 SCK  | PB3     | SCL (SCK)   |
| SPI1 MOSI | PB5     | SDA (MOSI)  |
| GPIO CS   | PC4     | CS          |
| GPIO      | PC5     | DC (A0/RS)  |
| GPIO      | PA5     | RST         |
| Power     | 5V      | LED         |

#### UI框架 LVGL
[官网: https://github.com/lvgl/lvgl](https://github.com/lvgl/lvgl)
> 版本: 7.11.0


# 图像处理部分  
参考:  
[通信协议](../ImageProcessing/Protocol.md)  
