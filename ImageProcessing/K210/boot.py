from Protocol.Protocol import BinaryProtocol
import sensor, lcd
from Detector.AppleDetector import AppleDetector

class WorkingMode():
    Standby = 0
    AppleDetectLeft  = 1
    AppleDetectRight = 2
    AppleDetectMax   = 3
    TargetDetect     = 4
    FruitDetection   = 5


class MainWorkflow():
    def __init__(self, TX_Pin, RX_Pin, baudRate = 115200, vflip = False, hmirror = False, Use_LCD = False, LCD_Rotation = 2, DisplayFPS = False, DisplayMode = False):
        # Init LCD Configure
        self.__useLCD__ = Use_LCD
        self.__displayMode__ = DisplayMode
        self.__displayFPS__ = DisplayFPS
        if(Use_LCD):
            self.__lcd__ = lcd
            self.__lcd__.init(type=1)
            self.__lcd__.rotation(LCD_Rotation)
            self.__lcd__.clear(lcd.WHITE)
        # TODO: Display logo

        
        # Init Binary Protocol
        self.__protocol__ = BinaryProtocol(TX_Pin = TX_Pin, RX_Pin = RX_Pin, baudRate = 115200)
        
        # Init mode selection
        self.__currentMode__ = WorkingMode.Standby
        self.__lastMode__ = WorkingMode.Standby

        # Init sensor
        self.__sensor__ = sensor
        self.__sensor__.reset()
        self.__sensor__.set_pixformat(sensor.RGB565)
        self.__sensor__.set_framesize(sensor.QVGA)
        self.__sensor__.set_vflip(vflip)
        self.__sensor__.set_hmirror(hmirror)
        self.__sensor__.skip_frames(time = 500)
        #self.__sensor__.run(1)

        # Init detector variables
        self.__appleDetector__ = None
        self.__targetDetector__ = None
        self.__fruitDetector__ = None
    

    def ReportWoringMode(self):
        self.__protocol__.SendChar("WM:", self.__currentMode__)


    def __appleDetectorInit__(self):
        self.__appleDetector__ = AppleDetector()

    def __appleDetectorRelease__(self):
        pass

    def Run(self):
        # Check Working Mode
        packet = self.__protocol__.GetNewPacket()
        while(packet != None):
            msg = self.__protocol__.GetLine(packet)

            if(msg == "CMD:AppleDetectLeft\r\n"):
                self.__currentMode__ = WorkingMode.AppleDetectLeft
            elif(msg == "CMD:AppleDetectRight\r\n"):
                self.__currentMode__ = WorkingMode.AppleDetectRight
            elif(msg == "CMD:AppleDetectMax\r\n"):
                self.__currentMode__ = WorkingMode.AppleDetectMax
            elif(msg == "CMD:TargetDetect\r\n"):
                self.__currentMode__ = WorkingMode.TargetDetect
            elif(msg == "CMD:FruitDetect\r\n"):
                self.__currentMode__ = WorkingMode.FruitDetection

            packet = self.__protocol__.GetNewPacket()
    
        self.ReportWoringMode()

        # Switching working mode
        if(self.__lastMode__ != self.__currentMode__):
            # Memory recall
            if(self.__lastMode__ == WorkingMode.AppleDetectLeft): 
                self.__appleDetectorRelease__()

            # Init new method
            ## Init apple detector
            if((self.__currentMode__ == WorkingMode.AppleDetectMax) or (self.__currentMode__ == WorkingMode.AppleDetectLeft) or (self.__currentMode__ == WorkingMode.AppleDetectRight)):
                if(self.__appleDetector__ == None):
                    self.__appleDetectorInit__()

            ## Init furit detector
            if(self.__currentMode__ == WorkingMode.FruitDetection):
                if(self.__useLCD__):
                    self.__lcd__.clear(lcd.BLACK)
                self.__sensor__.set_windowing((224, 224))
                

            ## Init target detector

            # Update lastMode
            self.__lastMode__ = self.__currentMode__
        
        # Run the detection
        img = self.__sensor__.snapshot()

        # AppleDetectMax
        if(self.__currentMode__ == WorkingMode.AppleDetectMax):
            results = self.__appleDetector__.Detect(img, NumberOfOutputTargets = 1)
            for result in results:
                img.draw_rectangle(result.rect(), color=(0, 255, 0), thickness = 2)
                self.__protocol__.SendFloat("AppCenX:", (result.x() + (result.w() / 2)) / 320.0)
                self.__protocol__.SendFloat("AppCenY:", (result.y() + (result.h() / 2)) / 240.0)

        # AppleDetectLeft or AppleDetectRight
        elif((self.__currentMode__ == WorkingMode.AppleDetectLeft) or (self.__currentMode__ == WorkingMode.AppleDetectRight)):
            results = self.__appleDetector__.Detect(img, NumberOfOutputTargets = 2)
            if(len(results) == 1):
                img.draw_rectangle(results[0].rect(), color=(0, 255, 0), thickness = 2)
                self.__protocol__.SendFloat("AppCenX:", (results[0].x() + (results[0].w() / 2)) / 320.0)
                self.__protocol__.SendFloat("AppCenY:", (results[0].y() + (results[0].h() / 2)) / 240.0)
            elif(len(results) == 2):
                appleCenterX = []
                appleCenterY = []
                for result in results:
                    appleCenterX.append((result.x() + (result.w() / 2)) / 320.0)
                    appleCenterY.append((result.y() + (result.h() / 2)) / 240.0)

                if(self.__currentMode__ == WorkingMode.AppleDetectLeft):
                    if(appleCenterX[0] < appleCenterX[1]):
                        img.draw_rectangle(results[0].rect(), color=(0, 255, 0), thickness = 2)
                        img.draw_rectangle(results[1].rect(), color=(255, 0, 0), thickness = 2)
                        self.__protocol__.SendFloat("AppCenX:", appleCenterX[0])
                        self.__protocol__.SendFloat("AppCenY:", appleCenterY[0])
                    else:
                        img.draw_rectangle(results[1].rect(), color=(0, 255, 0), thickness = 2)
                        img.draw_rectangle(results[0].rect(), color=(255, 0, 0), thickness = 2)
                        self.__protocol__.SendFloat("AppCenX:", appleCenterX[1])
                        self.__protocol__.SendFloat("AppCenY:", appleCenterY[1])
                else:
                    if(appleCenterX[0] < appleCenterX[1]):
                        img.draw_rectangle(results[1].rect(), color=(0, 255, 0), thickness = 2)
                        img.draw_rectangle(results[0].rect(), color=(255, 0, 0), thickness = 2)
                        self.__protocol__.SendFloat("AppCenX:", appleCenterX[1])
                        self.__protocol__.SendFloat("AppCenY:", appleCenterY[1])
                    else:
                        img.draw_rectangle(results[0].rect(), color=(0, 255, 0), thickness = 2)
                        img.draw_rectangle(results[1].rect(), color=(255, 0, 0), thickness = 2)
                        self.__protocol__.SendFloat("AppCenX:", appleCenterX[0])
                        self.__protocol__.SendFloat("AppCenY:", appleCenterY[0])

        # Display mode
        if(self.__displayMode__):
            if(self.__currentMode__ == WorkingMode.Standby):
                img.draw_string(0, 224, "Standby", color=(255, 255, 255))
            elif(self.__currentMode__ == WorkingMode.AppleDetectMax):
                img.draw_string(0, 224, "AppleMax", color=(255, 255, 255))
            elif(self.__currentMode__ == WorkingMode.AppleDetectLeft):
                img.draw_string(0, 224, "AppleDetectLeft", color=(255, 255, 255))
            elif(self.__currentMode__ == WorkingMode.AppleDetectRight):
                img.draw_string(0, 224, "AppleDetectRight", color=(255, 255, 255))

        # Display FPS
        if(self.__displayFPS__):
            img.draw_string(0, 0, str(fps), color=(255, 255, 255))

        # Display or not.
        if(self.__useLCD__):
            lcd.display(img)

        self.ReportWoringMode()


if(__name__ == "__main__"):
    app = MainWorkflow(TX_Pin = 15, RX_Pin = 17, vflip = True, hmirror = True, Use_LCD = True, DisplayMode = True)
    while(True):
        app.Run()
