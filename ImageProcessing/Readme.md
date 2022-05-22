# 水果种类
1. 苹果
2. 香梨
3. 橘子
4. 雪梨
5. 柠檬
6. 桃子
7. 香蕉
8. 火龙果
9. 猕猴桃

# 通信协议
[Protocol](/ImageProcessing/Protocol.md)

# 识别方案
## YoloV4Tiny
效果最好, 但是算力要求较高。
建议部署于RK1808或RK3568等平台上。
使用[Darknet](https://github.com/AlexeyAB/darknet)进行训练。
可识别内容仅限于上述9种水果, 不包括靶标。
靶标建议使用HSV色块识别。
训练集
> 链接：https://pan.baidu.com/s/13T8VhDXatusKtdSlCLSglQ?pwd=kcy0 
> 提取码：kcy0

## HSV色块识别
**不建议用于水果识别, 可用于靶标识别**
[HSV水果识别实现]

# 最终方案
使用YoloV4Tiny进行水果识别, 利用`RKNN_Lite`部署于RK1808计算棒上, HSV进行靶标识别，
最终方案见: [RKNN](\ImageProcessing\RKNN\Readme.md)
