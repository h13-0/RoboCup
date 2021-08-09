import os
import cv2 as cv

BaseDir = "E:/project/RoboCup/OpenMV/CascadeClassifier/Source/Apple"
TargetDir = "E:/project/RoboCup/OpenMV/CascadeClassifier/Resized/Apple"

listFile = open(TargetDir + "/List.txt", mode='w', encoding='utf-8')

def main():
    pictureID = 0
    # 检查是否是文件
    for files in os.listdir(BaseDir):
        if(os.path.isfile(BaseDir + "/" + files)):
            src = cv.imread(BaseDir + "/" + files)
            dst = cv.resize(src, (32, 32))
            cv.imwrite(TargetDir + "/Apple" + str(pictureID) + ".jpg", dst)
            listFile.write("Apple" + str(pictureID) + ".jpg 1 0 0 32 32\n")
            pictureID += 1

if __name__ == "__main__":
    main()