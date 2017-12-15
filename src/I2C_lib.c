#include "I2C_lib.h"

static uint32_t I2C_Timeout;


/*
* Initialization of I2Cx comunication (Default 100000KHz, 7bit Adress, NOPULL resistors)
* Params :
* 				I2Cx 			-> I2C peripheral selected
* Return :
*				void
*/

void I2C_InitConfig(I2C_TypeDef* I2Cx)
{
	GPIO_InitTypeDef gpio_init_struct;
    I2C_InitTypeDef i2c_init_struct;

    if(I2Cx != I2C2)
    {
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		/* Configuracion Pin8 (SCL) */
		gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
		gpio_init_struct.GPIO_OType =GPIO_OType_OD;
		gpio_init_struct.GPIO_Pin= GPIO_Pin_8;
		gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
		gpio_init_struct.GPIO_Speed=GPIO_Speed_2MHz;
		GPIO_Init(GPIOB,&gpio_init_struct);

		/*Configuracion Pin9 (SDA)*/
		gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
		gpio_init_struct.GPIO_OType =GPIO_OType_OD;
		gpio_init_struct.GPIO_Pin= GPIO_Pin_9;
		gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
		gpio_init_struct.GPIO_Speed=GPIO_Speed_2MHz;
		GPIO_Init(GPIOB,&gpio_init_struct);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

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


    else
    {
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

		/* Configuracion Pin8 (SCL) */
		gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
		gpio_init_struct.GPIO_OType =GPIO_OType_OD;
		gpio_init_struct.GPIO_Pin= GPIO_Pin_10;
		gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
		gpio_init_struct.GPIO_Speed=GPIO_Speed_2MHz;
		GPIO_Init(GPIOB,&gpio_init_struct);

		/*Configuracion Pin9 (SDA)*/
		gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
		gpio_init_struct.GPIO_OType =GPIO_OType_OD;
		gpio_init_struct.GPIO_Pin= GPIO_Pin_11;
		gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
		gpio_init_struct.GPIO_Speed=GPIO_Speed_2MHz;
		GPIO_Init(GPIOB,&gpio_init_struct);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);

		/*Configuracion de la comunicacion I2C */
		i2c_init_struct.I2C_Ack = I2C_Ack_Disable;
		i2c_init_struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		i2c_init_struct.I2C_ClockSpeed = 100000;
		i2c_init_struct.I2C_DutyCycle = I2C_DutyCycle_2;
		i2c_init_struct.I2C_Mode = I2C_Mode_I2C;
		i2c_init_struct.I2C_OwnAddress1 = 0xDA;

		I2C_Cmd(I2C2,DISABLE);
		I2C_Init(I2C2, &i2c_init_struct);

		I2C_Cmd(I2C2,ENABLE);
    }
}

/*
* Checks if the I2C device is connected and comunication is okay
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/

I2C_Error_Code I2C_IsConnected(I2C_TypeDef* I2Cx, uint8_t SlaveAddress) {

	I2C_Error_Code status;
	/* Try to start, function will return I2C_NoError in case device will send ACK */
	status = I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
	if (status) {return status;}

	I2C_Stop(I2Cx);
	return I2C_NoError;
}

/*
* Reads one byte from I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				data			-> Direction to store the byte data
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/

I2C_Error_Code I2C_ReadByte(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t* data)
{
	I2C_Error_Code status;

	status = I2C_Start(I2Cx, SlaveAddress, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	if (status) {return status;}

	/* Also stop condition happens */
	status = I2C_ReadNack(I2Cx, data);
	if (status) {return status;}

	return I2C_NoError;
}

/*
* Reads several bytes (count) from I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				data			-> Direction to store the byte data
*				count			-> Number of bytes to read
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t* data, uint16_t count)
{
	I2C_Error_Code status;

	status = I2C_Start(I2Cx, SlaveAddress, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	if (status) {return status;}

	while (count--)
	{
		if (!count)
		{
			status = I2C_ReadNack(I2Cx, data);
			if (status) {return status;}
		}
		else
		{
			status = I2C_ReadAck(I2Cx, data);
			if (status) {return status;}
		}
	}

	return I2C_NoError;
}

/*
* Writes one byte to I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				data			-> Byte to transmit to the slave
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t data)
{
	I2C_Error_Code status;

	status = I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	if (status) {return status;}

	status = I2C_Write(I2Cx, data);
	if (status) {return status;}

	status = I2C_Stop(I2Cx);
	if (status) {return status;}

	return I2C_NoError;
}

/*
* Writes several bytes to I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				data			-> Direction of the first position of data array
*				count			-> Number of bytes to transmit
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t* data, uint16_t count)
{
	I2C_Error_Code status;

	status = I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	if (status) {return status;}

	while (count--)
	{
		status = I2C_Write(I2Cx, *data++);
		if (status) {return status;}
	}

	status = I2C_Stop(I2Cx);
	if (status) {return status;}

	return I2C_NoError;
}

/*
* Reads one byte from the Register (reg) of I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				reg				-> Register selected to read
*				data			-> Direction to store the byte data
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_ReadByte_Reg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t* data)
{
	I2C_Error_Code status;

	status = I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	if (status) {return status;}

	status = I2C_Write(I2Cx, reg);
	if (status) {return status;}

	status = I2C_Stop(I2Cx);
	if (status) {return status;}

	status = I2C_Start(I2Cx, SlaveAddress, I2C_RECEIVER_MODE, I2C_ACK_DISABLE);
	if (status) {return status;}

	status = I2C_ReadNack(I2Cx, data);
	if (status) {return status;}

	return I2C_NoError;
}

/*
* Reads several bytes (count) from the Register (reg) of I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				reg				-> Register selected to read
*				data			-> Direction to store the byte data
*				count			-> Number of bytes to read
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/

I2C_Error_Code I2C_ReadByte_MultiReg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint16_t count)
{
	I2C_Error_Code status;

	status = I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
	if (status) {return status;}

	status = I2C_Write(I2Cx, reg);
	if (status) {return status;}

	status = I2C_Start(I2Cx, SlaveAddress, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	if (status) {return status;}

	while (count--)
	{
		if (!count)
		{
			status = I2C_ReadNack(I2Cx, data++);
			if (status) {return status;}
		}
		else
		{
			status = I2C_ReadAck(I2Cx, data++);
			if (status) {return status;}
		}
	}
	return I2C_NoError;
}

/*
* Reads a portion (BitStart and lenght) of the Register (reg) of I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				reg				-> Register selected to read
*				BitStart		-> Initial bit to read (76543210)
*				lenght			-> Portion size of the register
*				data			-> Direction to store the byte data
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_ReadBits_Reg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t BitStart, uint8_t length, uint8_t *data)
{
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted

	uint8_t tmp;
	I2C_Error_Code status;

    status = I2C_ReadByte_Reg(I2Cx, SlaveAddress, reg, &tmp);
    if (status) {return status;}

    uint8_t mask = ((1 << length) - 1) << (BitStart - length + 1);
    tmp &= mask;
    tmp >>= (BitStart - length + 1);

    *data = tmp;
    return I2C_NoError;
}

/*
* Writes one byte to Register (reg) of I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				reg				-> Register selected to write
*				data			-> Byte to transmit to the slave
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_WriteByte_Reg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t data)
{
	I2C_Error_Code status;

	status = I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	if (status) {return status;}

	status = I2C_Write(I2Cx, reg);
	if (status) {return status;}

	status = I2C_Write(I2Cx, data);
	if (status) {return status;}

	status = I2C_Stop(I2Cx);
	if (status) {return status;}

	return I2C_NoError;
}

/*
* Writes several bytes to Register (reg) of I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				reg				-> Register selected to write
*				data			-> Direction of the first position of data array
*				count			-> Number of bytes to transmit
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/

I2C_Error_Code I2C_WriteByte_MultiReg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint16_t count)
{
	I2C_Error_Code status;

	status = I2C_Start(I2Cx, SlaveAddress, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	if (status) {return status;}

	status = I2C_Write(I2Cx, reg);
	if (status) {return status;}

	while (count--)
	{
		status = I2C_Write(I2Cx, *data++);
		if (status) {return status;}
	}

	status = I2C_Stop(I2Cx);
	if (status) {return status;}

	return I2C_NoError;
}

/*
* Writes a portion (BitStart and lenght) to the Register (reg) of I2C device ( SlaveAddress )
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				reg				-> Register selected to write
*				BitStart		-> Initial bit to write (76543210)
*				lenght			-> Portion size of the register
*				data			-> Data (0-8bits)
* Return :
				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_WriteBits_Reg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t BitStart, uint8_t length, uint8_t data)
{
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
	// 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value

    uint8_t tmp;
    I2C_Error_Code status;

    status = I2C_ReadByte_Reg(I2Cx, SlaveAddress, reg, &tmp);
    if (status) {return status;}

    uint8_t mask = ((uint8_t)(1 << length) - 1) << (BitStart - length + 1);
    data <<= (BitStart - length + 1);
    data &= mask;
    tmp &= ~(mask);
    tmp |= data;

    status = I2C_WriteByte_Reg(I2Cx, SlaveAddress, reg, tmp);
    if (status) {return status;}

    return I2C_NoError;
}

/********************************************* PRIVATE FUNCTIONS *****************************************************/

/*
* Generates START signal
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				SlaveAddress 	-> I2C Device Address (7bits)
*				direction		-> Master Transmitter (0) o  Master Receiver (1)
*				ack				-> Enables ACK (1), Disable ACK (0)
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/

I2C_Error_Code I2C_Start(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t direction, uint8_t ack)
{
	/* Generate I2C start pulse */
	I2Cx->CR1 |= I2C_CR1_START;

	/* Wait till I2C is busy */
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_TestEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if (--I2C_Timeout == 0x00){return I2C_StartBit_TimeOut;}
	}

	/* Enable ack if we select it */
	if (ack) {I2Cx->CR1 |= I2C_CR1_ACK;}

	/* Send write/read bit */
	if (direction == I2C_TRANSMITTER_MODE)
	{
		/* Send SlaveAddress with zero last bit */
		I2Cx->DR = (uint8_t)(SlaveAddress << 1 );

		/* Wait till finished */
		I2C_Timeout = I2C_TIMEOUT;
		while (!I2C_TestEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			if (--I2C_Timeout == 0x00) {return I2C_AddressTransfer_Timeout;}
		}
	}
	if (direction == I2C_RECEIVER_MODE)
	{
		/* Send SlaveAddress with 1 last bit */
		I2Cx->DR = ((uint8_t)(SlaveAddress<<1)) | I2C_OAR1_ADD0;

		/* Wait till finished */
		I2C_Timeout = I2C_TIMEOUT;
		while (!I2C_TestEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			if (--I2C_Timeout == 0x00) {return I2C_AddressTransfer_Timeout;}
		}
	}

	/* Read status register to clear ADDR flag */
	I2Cx->SR2;

	/* Return 0, everything ok */
	return I2C_NoError;
}

/*
* Puts into DR register one byte (data)
*
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				data			-> Byte to transmit
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_Write(I2C_TypeDef* I2Cx, uint8_t data)
{
	/* Wait till I2C is not busy anymore */
	I2C_Timeout = I2C_TIMEOUT;

	while (!I2C_TestEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if (--I2C_Timeout == 0x00) {return I2C_TXE_Timeout;}
	}

	/* Send I2C data */
	I2Cx->DR = data;
	return I2C_NoError;
}

/*
* Reads from DR one byte and responds with ACK
*
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				data			-> Direction of received data
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_ReadAck(I2C_TypeDef* I2Cx, uint8_t* data)
{
	/* Enable ACK */
	I2Cx->CR1 |= I2C_CR1_ACK;

	/* Wait till not received */
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_TestEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if (--I2C_Timeout == 0x00) {return I2C_ReceiveEvent_Timeout;}
	}

	*data = I2Cx->DR;
	return I2C_NoError;
}

/*
* Reads from DR one byte and responds with NACK and stops the communication
*
* Params :
* 				I2Cx 			-> I2C peripheral selected
*				data			-> Direction of received data
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_ReadNack(I2C_TypeDef* I2Cx, uint8_t* data)
{
	/* Disable ACK */
	I2Cx->CR1 &= ~I2C_CR1_ACK;

	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;

	/* Wait till received */
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_TestEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if (--I2C_Timeout == 0x00) {return I2C_ReceiveEvent_Timeout;}
	}

	*data = I2Cx->DR;
	return I2C_NoError;
}

/*
* Stops the I2C communication
*
* Params :
* 				I2Cx 			-> I2C peripheral selected
* Return :
*				I2C_Error_Code  -> Returns the error code ( defined on I2C_Lib.h )
*/
I2C_Error_Code I2C_Stop(I2C_TypeDef* I2Cx)
{
	/* Wait till transmitter not empty */
	I2C_Timeout = I2C_TIMEOUT;
	while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF)))) {
		if (--I2C_Timeout == 0x00) {return I2C_End_Timeout;}
	}

	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;

	/* Return 0, everything ok */
	return I2C_NoError;
}

/*
* Reads from DR one byte and responds with NACK and stops the communication
*
* Params :
* 				I2Cx 			-> I2C peripheral selected
* Return :
*				ErrorStatus 	-> Returns Error (0) or Succes(1)
*/
ErrorStatus I2C_TestEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;
  ErrorStatus status = ERROR;

  /* Read the I2Cx status register */
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2) & FLAG_MASK;

  /* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & I2C_EVENT) == I2C_EVENT)
  {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS;
  }
  else
  {
    /* ERROR: last event is different from I2C_EVENT */
    status = ERROR;
  }
  /* Return status */
  return status;
}
