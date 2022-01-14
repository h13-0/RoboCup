from fpioa_manager import fm
from machine import UART
import struct

class BinaryProtocol():
    def __init__(self, TX_Pin, RX_Pin, baudRate):
        fm.register(TX_Pin, fm.fpioa.UART2_TX)
        fm.register(RX_Pin, fm.fpioa.UART2_RX)
        self.__usart__ = UART(UART.UART2, baudRate, 8, None, 1, timeout = 1000, read_buf_len = 4096)
        self.__buf__ = bytearray('')


    def Print(self, stringData):
        self.__usart__.write(str(stringData).encode('utf-8'))


    #1byte
    def SendChar(self, key, charValue):
        msgStruct = struct.pack("<bbb", charValue, 0x0D, 0x0A)
        self.__usart__.write(key)
        self.__usart__.write(msgStruct)


    #2byte
    def SendShort(self, key, shortValue):
        msgStruct = struct.pack("<hbb", shortValue, 0x0D, 0x0A)
        self.__usart__.write(key)
        self.__usart__.write(msgStruct)


    #4byte
    def SendInt(self, key, intValue):
        msgStruct = struct.pack("<ibb", intValue, 0x0D, 0x0A)
        self.__usart__.write(key)
        self.__usart__.write(msgStruct)


    #4byte
    def SendFloat(self, key, floatValue):
        msgStruct = struct.pack("<fbb", floatValue, 0x0D, 0x0A)
        self.__usart__.write(key)
        self.__usart__.write(msgStruct)


    def GetNewPacket(self):
        data = self.__usart__.readline()
        if(data == None):
            return None
        else:
            self.__buf__.extend(data)

        #if(self.__buf__.find('\r\n')):
        if('\r\n' in self.__buf__):
            data = self.__buf__
            self.__buf__ = bytearray('')
            return data
        else:
            return None


    def GetLine(self, packet):
        try:
            stringBuf = packet.decode('utf-8')
        except Exception as e:
            return None
        return stringBuf


    def MatchChar(self, packet, key):
        keyLength = len(key)
        if((len(packet) == keyLength + 3) and (packet[0:keyLength] == key)):
            value = struct.unpack("b", packet[keyLength])[0]
            return value
        else:
            return None


    def MatchShort(self, packet, key):
        keyLength = len(key)
        if((len(packet) == keyLength + 4) and (packet[0:keyLength] == key)):
            value = struct.unpack("h", packet[keyLength:])[0]
            return value
        else:
            return None


    def MatchInt(self, packet, key):
        keyLength = len(key)
        if((len(packet) == keyLength + 5) and (packet[0:keyLength] == key)):
            value = struct.unpack("i", packet[keyLength:])[0]
            return value
        else:
            return None


    def MatchFloat(self, packet, key):
        keyLength = len(key)
        if((len(packet) == keyLength + 6) and (packet[0:keyLength] == key)):
            value = struct.unpack("f", packet[keyLength:])[0]
            return value
        else:
            return None
