import serial
import threading
import struct
from enum import Enum

class ProtocolVariablesType(Enum):
    char = 0
    int = 1
    float = 2
    string = 3

class SerialProtocol():
    def __init__(self, SerialPort : str, BaudRate : int):
        # Init serial port.
        self.__port__ = serial.Serial(SerialPort, BaudRate)

        # Init listen thread.
        self.__listenThread__ = threading.Thread(target = self.__listenMethod__)
        self.__listenThread__.setDaemon(False)
        self.__listenThread__.start()

        # Init exit flag.
        self.__exitFlag__ = False
        self.__exitFlagLock__ = threading.Lock()

        # Init callback list.
        self.__callbackList__ = []
        self.__callbackListLock__ = threading.Lock()


    def __listenMethod__(self) -> None:
        while(True):
            msg = self.__port__.readline()
            print(msg)
            if(len(msg)):
                callbackList = []
                with self.__callbackListLock__:
                    callbackList = self.__callbackList__

                # Traversal callback function.
                for callback in callbackList:
                    # Check if the key is matched.
                    if(callback[0][0 : callback[1] - 1] == msg[0 : callback[1] - 1]):
                        # Check if the type is matched.
                        if(callback[2] == ProtocolVariablesType.char):
                            try:
                                callback[3](int.from_bytes(msg[callback[1] + 1 : len(msg) - 2], "little"))
                            except:
                                pass

                        if(callback[2] == ProtocolVariablesType.int):
                            try:
                                callback[3](int.from_bytes(msg[callback[1] + 1 : len(msg) - 2], "little"))
                            except:
                                pass

                        elif(callback[2] == ProtocolVariablesType.float):
                            try:
                                callback[3](struct.unpack("<f", msg[callback[1] + 1 : len(msg) - 2]))
                            except:
                                pass

                        elif(callback[2] == ProtocolVariablesType.string):
                            try:
                                callback[3]((msg[callback[1] + 1 : len(msg) - 2]).decode('utf-8'))
                            except:
                                pass
            
            exitFlag = False
            with self.__exitFlagLock__:
                exitFlag = self.__exitFlag__
            if(exitFlag):
                break


    def Print(self, Msg : str) -> None:
        '''
            @brief: Print message through serial port using protocol.
            @param:
                @Msg: Message to be print.
        '''
        self.__port__.write(Msg.encode())


    def AddCallback(self, Key : str, Type : int, Callback) -> None:
        '''
            @brief: Add callback to callback list.
            @param:
                @Key: Key of the value.
                @Type: Type of the value.
                @Callback: Callback function such as function(Value : type).
        '''
        with self.__callbackListLock__:
            self.__callbackList__.append([Key.encode(), len(Key), Type, Callback])


    def SendChar(self, Key : str, Value : int) -> None:
        '''
            @brief: Send char through serial port using protocol.
            @param:
                @Key: Key of the value.
                @Value: Value to be send.
        '''
        Key += ':'
        self.__port__.write(Key.encode())
        msgStruct = struct.pack("<bbb", Value, 0x0D, 0x0A)
        self.__port__.write(msgStruct)


    def SendShort(self, Key : str, Value : int) -> None:
        '''
            @brief: Send short through serial port using protocol.
            @param:
                @Key: Key of the value.
                @Value: Value to be send.
        '''
        Key += ':'
        self.__port__.write(Key.encode())
        msgStruct = struct.pack("<hbb", Value, 0x0D, 0x0A)
        self.__port__.write(msgStruct)


    def SendInt(self, Key : str, Value : int) -> None:
        '''
            @brief: Send int through serial port using protocol.
            @param:
                @Key: Key of the value.
                @Value: Value to be send.
        '''
        Key += ':'
        self.__port__.write(Key.encode())
        msgStruct = struct.pack("<ibb", Value, 0x0D, 0x0A)
        self.__port__.write(msgStruct)


    def SendFloat(self, Key : str, Value : float) -> None:
        '''
            @brief: Send float through serial port using protocol.
            @param:
                @Key: Key of the value.
                @Value: Value to be send.
        '''
        Key += ':'
        self.__port__.write(Key.encode())
        msgStruct = struct.pack("<fbb", Value, 0x0D, 0x0A)
        self.__port__.write(msgStruct)
    

    def SendString(self, Key : str, Value : str) -> None:
        '''
            @brief: Send string through serial port using protocol.
            @param:
                @Key: Key of the value.
                @Value: Value to be send.
        '''
        Key += ':'
        msgStruct = struct.pack("<ssbb", Key, Value, 0x0D, 0x0A)
        self.__port__.write(msgStruct)


    def __del__(self):
        # Set exit flag.
        with self.__exitFlagLock__:
            exitFlag = True

        # wait for listen thread to exit.
        self.__listenThread__.join()
