#include "USART_Lib.h"
#include<stdio.h>
#include<math.h>

void USART2_Init(uint32_t baudrate)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	RX_Buffer_Counter = 0;
	Read_Counter = 0;

	pending_data = 0;
	buffer_count = 0;

	SENT = 0;
	OVERRUN = 0;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 |	GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode =	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =	GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_WordLength =	USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity =	USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode	= USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStruct);

	USART_Cmd(USART2,ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =	0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =	ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


void USART_Send(USART_TypeDef* USARTx, uint8_t *s)
{
	SENT = 0;
	TX_Data = s;
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

	while (!SENT);
}

void USART_ReadByte(USART_TypeDef* USARTx, uint8_t* data)
{
	pending_data--;
	if(Read_Counter == RX_Buffer_Lenght) Read_Counter = 0;
	*data = RX_Buffer[Read_Counter++];
}

//void USART_ReadBytes(USART_TypeDef* USARTx, uint8_t* data, uint8_t num)
//{
//	pending_data -= num;
//}
//
//void USART_ReadBuffer(USART_TypeDef* USARTx, uint8_t* data)
//{
//
//}

// C program for implementation of ftoa()
void USART_SendFloat(USART_TypeDef* USARTx, float f, int decimals)
{
	char str[40];
	//ftoa(f,str,2);

	int entera;
	int decimal;
	if(f>0)
	{
	entera = (int)f;
	decimal = (int)((f - (float)entera)*pow(10,decimals));
	}
	else
	{
	entera = (int)f;
	decimal = -(int)((f - (float)entera)*pow(10,decimals));
	}
	sprintf(str,"%d.%d",entera,decimal);
	USART_Send(USARTx, str);
	USART_Send(USARTx, "\n");
}

// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
