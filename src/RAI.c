#include "RAI.h"
#include "USART_Lib.h"
#include "main.h"
//extern uint8_t TX_BUFFER[];
//__IO uint8_t Counter = 0x00;
//extern __IO uint32_t TimeOut;
//__IO uint32_t Event = 0x00;

//uint8_t HEADER_ADDRESS_Write = (((SLAVE_ADDRESS & 0xFF00) >> 7) | 0xF0);
//uint8_t HEADER_ADDRESS_Read = (((SLAVE_ADDRESS & 0xFF00) >> 7) | 0xF1);
//
//extern  __IO uint8_t TX_BYTE_NUM;
//extern  __IO uint8_t RX_BYTE_NUM;
//extern  uint8_t NumberOfByteToReceive;
//extern  uint8_t NumberOfByteToTransmit;
//__IO uint8_t GenerateStartStatus = 0x00;
//extern  uint8_t RX_BUFFER[];
//#ifdef I2C_10BITS_ADDRESS
//__IO uint8_t Send_HeaderStatus = 0x00;
//#endif /* I2C_10BITS_ADDRESS */
//extern __IO uint8_t MasterMode;


//extern uint8_t TRANSMISSION_MODE;
//extern uint8_t SLAVE_ADDRESS;
//extern uint8_t REG_ADDRESS;
//extern uint8_t TX_BUFFER[];
//extern uint8_t RX_BUFFER[];
//extern uint8_t TX_BYTE_NUM;
//extern uint8_t RX_BYTE_NUM;
//extern uint8_t NUM_TO_SEND;
//extern uint8_t NUM_TO_READ;

void SysTick_Handler(void)
{
	TimingDelay_Decrement();
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






