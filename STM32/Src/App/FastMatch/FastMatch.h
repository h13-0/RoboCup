/*
 * FastMatch.h
 *
 *  Created on: 2021Äê6ÔÂ11ÈÕ
 *      Author: h13
 */

#ifndef APP_FASTMATCH_FASTMATCH_H_
#define APP_FASTMATCH_FASTMATCH_H_

/**
 * @brief: Try to match key with string. If matched, it will run your code **and return**.
 * @param:
 * 		string:         Source string.
 * 		stringLength:   Length of string.
 * 		key:            The string representation of key.
 * 		keyLength:      Length of key.
 *
 * @example:
 * 		matchKey(string, stringLength, "Enable Ble", 10)
 * 			EnableBluetooth();
 * 		endMatchKey()
 *
 * 		matchKey(string, stringLength, "Disable Ble", 11)
 * 			EnableBluetooth();
 * 		endMatchKey()
 *
 * @equals:
 * 		if(stringLength == 10)
 * 		{
 * 			if(!memcmp(string, "Enable Ble", 10))
 * 			{
 * 				EnableBluetooth();
 * 				return;
 * 			}
 * 		}
 *
 * 		if(stringLength == 11)
 * 		{
 * 			if(!memcmp(string, "Disable Ble", 11))
 * 			{
 * 				EnableBluetooth();
 * 				return;
 * 			}
 * 		}
 *
 * @usage:
 * 		example:
 * 			When input params are:
 * 				string="Enable Ble"
 * 				stringLength=10
 * 			Then the following functions are executed:
 * 				EnableBluetooth();
 */
#define matchKey(string, stringLength, key, keyLength) \
			if(stringLength == keyLength) \
			{ \
				if(!memcmp(string, key, keyLength)) \
				{

#define endMatchKey() return;}}



/**
 * @brief: Generic implementation for passing nonstandard and standard variables, such as struct and Float.
 */
#define matchKeyRaw(string, stringLength, key, keyLength, targetVariable, targetSize) \
			if(stringLength == keyLength + targetSize) \
			{ \
				if(!memcmp(string, key, keyLength)) \
				{ \
					string += keyLength; \
					char *target = (char *) &(targetVariable); \
					memcpy(target, string, targetSize); \
					return; \
				} \
			}

/**
 * @brief: Try to match key to Float. If matched, it will write data to your float variable **and return**.
 * @param:
 * 		string:         Source string.
 * 		stringLength:   Length of string.
 * 		key:            The string representation of key.
 * 		keyLength:      Length of key.
 * 		targetVariable: The float target variable.
 *
 * @usage:
 * 		if(length == 7)
 * 		{
 * 			matchKeyValue(data, length, "AP:", 3, AnglePID.proportion);
 * 			matchKeyValue(data, length, "TA:", 3, AnglePID.setpoint);
 * 		}
 *
 * @equals:
 * 		if(length == 7)
 * 		{
 * 			if (!memcmp(data, "AP:", 3))
 * 			{
 * 				data += 3;
 * 				char *target = (char*) &(AnglePID.proportion);
 * 				memcpy(target, data, 4);
 * 				return;
 * 			}
 *
 * 			if (!memcmp(data, "TA:", 3))
 *      	{
 *      		data += 3;
 *      		char *target = (char*) &(AnglePID.setpoint);
 *      		memcpy(target, data, 4);
 *      		return;
 *      	}
 *      }
 *
 * @note:
 * 		**The targetVariable must be float!!!**
 * 		**To optimize efficiency, you must first manually check the string length!!!**
 *
 */
/*
#define matchKeyValue(string, stringLength, key, keyLength, targetVariable) \
	if (!memcmp(string, key, keyLength)) \
	{ \
		string += keyLength; \
		char *target = (char *) &(targetVariable); \
		memcpy(target, string, 4); \
		return; \
	}
*/

#define matchKeyFloat(string, stringLength, key, keyLength, targetFloat) matchKeyRaw(string, stringLength, key, keyLength, targetFloat, 4)

#define matchKeyDouble(string, stringLength, key, keyLength, targetDouble) matchKeyRaw(string, stringLength, key, keyLength, targetDouble, 8)

#define matchKeyInt8_t(string, stringLength, key, keyLength, targetInt8_t) matchKeyRaw(string, stringLength, key, keyLength, targetInt8_t, 1)

#define matchKeyInt16_t(string, stringLength, key, keyLength, targetInt16_t) matchKeyRaw(string, stringLength, key, keyLength, targetInt16_t, 2)

#define matchKeyInt32_t(string, stringLength, key, keyLength, targetInt32_t) matchKeyRaw(string, stringLength, key, keyLength, targetInt32_t, 3)

#define matchKeyInt64_t(string, stringLength, key, keyLength, targetInt64_t) matchKeyRaw(string, stringLength, key, keyLength, targetInt64_t, 4)

#define matchKeyString(string, stringLength, key, keyLength, targetString, targetLength) \
			if(stringLength >= keyLength) \
			{ \
				if(!memcmp(string, key, keyLength)) \
				{ \
					memset(targetString, '0', targetLength); \
					string += keyLength; \
					stringLength -= keyLength; \
					if(stringLength <= targetLength) \
					{ \
						memcpy(targetString, string, stringLength); \
					} else { \
						memcpy(targetString, string, targetLength); \
					} \
					return; \
				} \
			}


#endif /* APP_FASTMATCH_FASTMATCH_H_ */
