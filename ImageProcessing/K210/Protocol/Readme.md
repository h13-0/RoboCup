
# Usage
Copy Floder `` to Disk
```
from Protocol.Protocol import BinaryProtocol
import time

protocol = BinaryProtocol(115200)

while True:
    packet = protocol.GetNewPacket()
    while(packet != None):
        p = protocol.MatchFloat(packet, "AP:")
        if(p != None):
            print(p)
        packet = protocol.GetNewPacket()
    time.sleep(0.5)
```