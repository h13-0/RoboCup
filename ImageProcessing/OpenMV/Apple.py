import cv2

def detect():
    cascade_xpider = cv2.CascadeClassifier("./cascade.xml")
    cap = cv2.VideoCapture(0)
    while(True):
        ret, frame = cap.read()
        if(ret == False):
            break
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        xpiders = cascade_xpider.detectMultiScale(gray, 1.1, 3)
        for (x, y, w, h) in xpiders:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        
        cv2.imshow("Cascade", frame)
        cv2.waitKey(10)

if __name__ == "__main__":
    detect()