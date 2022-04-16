from PIL import Image
import numpy as np
#from matplotlib import pyplot as plt
import re
import math
import random
from rknn.api import RKNN

if __name__ == '__main__':

    # Create RKNN object
    rknn = RKNN()
    print('--> Loading model')
    rknn.load_darknet(model='E:/project/RoboCup/yolo/yoloV4TinyRetrainWithRealScene320/yolov4-tiny.cfg', weight="E:/project/RoboCup/yolo/yoloV4TinyRetrainWithRealScene320/backup/yolov4-tiny_best.weights")
    print('done')
    rknn.config(channel_mean_value='0 0 0 255', reorder_channel='0 1 2')
    # Build model
    print('--> Building model')
    rknn.build(do_quantization=True, dataset='E:/project/RoboCup/yolo/datasets/yolomarkRealScene/valid.txt', pre_compile=False)
    print('done')
    rknn.export_rknn('./yolov4-tiny.rknn')
    ret = rknn.init_runtime()
    if ret != 0:
        print('init runtime failed.')
        exit(ret)
    print('done')
    exit(0)
