import os

BaseDir = "E:/project/RoboCup/OpenMV/CascadeClassifier/Background"
TargetDir = "E:/project/RoboCup/OpenMV/CascadeClassifier/Background"

listFile = open(TargetDir + "/List.txt", mode='w', encoding='utf-8')

def main():
    # 检查是否是文件
    print(BaseDir)
    for files in os.listdir(BaseDir):
        if(os.path.isfile(BaseDir + "/" + files)):
            listFile.write(TargetDir + "/" + files + "\n")
    listFile.close()

if __name__ == "__main__":
    main()