#include "I2C_lib.h"

#include "stm32l1xx_gpio.h"
#include "stm32l1xx_i2c.h"
/* Private variables */
static uint32_t I2C_Timeout;

/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

void I2C_InitConfig(){

	GPIO_InitTypeDef gpio_init_struct;
    I2C_InitTypeDef i2c_init_struct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_I2C, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C, ENABLE);

	/* Configuracion Pin8 (SCL) */
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType =GPIO_OType_OD;
	gpio_init_struct.GPIO_Pin= I2C_SCL_Pin;
	gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(I2C_GPIO,&gpio_init_struct);

	/*Configuracion Pin9 (SDA)*/
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType =GPIO_OType_OD;
	gpio_init_struct.GPIO_Pin= I2C_SDA_Pin;
	gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(I2C_GPIO,&gpio_init_struct);

	/*Asignamos la funcion alterna de GPIO_AF_I2C1 a ambos pines para realizar comunicacion*/
	GPIO_PinAFConfig(I2C_GPIO, I2C_SCL_PinSource, GPIO_AF_I2Cx);
	GPIO_PinAFConfig(I2C_GPIO, I2C_SDA_PinSource, GPIO_AF_I2Cx);

    /*Configuracion de la comunicacion I2C */
	i2c_init_struct.I2C_Ack = I2C_Ack_Disable;
	i2c_init_struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_init_struct.I2C_ClockSpeed = 100000;
	i2c_init_struct.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_init_struct.I2C_Mode = I2C_Mode_I2C;
	i2c_init_struct.I2C_OwnAddress1 = 0xDA;

	I2C_Cmd(I2C1,DISABLE);
    I2C_Init(I2C1, &i2c_init_struct);

    I2C_Cmd(I2C1,ENABLE);

}

uint8_t I2C_IsConnected(I2C_TypeDef* I2Cx, uint8_t SlaveAddress) {

	uint8_t connected = 0;
	/* Try to start, function will return 0 in case device will send ACK */
	if (!I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE)) {connected = 1;}

	I2C_Stop(I2Cx);
	return connected;
}

uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg) {

	uint8_t received_data;
	I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, reg);
	I2C_Stop(I2Cx);
	I2C_Start(I2Cx, SlaveAddress, I2C_RECEIVER_MODE, I2C_ACK_DISABLE);
	received_data = I2C_ReadNack(I2Cx);
	return received_data;
}

void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint16_t count)
{
	I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
	I2C_WriteData(I2Cx, reg);
	//I2C_Stop(I2Cx);
	I2C_Start(I2Cx, SlaveAddress, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	while (count--)
	{
		if (!count) {*data++ = I2C_ReadNack(I2Cx);}
		else {*data++ = I2C_ReadAck(I2Cx);}
	}
}

uint8_t I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t SlaveAddress)
{
	uint8_t data;
	I2C_Start(I2Cx, SlaveAddress, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	/* Also stop condition happens */
	data = I2C_ReadNack(I2Cx);
	return data;
}

void I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t* data, uint16_t count)
{
	I2C_Start(I2Cx, SlaveAddress, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	while (count--)
	{
		if (!count) {*data = I2C_ReadNack(I2Cx);}
		else {*data = I2C_ReadAck(I2Cx);}
	}
}

void I2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t data)
{
	I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, reg);
	I2C_WriteData(I2Cx, data);
	I2C_Stop(I2Cx);
}

void I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint16_t count)
{
	I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, reg);
	while (count--) {
		I2C_WriteData(I2Cx, *data++);
	}
	I2C_Stop(I2Cx);
}

void I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t data) {
	I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, data);
	I2C_Stop(I2Cx);
}

void I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t* data, uint16_t count)
{
	I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	while (count--)
	{
		I2C_WriteData(I2Cx, *data++);
	}
	I2C_Stop(I2Cx);
}

int16_t I2C_Start(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t direction, uint8_t ack)
{
	/* Generate I2C start pulse */
	I2Cx->CR1 |= I2C_CR1_START;

	/* Wait till I2C is busy */
	I2C_Timeout = I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_SB))
	{
		if (--I2C_Timeout == 0x00)
		{
			return 1;
		}
	}

	/* Enable ack if we select it */
	if (ack) {
		I2Cx->CR1 |= I2C_CR1_ACK;
	}

	/* Send write/read bit */
	if (direction == I2C_TRANSMITTER_MODE) {
		/* Send SlaveAddress with zero last bit */
		I2Cx->DR = SlaveAddress & ~I2C_OAR1_ADD0;

		/* Wait till finished */
		I2C_Timeout = I2C_TIMEOUT;
		while (!(I2Cx->SR1 & I2C_SR1_ADDR)) {
			if (--I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	if (direction == I2C_RECEIVER_MODE) {
		/* Send SlaveAddress with 1 last bit */
		I2Cx->DR = SlaveAddress | I2C_OAR1_ADD0;

		/* Wait till finished */
		I2C_Timeout = I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
			if (--I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}

	/* Read status register to clear ADDR flag */
	I2Cx->SR2;

	/* Return 0, everything ok */
	return 0;
}

void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data) {
	/* Wait till I2C is not busy anymore */
	I2C_Timeout = I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_TXE) && I2C_Timeout) {
		I2C_Timeout--;
	}

	/* Send I2C data */
	I2Cx->DR = data;
}

uint8_t I2C_ReadAck(I2C_TypeDef* I2Cx)
{
	uint8_t data;

	/* Enable ACK */
	I2Cx->CR1 |= I2C_CR1_ACK;

	/* Wait till not received */
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Read data */
	data = I2Cx->DR;

	/* Return data */
	return data;
}

uint8_t I2C_ReadNack(I2C_TypeDef* I2Cx) {
	uint8_t data;

	/* Disable ACK */
	I2Cx->CR1 &= ~I2C_CR1_ACK;

	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;

	/* Wait till received */
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Read data */
	data = I2Cx->DR;

	/* Return data */
	return data;
}

uint8_t I2C_Stop(I2C_TypeDef* I2Cx)
{
	/* Wait till transmitter not empty */
	I2C_Timeout = I2C_TIMEOUT;
	while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF)))) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;

	/* Return 0, everything ok */
	return 0;
}



/********************************************************************************/


//void I2C_WriteData(uint8_t SlaveAddress, uint8_t WriteAddressReg, uint8_t* Buffer_ptr,  uint16_t NumBytesToWrite)
//{
//    I2C_GenerateSTART(I2Cx, ENABLE);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
//
//    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//
//    I2C_SendData(I2Cx, WriteAddressReg);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//    I2C_SendData(I2Cx, *Buffer_ptr);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//    I2C_GenerateSTOP(I2Cx, ENABLE);
//}
//void I2C_WriteByte(uint8_t SlaveAddress, uint8_t WriteAddressReg, uint8_t* Buffer_ptr)
//{
//    I2C_GenerateSTART(I2Cx, ENABLE);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
//
//    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//
//    I2C_SendData(I2Cx, WriteAddressReg);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//    I2C_SendData(I2Cx, *Buffer_ptr);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//    I2C_GenerateSTOP(I2Cx, ENABLE);
//
//    while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY));
//}
//void I2C_WriteBits(uint8_t SlaveAddress, uint8_t WriteAddressReg, uint8_t BitStart, uint8_t length, uint8_t data)
//{
//    //      010 value to write
//    // 76543210 bit numbers
//    //    xxx   args: bitStart=4, length=3
//    // 00011100 mask byte
//	// 10101111 original value (sample)
//    // 10100011 original & ~mask
//    // 10101011 masked | value
//    uint8_t tmp;
//    I2C_ReadByte(SlaveAddress, WriteAddressReg, &tmp);
//    uint8_t mask = ((1 << length) - 1) << (BitStart - length + 1);
//    data <<= (BitStart - length + 1); // shift data into correct position
//    data &= mask; // zero all non-important bits in data
//    tmp &= ~(mask); // zero all important bits in existing byte
//    tmp |= data; // combine data with existing byte
//    I2C_WriteByte(SlaveAddress, WriteAddressReg, &tmp);
//}
//void I2C_WriteBit (uint8_t SlaveAddress, uint8_t WriteAddressReg, uint8_t BitNum, uint8_t data)
//{
//    uint8_t tmp;
//    I2C_ReadByte(SlaveAddress, WriteAddressReg, &tmp);
//    tmp = (data != 0) ? (tmp | (1 << BitNum)) : (tmp & ~(1 << BitNum));
//    I2C_WriteByte(SlaveAddress, WriteAddressReg, &tmp);
//}
//void I2C_ReadData(uint8_t SlaveAddress, uint8_t ReadAddressReg, uint8_t* Buffer_ptr,  uint16_t NumByteToRead)
//{
//    /* While the bus is busy */
//    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
//
//    /* Send START condition */
//    I2C_GenerateSTART(I2Cx, ENABLE);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
//
//    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//
//    I2C_Cmd(I2Cx, ENABLE);
//    I2C_SendData(I2Cx, ReadAddressReg);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//    I2C_GenerateSTART(I2Cx, ENABLE);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
//
//    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//
//    while (NumByteToRead)
//    {
//        if (NumByteToRead == 1)
//        {
//            I2C_AcknowledgeConfig(I2Cx, DISABLE);
//            I2C_GenerateSTOP(I2Cx, ENABLE);
//        }
//        if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
//        {
//            *Buffer_ptr = I2C_ReceiveData(I2Cx);
//            Buffer_ptr++;
//            NumByteToRead--;
//        }
//    }
//
//    I2C_AcknowledgeConfig(I2Cx, ENABLE);
//}
//void I2C_ReadByte(uint8_t SlaveAddress, uint8_t ReadAddressReg, uint8_t* Buffer_ptr)
//{
//
//    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
//
//    I2C_GenerateSTART(I2Cx, ENABLE);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
//
//    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//
//    I2C_Cmd(I2Cx, ENABLE);
//    I2C_SendData(I2Cx, ReadAddressReg);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//    I2C_GenerateSTART(I2Cx, ENABLE);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
//
//    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);
//    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//
//    I2C_AcknowledgeConfig(I2Cx, DISABLE);
//    I2C_GenerateSTOP(I2Cx, ENABLE);
//
//   if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
//      {
//            *Buffer_ptr = I2C_ReceiveData(I2Cx);
//      }
//
//    I2C_AcknowledgeConfig(I2Cx, ENABLE);
//}
//void I2C_ReadBits(uint8_t SlaveAddress, uint8_t ReadAddressReg, uint8_t BitStart, uint8_t length, uint8_t *data)
//{
//    // 01101001 read byte
//    // 76543210 bit numbers
//    //    xxx   args: bitStart=4, length=3
//    //    010   masked
//    //   -> 010 shifted
//    uint8_t tmp;
//    I2C_ReadByte(SlaveAddress, ReadAddressReg, &tmp);
//    uint8_t mask = ((1 << length) - 1) << (BitStart - length + 1);
//    tmp &= mask;
//    tmp >>= (BitStart - length + 1);
//    *data = tmp;
//}
//void I2C_ReadBit(uint8_t SlaveAddress, uint8_t ReadAddressReg, uint8_t BitNum, uint8_t *data)
//{
//    uint8_t tmp;
//    I2C_ReadByte(SlaveAddress, ReadAddressReg, &tmp);
//    *data = tmp & (1 << BitNum);
//}
//
//void Write_Byte(uint8_t SlaveAddress, uint8_t WriteAddressReg, uint8_t data)
//{
//TRANSMISSION_MODE = I2C_Direction_Transmitter;
//SLAVE_ADDRESS = SlaveAddress;
////uint8_t REG_ADDRESS = ;
//TX_BUFFER[0] = WriteAddressReg;
//TX_BUFFER[1] = data;
//TX_BYTE_NUM = 0;
//NUM_TO_SEND = 2;
//
//I2C_ITConfig(I2Cx, (I2C_IT_EVT | I2C_IT_BUF), ENABLE);
//I2C_GenerateSTART(I2Cx, ENABLE);
//
//TimeOut = USER_TIMEOUT;
//while ((TX_BYTE_NUM < NUM_TO_SEND)&&(TimeOut != 0x00));
//if(TimeOut == 0)
//{
//  TimeOut_UserCallback();
//}
//
//TimeOut = USER_TIMEOUT;
//while ((I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))&&(TimeOut != 0x00));
//
//if(TimeOut == 0)
//{
//  TimeOut_UserCallback();
//}
//
//}
//
//void TimeOut_UserCallback(void)
//{
//  /* User can add his own implementation to manage TimeOut Communication failure */
//  /* Block communication and all processes */
//  while (1)
//  {
//  }
//}
