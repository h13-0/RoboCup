class AppleDetector():
    def __init__(self, AppleThre = (36, 71, -52, -9, -21, 39)):
        self.__appleThre__ = AppleThre


    def Detect(self, img, MinimumSize = 400, NumberOfOutputTargets = 1):
        appleBlobs = img.find_blobs([self.__appleThre__])
        suitableAppleBlobs = []
        minimumSizeOfBlobs = MinimumSize
        for appleBlob in appleBlobs:
            size = appleBlob.area()
            if(size > minimumSizeOfBlobs):
                if(len(suitableAppleBlobs) < NumberOfOutputTargets):
                    suitableAppleBlobs.append(appleBlob)
                else:
                    minimumSizeOfBlobs = size
                    minimumID = 0
                    for suitableBlob in suitableAppleBlobs:
                        if(suitableBlob.area() < minimumSizeOfBlobs):
                            suitableAppleBlobs[minimumID] = appleBlob
                            break
                        minimumID += 1
        
        return suitableAppleBlobs
