#include "RAI.h"
#include "USART_Lib.h"
#include "main.h"


void SysTick_Handler(void)
{
	TimingDelay_Decrement();
	counter++;
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






