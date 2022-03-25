# 通信协议
**物理接口采用串口, 波特率115200, 建议配合DMA使用**

基础协议为二进制小端通信, 基础协议结构:  
`${Key}:${HexValue}`  
**`${HexValue}` 表示小端模式的二进制信息。**  

## 协议表:  
### STM32 -> Module:
#### 命令
Key = `CMD`  
Type = string  
| Key | Value             | 作用              |
| --- | ----------------- | ---------------- |
| CMD | `AppleDetectLeft` | 检测左侧水果      |
| CMD | `FruitIdentify`   | 检测水果数量和种类 |
| CMD | `TargetDetection` | 检测靶标位置       |

### Module -> STM32:
#### 工作模式
Key = `WM`  
Type = uint8_t  
| Value | Mode             |
| ----- | ---------------- |
| 0     | Standby          |
| 1     | AppleDetectMax   |
| 2     | AppleDetectLeft  |
| 3     | AppleDetectRight |
| 4     | TargetDetection  |
| 5     | FruitIdentify    |


#### 苹果坐标输出
Key = `AppCenX`  
Type = float
Key = `AppCenY`
Type = float

| Key       | Value | 作用    |
| --------- | ----- | ------- |
| `AppCenX` | 0~1   | AppCenX |
| `AppCenY` | 0~1   | AppCenY |

#### 靶标坐标输出
Key = `TarCenX`  
Type = float
Key = `TarCenY`
Type = float

| Key       | Value | 作用    |
| --------- | ----- | ------- |
| `TarCenX` | 0~1   | TarCenX |
| `TarCenY` | 0~1   | TarCenY |

#### 水果种类输出
Type = uint8_t  
| Key                   | Value              | 作用          |
| --------------------- | ------------------ | ------------- |
| `AppleNum`            | [1, QuantityLimit] | Number        |
| `BananaNum`           | [0, QuantityLimit] |               |
| `KiwiFruitNum`        | [0, QuantityLimit] |               |
| `LemonNum`            | [0, QuantityLimit] |               |
| `OrangeNum`           | [0, QuantityLimit] |               |
| `PeachNum`            | [0, QuantityLimit] |               |
| `PearNum`             | [0, QuantityLimit] |               |
| `PitayaNum`           | [0, QuantityLimit] |               |
| `SnowPearNum`         | [0, QuantityLimit] |               |
| `FruitDetectFinished` | 0 ~ 1              | 1 -> Finished |

