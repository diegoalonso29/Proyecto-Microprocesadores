#include "RAI.h"
#include "I2C_lib.h"

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
  /* Decrement the time out value */
  if (TimeOut != 0x0)
  {
    TimeOut--;
  }


  if (Counter < 10)
  {
    Counter++;
  }
  else
  {
    Counter = 0x00;
  }
}

void I2C1_ER_IRQHandler(void)
{
  if ((I2C_ReadRegister(I2Cx, I2C_Register_SR1) & 0xFF00) != 0x00)
  {
    /* Clears erreur flags */
    I2Cx->SR1 &= 0x00FF;
  }
}

void I2C1_EV_IRQHandler(void)
{
  /* Once the Start condition is sent the master can be master receiver
  or master transmitter */

  if (TRANSMISSION_MODE == I2C_Direction_Transmitter)
  {
	 I2C_ClearITPendingBit(I2Cx, I2C_IT_EVT);
    /* Get Last I2C Event */
    uint32_t Event = I2C_GetLastEvent(I2Cx);
    switch (Event)
    {
  /* ************************************************************************/
  /*                        Master Transmitter Events                       */
  /*                                                                        */
  /* ************************************************************************/
/* Sending the header sequence for Master Transmitter case----------------*/

    /* Check on EV5 */
    case I2C_EVENT_MASTER_MODE_SELECT :
      /* Send slave Address for write */
      I2C_Send7bitAddress(I2Cx, SLAVE_ADDRESS , I2C_Direction_Transmitter);
      break;

      /* Check on EV6 */
    case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
      /* Transmit the First Data  */
      I2C_SendData(I2Cx, TX_BUFFER[TX_BYTE_NUM++]);
      break;

      /* Check on EV8 */
    case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
    case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
      if (TX_BYTE_NUM == (uint8_t)NUM_TO_SEND)
      {
        /* Send STOP condition */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        I2C_ITConfig(I2Cx, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
      }
      else
      {
        /* Transmit Data TX_BUFFER */
        I2C_SendData(I2Cx, TX_BUFFER[TX_BYTE_NUM++]);
      }
      break;

    default:
      break;
    }

  }
  /*************************************************************************/
  /*                        Master Receiver Events                         */
  /*                                                                       */
  /*************************************************************************/
//  else /* MASTER_MODE_RECEIVER */
//  {
//    /* Check on EV5 */
//    if(I2C_GetITStatus(I2Cx, I2C_IT_SB)== SET)
//    {
//      /* Send slave Address for read */
//      I2C_Send7bitAddress(I2Cx, SLAVE_ADDRESS, I2C_Direction_Receiver);
//      if (NumberOfByteToReceive == 0x03)
//      {
//        /* Disable buffer Interrupts */
//        I2C_ITConfig(I2Cx, I2C_IT_BUF , DISABLE);
//      }
//      else
//      {
//        /* Enable buffer Interrupts */
//        I2C_ITConfig(I2Cx, I2C_IT_BUF , ENABLE);
//      }
//    }
//
//    else if(I2C_GetITStatus(I2Cx, I2C_IT_ADDR)== SET)
//    {
//      if (NumberOfByteToReceive == 1)
//      {
//        I2C_AcknowledgeConfig(I2Cx, DISABLE);
//      }
//      /* Clear ADDR Register */
//      (void)(I2Cx->SR1);
//      (void)(I2Cx->SR2);
//      if (GenerateStartStatus == 0x00)
//      {
//        if (NumberOfByteToReceive == 1)
//        {
//          I2C_GenerateSTOP(I2Cx, ENABLE);
//        }
//
//        if (NumberOfByteToReceive == 2)
//        {
//          I2C_AcknowledgeConfig(I2Cx, DISABLE);
//          I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);
//          /* Disable buffer Interrupts */
//          I2C_ITConfig(I2Cx, I2C_IT_BUF , DISABLE);
//        }
//      }
//
//    }
//    else if((I2C_GetITStatus(I2Cx, I2C_IT_RXNE)== SET)&&(I2C_GetITStatus(I2Cx, I2C_IT_BTF)== RESET))
//    {
//      /* Store I2C received data */
//      RX_BUFFER[RX_BYTE_NUM++] = I2C_ReceiveData (I2Cx);
//      NumberOfByteToReceive--;
//
//      if (NumberOfByteToReceive == 0x03)
//      {
//        /* Disable buffer Interrupts */
//        I2C_ITConfig(I2Cx, I2C_IT_BUF , DISABLE);
//      }
//
//      if (NumberOfByteToReceive == 0x00)
//      {
//        /* Disable Error and Buffer Interrupts */
//        I2C_ITConfig(I2Cx, (I2C_IT_EVT | I2C_IT_BUF), DISABLE);
//      }
//    }
//    /* BUSY, MSL and RXNE flags */
//    else if(I2C_GetITStatus(I2Cx, I2C_IT_BTF)== SET)
//    {
//      /* if Three bytes remaining for reception */
//      if (NumberOfByteToReceive == 3)
//      {
//        I2C_AcknowledgeConfig(I2Cx, DISABLE);
//        /* Store I2C received data */
//        RX_BUFFER[RX_BYTE_NUM++] = I2C_ReceiveData (I2Cx);
//        NumberOfByteToReceive--;
//      }
//      else if (NumberOfByteToReceive == 2)
//      {
//        I2C_GenerateSTOP(I2Cx, ENABLE);
//
//        /* Store I2C received data */
//        RX_BUFFER[RX_BYTE_NUM++] = I2C_ReceiveData (I2Cx);
//        NumberOfByteToReceive--;
//        /* Store I2C received data */
//        RX_BUFFER[RX_BYTE_NUM++] = I2C_ReceiveData (I2Cx);
//        NumberOfByteToReceive--;
//        /* Disable Error and Buffer Interrupts */
//        I2C_ITConfig(I2Cx, (I2C_IT_EVT | I2C_IT_BUF), DISABLE);
//      }
//      else
//      {
//        /* Store I2C received data */
//        RX_BUFFER[RX_BYTE_NUM++] = I2C_ReceiveData (I2Cx);
//        NumberOfByteToReceive--;
//      }
//    }
  }
