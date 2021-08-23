import sensor, image, time
from Protocol.Protocol import BinaryProtocol

def main(Use_LCD = False, LCD_Rotation = 2):
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.skip_frames(time = 2000)
    sensor.set_vflip(True)
    sensor.set_hmirror(True)
    clock = time.clock()

    protocol = BinaryProtocol(TX_Pin = 15, RX_Pin = 17, baudRate = 115200)

    if(Use_LCD):
        import lcd
        lcd.init(type=1)
        lcd.rotation(LCD_Rotation)
        lcd.clear(lcd.WHITE)

    while(True):
        clock.tick()
        img = sensor.snapshot()
        apple = (36, 71, -52, -9, -21, 39)
        appleBlobs = img.find_blobs([apple])

        maxAppleBlob = None
        maxAppleSize = 0
        for appleBlob in appleBlobs:
            size = appleBlob.area()
            if(size > maxAppleSize):
                maxAppleSize = size
                maxAppleBlob = appleBlob

        if((maxAppleBlob != None) and (maxAppleSize > 400)):
            img.draw_rectangle(maxAppleBlob.rect(), color=(0, 255, 0), thickness = 2)
            protocol.SendFloat("AppCenX:", (maxAppleBlob.x() + (maxAppleBlob.w() / 2)) / 320.0)
            protocol.SendFloat("AppCenY:", (maxAppleBlob.y() + (maxAppleBlob.h() / 2)) / 240.0)

        fps = clock.fps()
        if(Use_LCD):
            img.draw_string(0, 0, str(fps), color=(255, 255, 255))
            lcd.display(img)
        print(fps)

if(__name__ == "__main__"):
    main(Use_LCD = True, LCD_Rotation = 2)
