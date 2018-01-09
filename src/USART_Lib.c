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


void USART_SendFloat(USART_TypeDef* USARTx, float f, int decimals)
{
	char str[16];
	USART_Send(USART2, ftoa(f, str, decimals));

}


void USART2_IRQHandler(void)
{
	if( USART_GetITStatus(USART2, USART_IT_RXNE) )
	{
		uint8_t DR = USART2->DR;

		/* If the RX buffer is full, it doesn´t save a new byte until it´s free*/
		if(pending_data < RX_Buffer_Lenght)
		{
			OVERRUN = 0;

			/* Resets the Index ( RX_Buffer_Counter ) of the Buffer if it overflows */
			if(RX_Buffer_Counter >= RX_Buffer_Lenght)
			{
				RX_Buffer_Counter = 0;
			}
			/* Saves into the RX_Buffer the last byte received */
			RX_Buffer[RX_Buffer_Counter++] = DR;
			pending_data++;
		}
		else { OVERRUN = 1; }
	}


	if( USART_GetITStatus(USART2, USART_IT_TXE) )
	{
		if(*TX_Data)
		{
		USART_SendData(USART2, *TX_Data);
		TX_Data++;
		}
		else
		{
		SENT = 1;
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
	}
}

char * ftoa(double f, char * buf, int precision)
{
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;

	// check precision bounds
	if (precision > MAX_PRECISION)
		precision = MAX_PRECISION;

	// sign stuff
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}

	if (precision < 0)  // negative precision == automatic precision guess
	{
		if (f < 1.0) precision = 6;
		else if (f < 10.0) precision = 5;
		else if (f < 100.0) precision = 4;
		else if (f < 1000.0) precision = 3;
		else if (f < 10000.0) precision = 2;
		else if (f < 100000.0) precision = 1;
		else precision = 0;
	}

	// round value according the precision
	if (precision)
		f += rounders[precision];

	// integer part...
	intPart = f;
	f -= intPart;

	if (!intPart)
		*ptr++ = '0';
	else
	{
		// save start pointer
		p = ptr;

		// convert (reverse order)
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}

		// save end pos
		p1 = p;

		// reverse result
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		// restore end pos
		ptr = p1;
	}

	// decimal part
	if (precision)
	{
		// place decimal point
		*ptr++ = '.';

		// convert
		while (precision--)
		{
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}

	// terminating zero
	*ptr = 0;

	return buf;
}
