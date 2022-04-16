from WorkingFlow.MainWorkingFlow import MainWorkingFlow

def main():
    workingFlow = MainWorkingFlow(0, "/dev/ttyS1", 115200)
    workingFlow.Run(True)


if(__name__ == "__main__"):
    main()

