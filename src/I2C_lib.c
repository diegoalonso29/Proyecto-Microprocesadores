#include "I2C_lib.h"

void I2C_Config(uint32_t I2C_ClockSpeed){

	GPIO_InitTypeDef gpio_init_struct;
    I2C_InitTypeDef i2c_init_struct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_I2C, ENABLE);

	/*Asignamos la funcion alterna de GPIO_AF_I2C1 a ambos pines para realizar comunicacion*/
	GPIO_PinAFConfig(I2C_GPIO,I2C_SCL_PinSource, GPIO_AF_I2Cx);
	GPIO_PinAFConfig(I2C_GPIO,I2C_SDA_PinSource, GPIO_AF_I2Cx);

	/* Configuracion Pin8 (SCL) */
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType =GPIO_OType_OD;
	gpio_init_struct.GPIO_Pin= I2C_SCL_Pin;
	gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_Init(I2C_GPIO,&gpio_init_struct);

	/*Configuracion Pin9 (SDA)*/
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType =GPIO_OType_OD;
	gpio_init_struct.GPIO_Pin= I2C_SDA_Pin;
	gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_Init(I2C_GPIO,&gpio_init_struct);

    /*Configuracion de la comunicacion I2C */
	i2c_init_struct.I2C_Ack = I2C_Ack_Enable;
	i2c_init_struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_init_struct.I2C_ClockSpeed = I2C_ClockSpeed;
	i2c_init_struct.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_init_struct.I2C_Mode = I2C_Mode_I2C;
	i2c_init_struct.I2C_OwnAddress1 = 0xDA;
    I2C_Init(I2Cx, &i2c_init_struct);

    I2C_Cmd(I2Cx,ENABLE);

}

void NVIC_Config(){

	/*Configuracion de NVIC para la interrupcion I2C */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = I2Cx_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void I2C_WriteByte(uint8_t SlaveAddress, uint8_t* Buffer_ptr, uint8_t AddressWriteReg)
{
    I2C_GenerateSTART(I2Cx, ENABLE);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2Cx, AddressWriteReg);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2Cx, *Buffer_ptr);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2Cx, ENABLE);
}

void I2C_ReadData(uint8_t SlaveAddress, uint8_t* Buffer_ptr, uint8_t AddressReadReg, uint16_t NumByteToRead)
{
    /* While the bus is busy */
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send MPU6050 address for write */
    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2Cx, ENABLE);

    /* Send the MPU6050's internal address to write to */
    I2C_SendData(I2Cx, AddressReadReg);

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send MPU6050 address for read */
    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* While there is data to be read */
    while (NumByteToRead)
    {
        if (NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2Cx, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(I2Cx, ENABLE);
        }

        /* Test on EV7 and clear it */
        if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the MPU6050 */
            *Buffer_ptr = I2C_ReceiveData(I2Cx);

            /* Point to the next location where the byte read will be saved */
            Buffer_ptr++;

            /* Decrement the read bytes counter */
            NumByteToRead--;
        }
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    // EXT_CRT_SECTION();
}

void I2C_WriteBits(uint8_t SlaveAddress, uint8_t WriteAddressReg, uint8_t BitStart, uint8_t length, uint8_t data)
{
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t tmp;
    I2C_ByteRead(SlaveAddress, &tmp, WriteAddressReg, 1);
    uint8_t mask = ((1 << length) - 1) << (BitStart - length + 1);
    data <<= (BitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    tmp &= ~(mask); // zero all important bits in existing byte
    tmp |= data; // combine data with existing byte
    I2C_ByteWrite(SlaveAddress, &tmp, WriteAddressReg);
}

/** write a single bit in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 */
void MPU6050_WriteBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data)
{
    uint8_t tmp;
    MPU6050_I2C_BufferRead(slaveAddr, &tmp, regAddr, 1);
    tmp = (data != 0) ? (tmp | (1 << bitNum)) : (tmp & ~(1 << bitNum));
    MPU6050_I2C_ByteWrite(slaveAddr, &tmp, regAddr);
}

/** Read multiple bits from an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
 */
void I2C_ReadBits(uint8_t SlaveAddr, uint8_t ReadAddressReg, uint8_t BitStart, uint8_t length, uint8_t *data)
{
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t tmp;
    I2C_ReadData(slaveAddr, &tmp, ReadAddressReg, 1);
    uint8_t mask = ((1 << length) - 1) << (BitStart - length + 1);
    tmp &= mask;
    tmp >>= (BitStart - length + 1);
    *data = tmp;
}

/** Read a single bit from an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
 */
void I2C_ReadBit(uint8_t SlaveAddress, uint8_t ReadAddressReg, uint8_t BitNum, uint8_t *data)
{
    uint8_t tmp;
    I2C_ReadData(SlaveAddress, &tmp, ReadAddressReg, 1);
    *data = tmp & (1 << BitNum);
}