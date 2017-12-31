#include "stm32l1xx.h"

#ifndef USART_LIB_H_
#define USART_LIB_H_

#define RX_Buffer_Lenght 20


uint8_t* TX_Data;

uint8_t RX_Buffer[RX_Buffer_Lenght];
uint8_t RX_Buffer_Counter;
uint8_t Read_Counter;

uint8_t pending_data;
uint8_t buffer_count;

uint8_t SENT;
uint8_t OVERRUN;

#define MAX_PRECISION	(10)
static const double rounders[MAX_PRECISION + 1] =
{
	0.5,				// 0
	0.05,				// 1
	0.005,				// 2
	0.0005,				// 3
	0.00005,			// 4
	0.000005,			// 5
	0.0000005,			// 6
	0.00000005,			// 7
	0.000000005,		// 8
	0.0000000005,		// 9
	0.00000000005		// 10
};

void USART2_Init(uint32_t baudrate);
void USART_Send(USART_TypeDef* USARTx, uint8_t *s);
void USART_ReadByte(USART_TypeDef* USARTx, uint8_t* data);
void USART_SendFloat(USART_TypeDef* USARTx, float f, int decimals);
void USART2_IRQHandler(void);
char * ftoa(double f, char * buf, int precision);

#endif /* USART_LIB_H_ */
