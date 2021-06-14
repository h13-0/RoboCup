#include "SerialPrintf.h"

//Ä¬ÈÏUSART1
static USART_TypeDef *target = USART1;

#ifdef __GNUC__
int __io_putchar(int ch) {
	while (LL_USART_IsActiveFlag_TC(target) == RESET)
		;
	LL_USART_TransmitData8(target, (uint8_t) ch);
	return ch;
}
#else
int fputc(int ch, FILE *stream)
{
	while(LL_USART_IsActiveFlag_TC(target) == RESET);
	LL_USART_TransmitData8(target, (uint8_t)ch);
	return ch;
}
#endif

void SetSerialPrintfTarget(USART_TypeDef *USARTx)
{
	target = USARTx;
}
