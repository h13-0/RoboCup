import cv2 as cv
from cv2 import minEnclosingCircle
import numpy as np
from typing import List, Tuple

class TargetDetector():
    def __init__(self,
        CentralThreshold : List = [(240, 95, 80), (255, 255, 255)],
        EdgeThreshold : List = [(140, 0, 90), (167, 50, 255)],
        MinimumCentralSize : float = 2000.0,
        MinimumCentralLengthWidthRatio : float = 0.4,
        MinimumEdgeSize = 5000.0,
        MinimumEdgeLengthWidthRatio : float = 0.4,
        MinimumRectOverlapRate : float = 0.5
    ) -> None:
        '''
            @brief: Init TargetDetector.
            @param:
                - CentralThreshold: The HSV_FULL threshold of central target.
                - EdgeThreshold: The HSV_FULL threshold of edge target.
                - MinimumCentralSize: The minimum size of central target.
                - MinimumCentralLengthWidthRatio: The minimum length/width ratio of central target.
                - MinimumEdgeSize: The minimum size of edge target.
                - MinimumEdgeLengthWidthRatio: The minimum length/width ratio of edge target.
        '''
        self.__centralThreshold__ = CentralThreshold
        self.__edgeThreshole__ = EdgeThreshold
        self.__minimumCentralSize__ = MinimumCentralSize
        self.__minimumCentralLengthWidthRatio__ = MinimumCentralLengthWidthRatio
        self.__minimumEdgeSize__ = MinimumEdgeSize
        self.__minimumEdgeLengthWidthRatio__ = MinimumEdgeLengthWidthRatio
        self.__minimumRectOverlapRate__ = MinimumRectOverlapRate


    def Detect(self, BRG_Image : np.ndarray):
        '''
            @brief: Detect Target.
            @param:
                - BRG_Image: The input RGB image.
            @return:

        '''
        # Convert BGR to HSV_FULL.
        frame = cv.cvtColor(BRG_Image, cv.COLOR_BGR2HSV_FULL)

        # Find Edge Part.
        mask = cv.inRange(frame, self.__edgeThreshole__[0], self.__edgeThreshole__[1])

        ## Erode.
        mask = cv.erode(mask, np.ones((5 ,5), np.uint8), iterations = 1)

        ## Find countours.
        contours, hierarchy = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

        ## Filter illegal rects.
        edgeRects = []
        for contour in contours:
            if(cv.contourArea(contour) > self.__minimumEdgeSize__):
                minRect = cv.minAreaRect(contour)
                lengthWidthRatio = (minRect[1][0] / minRect[1][1]) if (minRect[1][0] < minRect[1][1]) else (minRect[1][1] / minRect[1][0])
                if(lengthWidthRatio > self.__minimumEdgeLengthWidthRatio__):
                    edgeRects.append(minRect)

        # Find Central Part.
        mask = cv.inRange(frame, self.__centralThreshold__[0], self.__centralThreshold__[1])
        
        ## Erode.
        mask = cv.erode(mask, np.ones((5 ,5), np.uint8), iterations = 1)

        ## Find countours.
        contours, hierarchy = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

        ## Filter illegal rects.
        centralRects = []
        for contour in contours:
            if(cv.contourArea(contour) > self.__minimumCentralSize__):
                minRect = cv.minAreaRect(contour)
                lengthWidthRatio = (minRect[1][0] / minRect[1][1]) if (minRect[1][0] < minRect[1][1]) else (minRect[1][1] / minRect[1][0])
                if(lengthWidthRatio > self.__minimumCentralLengthWidthRatio__):
                    centralRects.append(minRect)

        # Find best result.
        results = []
        if(len(edgeRects) > 0 and len(centralRects) > 0):
            for centralRect in centralRects:
                for edgeRect in edgeRects:
                    overlapRate = self.__calcBoxesOverlapRate__(cv.boundingRect(cv.boxPoints(centralRect)), cv.boundingRect(cv.boxPoints(edgeRect)))
                    if(overlapRate > self.__minimumRectOverlapRate__):
                        results.append(centralRect)

            return results
        elif(len(centralRects) > 0):
            return centralRects


    def __calcBoxesOverlapRate__(self, box1 : cv.boundingRect, box2 : cv.boundingRect) -> float:
        minSize = min(
            box1[2] * box1[3],
            box2[2] * box2[3]
        )

        minRect = [
            # Left
            max(box1[0], box2[0]),
            # Top
            max(box1[1], box2[1]),
            # Right
            min(box1[0] + box1[2], box2[0] + box2[2]),
            # Buttom
            min(box1[1] + box1[3], box2[1] + box2[3])
        ]
        if(minSize > 0 and minRect[2] > minRect[0] and minRect[3] > minRect[1]):
            return (minRect[2] * minRect[3]) / minSize
        else:
            return 0


if(__name__ == "__main__"):
    detector = TargetDetector()
    cap = cv.VideoCapture(0)

    while(True):
        ret, frame = cap.read()
        if(ret):
            #print(frame.shape)
            results = detector.Detect(frame)
            if(results is not None):
                for result in results:
                    cv.drawContours(frame, [np.int0(cv.boxPoints(result))], 0, (0, 255, 0), 3)
                    #cv.rectangle(frame, cv.boundingRect(cv.boxPoints(result)), (0, 255, 0), 2)
            cv.imshow("frame", frame)
            if(cv.waitKey(1) & 0xFF == ord('q')):
                break
