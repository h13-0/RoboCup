# 通信协议
**物理接口采用串口, 波特率115200, 建议配合DMA使用**

基础协议为二进制小端通信, 基础协议结构:  
`${Key}:${HexValue}`  
**`${HexValue}` 表示小端模式的二进制信息。**  

## 协议表:  
### STM32 -> Module:
#### 命令
Key = `CMD`  
Type = char*  
| Key | Value | 作用 |
| --- | ----- | ---- |
| CMD | 

### Module -> STM32:
#### 工作模式
Key = `WM`  
Type = uint8_t  
| Value | 作用           |
| ----- | -------------- |
| 0     | Standby        |
| 1     | AppleDetection |

#### 苹果坐标输出
Key = `AppCenX`  
Type = float
Key = `AppCenY`
Type = float

| Key       | Value | 作用    |
| --------- | ----- | ------- |
| `AppCenX` | 0~1   | AppCenX |
| `AppCenY` | 0~1   | AppCenY |

#### 水果种类输出


