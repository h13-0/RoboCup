# 程序框架
本程序采用LL库, 底层使用CubeMX生成, 其他层由CubeIDE编写。

## 为什么不用STP和Keil?
Keil的编译器过于老旧, 有很多BUG, 并且stp(标准库)不再支持STM32F7 G0等新产品。
而LL效率和stp相当甚至更优, 写法相似, 故迁移至LL进行编写。

## 程序移植
你只需要重新实现 `Platform` 文件夹中全部的函数即可。即使你使用的是Hal库或者标准库, 只要STM32和C语言过关, 就不存在任何问题。
<details>
<summary>代码结构</summary>
程序分为应用层, 驱动层, 底层。各层之间解耦合。  
应用层只能通过 `Platform/ports.h` 调用驱动层(包括应用层的lvgl也是如此)。  
驱动层只能通过 `Platform/xx.h` 调用底层。  
为了考虑整体移植性, 降低了部分驱动层使用简洁性。  

如无特殊需求, 请只关注 `App.c` 文件。  

程序结构:  
```
STM32:
├─Drivers             -> CMSIS以及HAL, LL库, 无需关心。
├─Inc                 -> 无需关心
├─Src                 -> 工程源代码, 如果要更换为标准库则只需要保留本文件夹并重建工程移植即可。
│  │  main.c          -> 程序入口, 大部分由CubeMX自动生成, 负责初始化底层和接入App()函数。
│  │  stm32f1xx_it.c  -> 中断管理文件, 负责调度对应中断接口。
│  ├─App              -> 应用层, 主要逻辑部分。重建工程需要移植对应中断函数接口和App()函数。
│  │  └─ThirdParty    -> 应用层, 第三方依赖库总文件夹。尽量不要修改本文件夹内文件。
│  │      └─lvgl      -> 应用层, lvgl源文件。
│  ├─Motor            -> 驱动层, H桥驱动, 引用MotorPorts库。
│  ├─mpu9250          -> 驱动层, MPU9250陀螺仪驱动, 引用I2C底层。
│  ├─Platform         -> 底层总文件夹。
│  │  ├─AppLog        -> 底层应用层, 为了优化串口而无法高效解耦的简易Log库, 移植只需要移植JustFloat库和SerialPrintf库, 并且重写Init函数。
│  │  ├─Clock         -> 底层, 获取系统时钟和延时函数库, 如果需要更换为标准库需要重写其中函数。
│  │  ├─I2C           -> 底层, I2C库, 如果需要更换I2C接口ID或者更换为标准库需要重写其中函数。
│  │  ├─JustFloat     -> 底层应用层, 为了优化串口而无法高效解耦的VOFA+上位机通信协议, 移植只需要重新实现串口发送二进制相关函数。
│  │  ├─lvglPorts     -> 底层, lvgl的底层配置库。
│  │  ├─MotorPorts    -> 底层, Motor的底层配置库。
│  │  ├─SerialPrintf  -> 底层应用层, 为了优化串口而无法高效解耦的串口printf重定向实现, 移植只需要重新实现串口发送二进制相关函数。
│  │  ├─ServoPorts    -> 底层, Servo的底层配置库。
│  │  └─SPI           -> 底层, SPI库, 如果需要更换SPI接口ID或者更换为标准库需要重写其中函数。
│  ├─Servo            -> 驱动层, 舵机机械臂驱动接口, 引用ServoBase库。
│  └─SSD1283          -> 驱动层, SSD1283屏幕驱动, 引用SPI库。
└─Startup             -> 启动文件, 无需关心。
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
第5级舵机, 复位点2.5ms  

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
Channel 2 -> PA7 -> D1
Channel 3 -> PB0 -> D2
Channel 4 -> PB1 -> D4
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

### 备用TIM
TIM8:
```
PC6 PC7 PC8 PC9
```

## I2C
I2C需要实现VL6180X数据的读取等。  
而I2C只有两个, 故需要自行实现互斥锁。  

I2C1:  
```
PB8 -> SCL
PB9 -> SDA
```

## UART
以下所有需求均不需要USART实现, 只需要UART即可。两者区别自行百度。  

UART需要完成:
1. 串口调参(debug)/串口屏(release, 移植LVGL虽然没有难度, 但是还得再浪费一个定时器, 除非上FreeRTOS)等 (收发)  
2. TOF数据的获取和处理 (只需要接收)  
3. 蓝牙模块数据的收发 (收发)  
4. OpenMV数据通信 (收发)  

### 串口调参
接口: USART1  
波特率: 1152000  
接口:   
```
PB6 = TX
PB7 = RX
```

### 串口屏
接口: USART1  

### 陀螺仪
**实测MPU9250偏航角mpl漂移率无法满足要求, 故替换为EPSON XV7011**
接口: USART2
```
PA2 = TX
PA3 = RX
```

### TOF数据获取
接口: USART3  
```
PB10 = TX
PB11 = RX
```


```
7E 30 31 30 33 30 31 30 30 30 33 39 30 34 34 41 41 0D 0A 
7E 30 31 30 33   30 31   30 30   30 31   42 34 34 35 44 31 0D 0A
~ 01 03  0100  0196  C5C8
~ 01 030100  0196  C5C8
```

### 图像处理模块
接口: UART4  
```
PC10 = TX
PC11 = RX
```

### 蓝牙模块  
> 波特率: 9600  

接口: UART5
| STM32功能 | 映射引脚 | 蓝牙串口引脚 |
| --------- | ------- | ----------- |
| Power     |         |             |
| Power     | GND     |             |
| UART5 TX  | PC12    | RX          |
| UART5 RX  | PD2     | TX          |

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
![通信协议](../ImageProcessing/Protocol.md)  