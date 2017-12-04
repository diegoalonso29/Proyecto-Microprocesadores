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

void USART2_Init(uint32_t baudrate);
void USART_Send(USART_TypeDef* USARTx, uint8_t *s);
void USART_ReadByte(USART_TypeDef* USARTx, uint8_t* data);


#endif /* USART_LIB_H_ */
