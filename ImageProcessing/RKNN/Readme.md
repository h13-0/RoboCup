# 程序架构
文件结构: 
```
RKNN/Code:
│  Logo.jpg
│  
│  Readme.md
│
├─FrameBufferMonitor
│
├─Images
│
├─TargetDetector
│
├─WorkingFlow
│
└─YoloDetector
```

`Logo.jpg`           为启动时显示的Logo
`main.py`            为入口函数, 随后调用`WorkingFlow/MainWorkingFlow.py`  
`FrameBufferMonitor` 将图像越过Linux的UI层直接输出到屏幕的代码实现, **注意不能使用桌面模式，只能使用命令行模式(即initlevel为3而不是5)**  
`Images`             程序STM32控制程序拍照后, 图片保存的目录  
`TargetDetector`     基于HSV的靶标检测  
`WorkingFlow`        主要工作流程实现  
`YoloDetector`       RKNN的YOLO V4 Tiny实现  

## FrameBufferMonitor
具体linux的`FrameBuffer`是什么请自行百度。  
该实现使用C++和Python混合编程, C++实现详见对应src文件夹。  
如果项目所提供的[libframebuffer.so](./Code/FrameBufferMonitor/lib/libframebuffer.so)无法使用, 请自行搭建`OpenCV`环境重新编译。 
```Bash
cd ImageProcessing/RKNN/Code/FrameBufferMonitor
chomd 777 ./build.sh
./build.sh
```
如遇到错误, 请自行检查OpenCV环境。  

### 调用方式
比较简单, 建议直接查看[FrameBufferMonitor.py](./Code/FrameBufferMonitor/FrameBufferMonitor.py)源码。  

## Images
文件命名方式: `时间戳.jpg`。  
本功能是为了方便调节靶标的HSV色域提供的方法。  
在STM32程序中的[PlaceApple.c](../../STM32/Src/App/Examples/PlaceApple.c)中有所调用。  
`STM32/Src/App/Examples/PlaceApple.c Line 121`
```C
TakeSnapShot();
```

## TargetDetector
本方案采用HSV进行实现。
为方便调参, 我写了一个HSV调参工具: [HSV_Range](https://github.com/h13-0/HSV-Range)
参数较为简单, 建议直接阅读源码: [TargetDetector.py](./Code/TargetDetector/TargetDetector.py)
值得注意的是, 本参数仅适用于固定的靶标和**固定的分辨率(640, 320)**

## WorkingFlow
参数较多但简单, 看源码吧。

## YoloDetector
模型文件需要从`Darknet`转换到`rknn`, 参考: [tools](../YoloV4Tiny/tools/Readme.md)

# 环境配置
有以下几种方案(按推荐顺序排序):
1. [RKNN_Lite](https://github.com/rockchip-linux/rknn-toolkit/tree/master/rknn-toolkit-lite/packages)
2. [直接使用Archiconda部署](./Environment/RK3328_env/Readme.md)
3. ROC-RK3328-CC 系统镜像: 购买板子直接刷入镜像即可。

本文件夹下所有文件的下载链接: 
> 链接：https://pan.baidu.com/s/1-s8oynvUIC-BW_bOgdHDag?pwd=heop 
> 提取码：heop

