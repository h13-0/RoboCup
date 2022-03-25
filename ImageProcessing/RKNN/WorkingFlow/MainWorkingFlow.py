import threading
import time
from enum import Enum
from tkinter.messagebox import NO
from typing import List
from cv2 import setIdentity

import numpy as np
import cv2 as cv

from FrameBufferMonitor.FrameBufferMonitor import FrameBufferMonitor
from YoloDetector.YoloDetector import LoadYoloV4_Tiny
from TargetDetector.TargetDetector import TargetDetector
from Protocol.SerialProtocol import SerialProtocol, ProtocolVariablesType


class WorkingMode(Enum):
    '''
        @brief: Define working mode enumeration.
    '''
    StandBy = 0
    AppleDetectMax = 1
    AppleDetectLeft = 2
    AppleDetectRight = 3
    TargetDetection = 4
    FruitIdentify = 5


class MainWorkingFlow():
    def __init__(self, 
        CameraID, SerialPort : str, BaudRate : int,
        RKNN_WeightPath : str = "./YoloDetector/yolov4-tiny.rknn",
        RKNN_Lables : list = ["Apple","Banana", "KiwiFruit", "Lemon", "Orange", "Peach", "Pear", "Pitaya", "SnowPear"],
        RKNN_Width : int = 320, RKNN_Height : int = 224,
        DisplayOnFrameBuffer : bool = True, DisplayFPS : bool = True, DisplayWorkingMode : bool = True
    ) -> None:
        '''
            @brief: Initialize MainWorkingFlow.
            @param: 
                SerialPort: Serial port.
                BaudRate: Baud rate.
        '''
        # Set camera variables.
        self.__cameraID__ = CameraID
        self.__frame__ = None
        self.__frameLock__ = None
        self.__refreshFrameThread__ = None

        # Set FrameBufferMonitor variables.
        self.__frameBufferMonitor__ = None

        # Set yolo variables.
        self.__rknnWeightPath__ = RKNN_WeightPath
        self.__rknnLables__ = RKNN_Lables
        self.__rknnWidth__ = RKNN_Width
        self.__rknnHeight__ = RKNN_Height
        self.__yoloDetector__ = None
        
        # Set target variables.
        self.__targetDetector__ = None

        # Set protocol.
        self.__serialPort__ = SerialPort
        self.__baudRate__ = BaudRate
        self.__protocol__ = None

        # Set working mode variables.
        #self.__workingMode__ = WorkingMode.StandBy
        self.__workingMode__ = WorkingMode.StandBy
        self.__workingModeLock__ = threading.Lock()

        # Set additional variables.
        self.__displayOnFrameBuffer__ = DisplayOnFrameBuffer
        self.__displayFPS = DisplayFPS
        self.__displayWorkingMode = DisplayWorkingMode

        # Set main method variables.
        self.__mainMethodThread__ = None

        # Set exit flag.
        self.__exitFlag__ = False
        self.__exitFlagLock__ = threading.Lock()


    def __cmdCallback__(self, Mode : str) -> None:
        '''
            @brief: Switch mode command callback function.
            @param: Mode: Switch mode.
        '''
        with self.__workingModeLock__:
            if(Mode == "AppleDetectMax"):
                self.__workingMode__ = WorkingMode.AppleDetectMax
                print("Switch to mode: " + Mode)
            elif(Mode == "AppleDetectLeft"):
                self.__workingMode__ = WorkingMode.AppleDetectLeft
                print("Switch to mode: " + Mode)
            elif(Mode == "AppleDetectRight"):
                self.__workingMode__ = WorkingMode.AppleDetectRight
                print("Switch to mode: " + Mode)
            elif(Mode == "TargetDetection"):
                self.__workingMode__ = WorkingMode.TargetDetection
                print("Switch to mode: " + Mode)
            elif(Mode == "FruitIdentify"):
                self.__workingMode__ = WorkingMode.FruitIdentify
                print("Switch to mode: " + Mode)
            else:
                pass


    def __calcBoxesOverlapRate__(self, box1, box2) -> float:
        minSize = min(
            (box1[2] - box1[0]) * (box1[3] - box1[1]),  # Width * Height
            (box2[2] - box2[0]) * (box2[3] - box2[1])
        )

        minRect = [
            # Left
            max(box1[0], box2[0]),
            # Top
            max(box1[1], box2[1]),
            # Right
            min(box1[2], box2[2]),
            # Buttom
            min(box1[3], box2[3])
        ]
        if(minSize > 0 and minRect[2] > minRect[0] and minRect[3] > minRect[1]):
            return (minRect[2] - minRect[0]) * (minRect[3] - minRect[1]) / minSize
        else:
            return 0


    def __getMergedBox__(self, box1, box2) -> List:
        return [
            # Left
            min(box1[0], box2[0]),
            # Top
            min(box1[1], box2[1]),
            # Right
            max(box1[2], box2[2]),
            # Buttom
            max(box1[3], box2[3])
        ]


    def __boxesPostProcess__(self, boxes, classes, scores, MaximumOverlapRate : float = 0.5):
        '''
            @brief: Delete and merge coincident boxes.
            @param:
                boxes: Boxes.
                classes: Classes.
                scores: Scores.
                MaximumOverlapRate: Maximum overlap rate.
            @return:
                boxes: Boxes.
                classes: Classes.
                scores: Scores.
        '''
        if(boxes is not None):
            i = 0
            while(i < len(boxes)):
                j = i + 1
                while(j < len(boxes)):
                    if(classes[i] == classes[j] and self.__calcBoxesOverlapRate__(boxes[i], boxes[j]) > MaximumOverlapRate):
                        boxes[i] = self.__getMergedBox__(boxes[i], boxes[j])
                        scores[j] = max(scores[i], scores[j])
                        
                        boxes = np.delete(boxes, j, axis = 0)
                        classes = np.delete(classes, j, axis = 0)
                        scores = np.delete(scores, j, axis = 0)
                    else:
                        j += 1
                i += 1
    
            return boxes, classes, scores
        else:
            return [], [], []


    def __standBy__(self, Frame : np.ndarray) -> None:
        pass


    def __appleDetectMax__(self, Frame : np.ndarray) -> None:
        boxes, classes, scores = self.__yoloDetector__.Predict(Frame)
        boxes, classes, scores = self.__boxesPostProcess__(boxes, classes, scores)
        
        maxRect = None
        maxSize = 0
        score = 0
        for index in range(len(classes)):
            if(classes[index] == 0):
                size = (boxes[index][2] - boxes[index][0]) * (boxes[index][3] - boxes[index][1])
                if(size > maxSize):
                    maxSize = size
                    maxRect = boxes[index]
                    score = scores[index]
        
        if(maxRect is not None):
            cv.rectangle(Frame, (maxRect[0], maxRect[1]), (maxRect[2], maxRect[3]), (0, 255, 0), 2)
            cv.putText(Frame, str(score), (maxRect[0], maxRect[1]), 
                    cv.FONT_HERSHEY_SIMPLEX, 1.0, (255, 0, 0), 2
                )
        else:
            return
        
        # Find focus.
        x = (maxRect[0] + maxRect[2]) / (2.0 * Frame.shape[1])
        y = (maxRect[1] + maxRect[3]) / (2.0 * Frame.shape[0])

        # Send coordinate.
        self.__protocol__.SendFloat("AppCenX", x)
        self.__protocol__.SendFloat("AppCenY", y)


    def __appleDetectLeft__(self, Frame : np.ndarray) -> None:
        boxes, classes, scores = self.__yoloDetector__.Predict(Frame)
        boxes, classes, scores = self.__boxesPostProcess__(boxes, classes, scores)
        
        leftRect = None
        leftMost = Frame.shape[1] * 2
        score = 0
        for index in range(len(classes)):
            if(classes[index] == 0):
                focus = boxes[index][0] + boxes[index][2]
                if(focus < leftMost):
                    leftMost = focus
                    leftRect = boxes[index]
                    score = scores[index]
        
        if(leftRect is not None):
            cv.rectangle(Frame, (leftRect[0], leftRect[1]), (leftRect[2], leftRect[3]), (0, 255, 0), 2)
            cv.putText(Frame, str(score), (leftRect[0], leftRect[1]), 
                    cv.FONT_HERSHEY_SIMPLEX, 1.0, (255, 0, 0), 2
                )
        else:
            return
        
        # Find focus.
        x = (leftRect[0] + leftRect[2]) / (2.0 * Frame.shape[1])
        y = (leftRect[1] + leftRect[3]) / (2.0 * Frame.shape[0])

        # Send coordinate.
        self.__protocol__.SendFloat("AppCenX", x)
        self.__protocol__.SendFloat("AppCenY", y)


    def __appleDetectRight__(self, Frame : np.ndarray) -> None:
        boxes, classes, scores = self.__yoloDetector__.Predict(Frame)
        boxes, classes, scores = self.__boxesPostProcess__(boxes, classes, scores)
        
        rightRect = None
        rightMost = 0
        score = 0
        for index in range(len(classes)):
            if(classes[index] == 0):
                focus = boxes[index][0] + boxes[index][2]
                if(focus > rightMost):
                    rightMost = focus
                    rightRect = boxes[index]
                    score = scores[index]
        
        if(rightRect is not None):
            cv.rectangle(Frame, (rightRect[0], rightRect[1]), (rightRect[2], rightRect[3]), (0, 255, 0), 2)
            cv.putText(Frame, str(score), (rightRect[0], rightRect[1]), 
                    cv.FONT_HERSHEY_SIMPLEX, 1.0, (255, 0, 0), 2
                )
        else:
            return
        
        # Find focus.
        x = (rightRect[0] + rightRect[2]) / (2.0 * Frame.shape[1])
        y = (rightRect[1] + rightRect[3]) / (2.0 * Frame.shape[0])

        # Send coordinate.
        self.__protocol__.SendFloat("AppCenX", x)
        self.__protocol__.SendFloat("AppCenY", y)


    def __targetDetection__(self, Frame : np.ndarray) -> None:
        results = self.__targetDetector__.Detect(Frame)
        if(results is not None):
            maxSize = 0
            maxTarget = None
            for result in results:
                size = result[1][0] * result[1][1]
                if(size > maxSize):
                    maxTarget = result
            
            if(maxTarget is not None):
                cv.drawContours(Frame, [np.int0(cv.boxPoints(maxTarget))], 0, (0, 255, 0), 3)

                # Send Result.
                x = maxTarget[0][0] / Frame.shape[1]
                y = maxTarget[0][1] / Frame.shape[0]
                self.__protocol__.SendFloat("TarCenX", x)
                self.__protocol__.SendFloat("TarCenY", y)


    def __fruitIdentify__(self, Frame : np.ndarray) -> None:
        '''
            @brief: Function of identifying fruits.
            
        '''
        boxes, classes, scores = self.__yoloDetector__.Predict(Frame)
        boxes, classes, scores = self.__boxesPostProcess__(boxes, classes, scores)
        if(boxes is not None):
            for index in range(len(boxes)):
                cv.rectangle(Frame, (boxes[index][0], boxes[index][1]), (boxes[index][2], boxes[index][3]), (255, 0, 0), 2)
                cv.putText(Frame, self.__rknnLables__[classes[index]], 
                    (boxes[index][0], boxes[index][1]), 
                    cv.FONT_HERSHEY_SIMPLEX, 1.0, (255, 0, 0), 2
                )

        # Process Result.
        AppleNum = 0
        BananaNum = 0
        KiwiFruitNum = 0
        LemonNum = 0
        OrangeNum = 0
        PeachNum = 0
        PearNum = 0
        PitayaNum = 0
        SnowPearNum = 0
        FruitDetectFinished = 1

        # Count.
        for cls in classes:
            if(cls == 0):
                AppleNum += 1
            elif(cls == 1):
                BananaNum += 1
            elif(cls == 2):
                KiwiFruitNum += 1
            elif(cls == 3):
                LemonNum += 1
            elif(cls == 4):
                OrangeNum += 1
            elif(cls == 5):
                PeachNum += 1
            elif(cls == 6):
                PearNum += 1
            elif(cls == 7):
                PitayaNum += 1
            elif(cls == 8):
                SnowPearNum += 1
            else:
                pass

        # Send Result.
        '''
            Type = uint8_t  
            | Key                   | Value              | Effect        |
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
        '''
        self.__protocol__.SendChar('AppleNum', AppleNum)
        self.__protocol__.SendChar('BananaNum', BananaNum)
        self.__protocol__.SendChar('KiwiFruitNum', KiwiFruitNum)
        self.__protocol__.SendChar('LemonNum', LemonNum)
        self.__protocol__.SendChar('OrangeNum', OrangeNum)
        self.__protocol__.SendChar('PeachNum', PeachNum)
        self.__protocol__.SendChar('PearNum', PearNum)
        self.__protocol__.SendChar('PitayaNum', PitayaNum)
        self.__protocol__.SendChar('SnowPearNum', SnowPearNum)
        self.__protocol__.SendChar('FruitDetectFinished', FruitDetectFinished)


    def __refreshFrame__(self) -> None:
        '''
            @brief: Refresh frame method when camera source is video stream.
        '''
        while(True):
            # Get frame from camera.
            ret, frame = self.__camera__.read()
            if(ret):
                with self.__frameLock__:
                    self.__frame__ = frame.copy()

            # exit method.
            exitFlag = False
            with self.__exitFlagLock__:
                exitFlag = self.__exitFlag__
            if(exitFlag):
                break

            time.sleep(0.02)


    def __mainMethod__(self) -> None:
        '''
            @brief: Main working method.
        '''
        # Initialize FrameBufferMonitor.
        self.__frameBufferMonitor__ = FrameBufferMonitor("/dev/fb0")
        #print(cv.imread("./Logo.jpg"))
        self.__frameBufferMonitor__.Display(cv.imread("./Logo.jpg"))


        # Initialize camera.
        self.__camera__ = cv.VideoCapture(self.__cameraID__)
        if(type(self.__cameraID__) is str):
            self.__frameLock__ = threading.Lock()
            self.__refreshFrameThread__ = threading.Thread(target = self.__refreshFrame__)
            self.__refreshFrameThread__.start()


        # Initialize YoloDetector. 
        while(self.__yoloDetector__ is None):
            try:
                self.__yoloDetector__ = LoadYoloV4_Tiny(
                    self.__rknnWeightPath__, self.__rknnLables__, self.__rknnWidth__, self.__rknnHeight__
                )
            except Exception as e:
                print("RKNN loading failed with message: " + str(e))


        # Initialize targetDetector.
        self.__targetDetector__ = TargetDetector()

        # Initialize protocol.
        self.__protocol__ = SerialProtocol(self.__serialPort__, self.__baudRate__)
        ## Add callback function.
        self.__protocol__.AddCallback("CMD", ProtocolVariablesType.string, self.__cmdCallback__)

        # Enter the main cycle.
        while(True):
            # Get frame from camera.
            frame = None
            if(type(self.__cameraID__) is int):
               ret, frame = self.__camera__.read() 
            elif(type(self.__cameraID__) is str):
                with self.__frameLock__:
                    frame = self.__frame__.copy()
            else:
                break


            # Copy working mode.
            modeStr = ""
            currentWorkingMode = None
            with self.__workingModeLock__:
                currentWorkingMode = self.__workingMode__

            if(frame is not None):
                # Image processing.
                startTime = time.time()
                
                # Enter working mode.
                if(currentWorkingMode == WorkingMode.StandBy):
                    modeStr = "StandBy"
                    self.__standBy__(frame)
                elif(currentWorkingMode == WorkingMode.AppleDetectMax):
                    modeStr = "AppleDetectMax"
                    self.__appleDetectMax__(frame)

                elif(currentWorkingMode == WorkingMode.AppleDetectLeft):
                    modeStr = "AppleDetectLeft"
                    self.__appleDetectLeft__(frame)

                elif(currentWorkingMode == WorkingMode.AppleDetectRight):
                    modeStr = "AppleDetectRight"
                    self.__appleDetectRight__(frame)

                elif(currentWorkingMode == WorkingMode.TargetDetection):
                    modeStr = "TargetDetection"
                    self.__targetDetection__(frame)

                elif(currentWorkingMode == WorkingMode.FruitIdentify):
                    modeStr = "FruitIdentify"
                    self.__fruitIdentify__(frame)

                endTime = time.time()

                # Calculate frame rate.
                if(self.__displayFPS):
                    fps = 1.0 / (endTime - startTime)
                    if(fps > 120.0):
                        fps = 120.0
                    print("FPS: {:.2f}".format(fps))
                    if(self.__displayOnFrameBuffer__):
                        cv.putText(frame, "FPS: {:.2f}".format(fps), (0, 30), cv.FONT_HERSHEY_SIMPLEX, 1.0, (0, 255, 0), 2)

                
                if(self.__displayOnFrameBuffer__):
                    # Display working mode.
                    if(self.__displayWorkingMode):
                        cv.putText(frame, "Mode: " + modeStr, (0, frame.shape[0] - 16), cv.FONT_HERSHEY_SIMPLEX, 1.0, (0, 0, 255), 2)

                    # Display frame.
                    self.__frameBufferMonitor__.Display(frame)

            else:
                print("Camera read failed.")
                time.sleep(0.1)

            # Synchronous working mode.
            '''
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
            '''
            self.__protocol__.SendChar("WM", currentWorkingMode.value)

            exitFlag = False
            with self.__exitFlagLock__:
                exitFlag = self.__exitFlag__
            if(exitFlag):
                break


    def Run(self, Block : bool = True) -> None:
        """
        
        """
        if(Block):
            self.__mainMethod__()
        else:
            self.__mainMethodThread__ = threading.Thread(target = self.__mainMethod__)
            self.__mainMethodThread__.start()


    def __del__(self):
        # Set exit flag.
        with self.__exitFlagLock__:
            self.__exitFlag__ = True

        # Wait for threads to exit.
        if(self.__mainMethodThread__ is not None):
            self.__mainThread__.join()

        if(self.__refreshFrameThread__ is not None):
            self.__refreshFrameThread__.join()
