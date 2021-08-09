# OpenMV水果识别部分

## 串口通信
### STM32 -> OpenMV
```

```

### OpenMV -> STM32
```

import struct
usart = UART(3, 115200)
msgPack = struct.pack("<ffbbbb", float((r[0] + r[2]) / 400.0), float((r[1] + r[3]) / 240.0), 0x00, 0x00, 0x80, 0x7f)
usart.write(msgPack)
```

## Cascade级联分类器实现

### 制作数据集

### 打包数据集
```
opencv_createsamples.exe -info E:\project\RoboCup\OpenMV\CascadeClassifier\Resized\Apple\List.txt -vec apple.vec -bg E:\project\RoboCup\OpenMV\CascadeClassifier\Background\List.txt -num 151 -w 32 -h 32
```

### 训练
```
mkdir xml
C:\Users\h13\Documents\OpenCVCascade\opencv_traincascade.exe -data xml -vec .\apple.vec -bg E:\project\RoboCup\OpenMV\CascadeClassifier\Background\List.txt -numPos 120 -numNeg 151 -numStages 17 -featureType haar -w 32 -h 32
```

### 转换二进制文件
```
python.exe .\cascade_convert.py .\cascade.xml
```
