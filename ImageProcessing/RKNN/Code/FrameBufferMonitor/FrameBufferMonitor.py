import ctypes
import numpy as np

LibFrameBufferMonitor = ctypes.CDLL("./FrameBufferMonitor/lib/libframebuffer.so")
LibFrameBufferMonitor.Init.restype = ctypes.c_void_p

class FrameBufferMonitor:
    def __init__(self, TargetFrameBufferFile : str):
        self.__frameBufferHandle__ = ctypes.c_void_p(LibFrameBufferMonitor.Init(TargetFrameBufferFile.encode()))


    def Display(self, BGR_Image : np.ndarray):
        LibFrameBufferMonitor.Display(self.__frameBufferHandle__, BGR_Image.tobytes(), BGR_Image.shape[0], BGR_Image.shape[1])


    def __del__(self) -> None:
        LibFrameBufferMonitor.DeInit(self.__frameBufferHandle__)


if __name__ == "__main__":
    import cv2 as cv
    import time
    cap = cv.VideoCapture(0)
    cap.set(cv.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv.CAP_PROP_FRAME_HEIGHT, 480)
    fb = FrameBufferMonitor("/dev/fb0")
    lastTime = time.time()
    while(True):
        ret, frame = cap.read()
        if(ret):
            print(frame.shape)
            print(time.time() - lastTime)
            lastTime = time.time()
            startTime = time.time()
            fb.Display(frame)
            endTime = time.time()
            print(endTime - startTime)
