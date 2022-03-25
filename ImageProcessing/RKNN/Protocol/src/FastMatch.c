#include <string.h>

/**
 * @brief:  Fast matching communication packet.
 * @param: 
 *      char* data:     binary packet data.
 *      int dataLength: length of binary packet.
 *      char* data:     binary key data.
 *      int keyLength:  length of key.
 * @return: 
 *      Effective data length.
 *      It will return -1 when data is invalid.
 */
int FastMatch(char* data, int dataLength, char* Key, int keyLength)
{
    if(memcmp(data, Key, keyLength) == 0)
    {
        return dataLength - keyLength;
    }
    else
    {
        return -1;
    }
}
