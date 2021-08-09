/*
 * SimpleProtocolPraise.h
 *
 *  Created on: 2021, 7, 7
 *      Author: h13
 */

#ifndef SRC_SIMPLEPROTOCOLPRAISE_SIMPLEPROTOCOLPRAISE_H_
#define SRC_SIMPLEPROTOCOLPRAISE_SIMPLEPROTOCOLPRAISE_H_
#include <stdint.h>
//#include <assert.h>
#include <string.h>
/**
 * @group: Configs
 */
//
#define MaxKeyLength    256
#define MaxBufferLength 256

//Apply Configs
#if(MaxKeyLength <= 256)
typedef uint8_t KeyIndex_t;
#elif(MaxKeyLength <= 65535)
typedef uint16_t KeyIndex_t;
#else
#error "MaxKeyLength is too large."
#endif

#if(MaxBufferLength <= 256)
typedef uint8_t BufferIndex_t;
#elif(MaxBufferLength <= 65535)
typedef uint16_t BufferIndex_t;
#else
#error "MaxBufferLength is too large."
#endif

/**
 * @
 * @note: Not Recommend.
 */
#define GeneratePraiseWithPrefixMethod(Byte, ConstPrefix, ConstPrefixLength, Buffer, ConstMaximumBufferLength, Callback)

#define PraiseWithPrefixAndStrictBufferLength(Byte, Prefix, PrefixLength, Buffer, BufferLength, Callback)

#define GeneratePraiseWithSuffixMethod(Byte, Suffix, SuffixLength, BufferPtr, MaximumBufferLength, DataLength, Callback) \
		{ \
			static BufferIndex_t bufferIndex = 0; \
			if(bufferIndex < MaximumBufferLength) \
			{ \
				*(BufferPtr + bufferIndex) = Byte; \
				bufferIndex ++; \
				if(bufferIndex >= SuffixLength) \
				{ \
					if(!memcmp((BufferPtr + bufferIndex - SuffixLength), Suffix, SuffixLength)) \
					{ \
						DataLength = bufferIndex; \
						Callback; \
						bufferIndex = 0; \
						memset(BufferPtr, '\0', MaximumBufferLength); \
					} \
				} \
			} else { \
				bufferIndex = 0; \
				memset(BufferPtr, '\0', MaximumBufferLength); \
			} \
		}


#define PraiseWithSuffixAndStrictBufferLength(Byte, Suffix, SuffixLength, Buffer, BufferLength, Callback)

#define PraiseWithPrefixAndSuffix(Byte, Prefix, PrefixLength, Suffix, SuffixLength, MaximumBufferLength)

#define GeneratePraiseWithPrefixAndSuffixAndStrictLengthMethod(Byte, Prefix, PrefixLength, Suffix, SuffixLength, BufferLength, Callback)

#endif /* SRC_SIMPLEPROTOCOLPRAISE_SIMPLEPROTOCOLPRAISE_H_ */
