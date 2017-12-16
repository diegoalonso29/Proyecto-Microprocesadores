#include "MPU6050.h"


I2C_Error_Code MPU6050_InitConfig(uint8_t AccelRange, uint8_t GyroRange, uint8_t  SampleRate)
{

	uint8_t data_temp = 0;
	I2C_Error_Code status;

	/* Inicialización y configuración de la comunicación I2C */
	I2C_InitConfig(MPU6050_I2C);

	/* Comprobación de la conexión del dispositivo*/
	status = I2C_IsConnected(MPU6050_I2C, MPU6050_I2C_ADDR);
	if(status) {return status;}

	/* Comprueba que el sistema se comunica con el dispositivo adecuado */
	status = I2C_ReadByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_WHO_AM_I, &data_temp);
	if(status) {return status;}
	if (data_temp != MPU6050_I_AM) { return I2C_WhoIam_Error; }

	status = MPU6050_SetLPF(MPU6050_DLPF_BW_5);
	if(status) {return status;}

	/* Set 20Hz sample rate */
	status = MPU6050_Set_SampleRate(SampleRate);
	if(status) {return status;}

	/* Configuración de la escala para el acelerómetro */
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, AccelRange);
	if(status) {return status;}

	/* Configuración de la escala para el giróscopo */
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_GYRO_CONFIG,  MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, GyroRange);
	if(status) {return status;}

	status = MPU6050_Set_INT_Config(DISABLE,DISABLE,DISABLE,1);
	if(status) {return status;}

	status = MPU6050_Set_ClockSel(MPU6050_CLOCK_PLL_XGYRO);
	if(status) {return status;}

	status = MPU6050_SleepMode(DISABLE);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Get_Raw_Accelerometer(MPU6050_Data_Raw* DataStruct)
{
	uint8_t data[6];
	I2C_Error_Code status;

	/* Read accelerometer data */
	status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_ACCEL_XOUT_H, data, 6);
	if(status) {return status;}

	/* Format */
	DataStruct->raw_accel_x = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->raw_accel_y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->raw_accel_z = (int16_t)(data[4] << 8 | data[5]);

	/* Return OK */
	return I2C_NoError;
}

I2C_Error_Code MPU6050_Get_Raw_Gyroscope(MPU6050_Data_Raw* DataStruct)
{
	uint8_t data[6];
	I2C_Error_Code status;

	/* Read gyroscope data */
	status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_GYRO_XOUT_H, data, 6);
	if(status) {return status;}

	/* Format */
	DataStruct->raw_gyro_x = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->raw_gyro_y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->raw_gyro_z = (int16_t)(data[4] << 8 | data[5]);

	/* Return OK */
	return I2C_NoError;
}

I2C_Error_Code MPU6050_Get_Raw_Temperature(MPU6050_Data_Raw* DataStruct)
{
	uint8_t data[2];
	int16_t temp;
	I2C_Error_Code status;

	/* Read temperature */
	status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_TEMP_OUT_H, data, 2);
	if(status) {return status;}

	/* Format temperature */
	temp = (data[0] << 8 | data[1]);
	DataStruct->raw_temp = (float)((int16_t)temp / (float)340.0 + (float)36.53);

	/* Return OK */
	return I2C_NoError;
}

I2C_Error_Code MPU6050_Get_Raw_Data(MPU6050_Data_Raw* DataStruct)
{
	uint8_t data[14];
	I2C_Error_Code  status;

	/* Read full raw data, 14bytes */
	status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_ACCEL_XOUT_H, data, 14);
	if(status) {return status;}

	/* Format accelerometer data */
	DataStruct->raw_accel_x = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->raw_accel_y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->raw_accel_z = (int16_t)(data[4] << 8 | data[5]);

	/* Format temperature */
	DataStruct->raw_temp = (int16_t)(data[6] << 8 | data[7]);

	/* Format gyroscope data */
	DataStruct->raw_gyro_x = (int16_t)(data[8] << 8 | data[9]);
	DataStruct->raw_gyro_y = (int16_t)(data[10] << 8 | data[11]);
	DataStruct->raw_gyro_z = (int16_t)(data[12] << 8 | data[13]);


	/* Return OK */
	return I2C_NoError;
}

float_data getFloat (MPU6050_Data_Raw DataStruct)
{
	float_data DataFloat;

	DataFloat.accel_x = (float)DataStruct.raw_accel_x / MPU6050_ACCE_SENS_4;
	DataFloat.accel_y = (float)DataStruct.raw_accel_y / MPU6050_ACCE_SENS_4;
	DataFloat.accel_z = (float)DataStruct.raw_accel_x / MPU6050_ACCE_SENS_4;

	DataFloat.gyro_x = (float)DataStruct.raw_gyro_x / MPU6050_GYRO_SENS_250;
	DataFloat.gyro_y = (float)DataStruct.raw_gyro_x / MPU6050_GYRO_SENS_250;
	DataFloat.gyro_z = (float)DataStruct.raw_gyro_x / MPU6050_GYRO_SENS_250;

	return DataFloat;
}



float MPU6050_Mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void DisplayErrorCode(I2C_Error_Code error)
{

	switch(error)
	{
		case I2C_NoError:
		{
			 USART_Send(USART2, "No Error\n");
			 break;
		}
		case I2C_DeviceNotConnected:
		{
			 USART_Send(USART2, "Error: Device not connected\n");
			 break;
		}
		case I2C_AddressDeviceInvalid:
		{
			 USART_Send(USART2, "Error: Invalid Device Address\n");
			 break;
		}
		case I2C_StartBit_TimeOut:
		{
			 USART_Send(USART2, "Error: StartBit no responds\n");
			 break;
		}
		case I2C_AddressTransfer_Timeout:
		{
			 USART_Send(USART2, "Error: Address transfer no response\n");
			 break;
		}
		case I2C_TXE_Timeout:
		{
			 USART_Send(USART2, "Error: TXE always is not empty\n");
			 break;
		}
		case I2C_ReceiveEvent_Timeout:
		{
			 USART_Send(USART2, "Error: RXNE always is not empty\n");
			 break;
		}
		case I2C_End_Timeout:
		{
			 USART_Send(USART2, "Error: The system can't close the I2C comunication\n");
			 break;
		}
		case I2C_WhoIam_Error:
		{
			 USART_Send(USART2, "Error: The selected device is not the MPU6050");
			 break;
		}
		default:
			break;

	}
}


I2C_Error_Code MPU6050_SetLPF(uint8_t bandwith)
{
	I2C_Error_Code status;
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, bandwith);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_GetLPF(uint8_t* bandwith)
{
	I2C_Error_Code status;
	status = I2C_ReadBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, bandwith);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Set_SampleRate(uint8_t SampleRate)
{
	I2C_Error_Code status;
	uint8_t bandwith;
	uint8_t rate_div = 0;

	status = I2C_ReadBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, &bandwith);
	if(status) {return status;}

	if(bandwith != 0)
	{
		rate_div = (uint8_t)(1000/SampleRate) -1;
		status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_SMPLRT_DIV, rate_div);
		if(status) {return status;}
	}
	else
	{
		rate_div = (uint8_t)(8000/SampleRate) -1;
		status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_SMPLRT_DIV, rate_div);
		if(status) {return status;}
	}
	return I2C_NoError;
}

I2C_Error_Code MPU6050_Get_SampleRate(uint8_t* SampleRate)
{
	I2C_Error_Code status;
	status = I2C_ReadByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_SMPLRT_DIV, SampleRate);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Set_FIFO_Config(uint8_t FilledData)
{
	I2C_Error_Code status;
	status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_EN, FilledData);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Get_FIFO_Config(uint8_t* FilledData)
{
	I2C_Error_Code status;
	status = I2C_ReadByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_EN, FilledData);
	if(status) {return status;}

	return I2C_NoError;
}


I2C_Error_Code MPU6050_Set_INT_Config(uint8_t INT_Level, uint8_t INT_Open, uint8_t Latch, uint8_t Read_Clear)
{
	I2C_Error_Code status;

	uint8_t int_config = 0;
	int_config = (INT_Level << 3) | (INT_Open << 2) | (Latch << 1) | (Read_Clear);

	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, 4,  int_config);
	if(status) {return status;}

	return I2C_NoError;
}


I2C_Error_Code MPU6050_Get_INT_Config(uint8_t* int_config)
{
	I2C_Error_Code status;
	status = I2C_ReadByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_INT_PIN_CFG, int_config);
	if(status) {return status;}

	return I2C_NoError;
}


I2C_Error_Code MPU6050_Set_INT_Enable(uint8_t int_enable)
{
	I2C_Error_Code status;
	status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_INT_ENABLE, int_enable);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Get_INT_Enable(uint8_t* int_enable)
{
	I2C_Error_Code status;
	status = I2C_ReadByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_INT_ENABLE, int_enable);
	if(status) {return status;}

	return I2C_NoError;
}


I2C_Error_Code MPU6050_Get_INT_Status(uint8_t INT, uint8_t* int_status)
{
	I2C_Error_Code status;
	status = I2C_ReadBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_INT_STATUS, INT, 1, int_status);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_SignalPath_Reset(uint8_t sensor_reset)
{
	I2C_Error_Code status;
	status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_SIGNAL_PATH_RESET, sensor_reset);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Set_FIFO_Enable()
{
	I2C_Error_Code status;
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_EN_BIT, 1, 1);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Get_FIFO_Enable(uint8_t* fifo_en)
{
	I2C_Error_Code status;
	status = I2C_ReadBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_EN_BIT, 1, fifo_en);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_ResetDevice()
{
	I2C_Error_Code status;
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, 1, 1);
	if(status) {return status;}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_SleepMode(uint8_t mode)
{
	I2C_Error_Code status;
	if(mode)
	{
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 1, 1);
	if(status) {return status;}
	}

	else
	{
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 1, 0);
	if(status) {return status;}
	}

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Set_ClockSel(uint8_t sel)
{
	I2C_Error_Code status;
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, sel);
	if(status) {return status;}

	return I2C_NoError;
}


I2C_Error_Code MPU6050_Get_ClockSel(uint8_t* sel)
{
	I2C_Error_Code status;
	status = I2C_ReadBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, sel);
	if(status) {return status;}

	return I2C_NoError;
}


I2C_Error_Code MPU6050_Get_FIFO_Count(uint16_t* sel)
{
	uint8_t tmp1 = 0;
	uint8_t tmp2 = 0;

	I2C_Error_Code status;

	status = I2C_ReadByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_COUNTH, &tmp1);
	if(status) {return status;}
	status = I2C_ReadByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_COUNTL, &tmp2);
	if(status) {return status;}

	*sel = (uint16_t)(tmp1<<8) | (uint16_t)tmp2;

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Read_FIFO(MPU6050_Data_Raw* DataStruct)
{
	uint8_t tmp[14];
	I2C_Error_Code status;
	status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_R_W, tmp, 14);
	if(status) {return status;}

	/* Format accelerometer data */
	DataStruct->raw_accel_x = (int16_t)(tmp[0] << 8 | tmp[1]);
	DataStruct->raw_accel_y = (int16_t)(tmp[2] << 8 | tmp[3]);
	DataStruct->raw_accel_z = (int16_t)(tmp[4] << 8 | tmp[5]);

	/* Format temperature */
	DataStruct->raw_temp = (int16_t)(tmp[6] << 8 | tmp[7]);

	/* Format gyroscope tmp */
	DataStruct->raw_gyro_x = (int16_t)(tmp[8] << 8 | tmp[9]);
	DataStruct->raw_gyro_y = (int16_t)(tmp[10] << 8 | tmp[11]);
	DataStruct->raw_gyro_z = (int16_t)(tmp[12] << 8 | tmp[13]);

	return I2C_NoError;
}

I2C_Error_Code MPU6050_Config_ContinuousMeasurement(void)
{
	I2C_Error_Code status;

	GPIO_InitTypeDef gpio_init_struct;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	/* Configuracion Pin13 del GPIOC para la entrada de la EXTI */
	gpio_init_struct.GPIO_Mode = GPIO_Mode_IN;
	gpio_init_struct.GPIO_OType =GPIO_OType_PP;
	gpio_init_struct.GPIO_Pin= GPIO_Pin_13;
	gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&gpio_init_struct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

	EXTI_InitStructure.EXTI_Line = GPIO_Pin_13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger	= EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	pos_buffer = 0;
	data_available = 0;

	status = MPU6050_Set_INT_Enable(MPU6050_DATA_RDY_INT_EN);
	if(status) {return status;}

	return I2C_NoError;
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line13))
	{
		if(pos_buffer < BUFFER_SIZE)
		{
			Main_State = MPU6050_Get_Raw_Data(Buffer_Data+pos_buffer);
			if(Main_State == I2C_NoError)
			{
				data_available++;
				pos_buffer++;
			}
		}
		else
		{
			pos_buffer = 0;
			Main_State = MPU6050_Get_Raw_Data(Buffer_Data+pos_buffer);
			if(Main_State == I2C_NoError) {pos_buffer++;}
		}

		EXTI_ClearITPendingBit(EXTI_Line13);

	}
}







//uint8_t MPU6050_GetDeviceID()
//{
//    uint8_t tmp;
//    I2C_ReadBits(MPU6050_SlaveAddress, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, &tmp);
//    return tmp;
//}
//
//uint8_t MPU6050_TestConnection()
//{
//    return MPU6050_GetDeviceID() == 0x34 ? 0x01 : 0x00; //0b110100; 8-bit representation in hex = 0x34
//}
//
///** Set clock source setting.
// * An internal 8MHz oscillator, gyroscope based clock, or external sources can
// * be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator
// * or an external source is chosen as the clock source, the MPU-60X0 can operate
// * in low power modes with the gyroscopes disabled.
// *
// * Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
// * However, it is highly recommended that the device be configured to use one of
// * the gyroscopes (or an external clock source) as the clock reference for
// * improved stability. The clock source can be selected according to the following table:
// *
// * <pre>
// * CLK_SEL | Clock Source
// * --------+--------------------------------------
// * 0       | Internal oscillator
// * 1       | PLL with X Gyro reference
// * 2       | PLL with Y Gyro reference
// * 3       | PLL with Z Gyro reference
// * 4       | PLL with external 32.768kHz reference
// * 5       | PLL with external 19.2MHz reference
// * 6       | Reserved
// * 7       | Stops the clock and keeps the timing generator in reset
// * </pre>
// *
// * @param source New clock source setting
// * @see MPU6050_GetClockSource()
// * @see MPU6050_RA_PWR_MGMT_1
// * @see MPU6050_PWR1_CLKSEL_BIT
// * @see MPU6050_PWR1_CLKSEL_LENGTH
// */
//void MPU6050_SetClockSource(uint8_t source)
//{
//    I2C_WriteBits(MPU6050_SlaveAddress, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
//}
//
///** Set full-scale gyroscope range.
// * @param range New full-scale gyroscope range value
// * @see MPU6050_GetFullScaleGyroRange()
// * @see MPU6050_GYRO_FS_250
// * @see MPU6050_RA_GYRO_CONFIG
// * @see MPU6050_GCONFIG_FS_SEL_BIT
// * @see MPU6050_GCONFIG_FS_SEL_LENGTH
// */
//void MPU6050_SetFullScaleGyroRange(uint8_t range)
//{
//    I2C_WriteBits(MPU6050_SlaveAddress, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
//}
//
//// GYRO_CONFIG register
//
///** Get full-scale gyroscope range.
// * The FS_SEL parameter allows setting the full-scale range of the gyro sensors,
// * as described in the table below.
// *
// * <pre>
// * 0 = +/- 250 degrees/sec
// * 1 = +/- 500 degrees/sec
// * 2 = +/- 1000 degrees/sec
// * 3 = +/- 2000 degrees/sec
// * </pre>
// *
// * @return Current full-scale gyroscope range setting
// * @see MPU6050_GYRO_FS_250
// * @see MPU6050_RA_GYRO_CONFIG
// * @see MPU6050_GCONFIG_FS_SEL_BIT
// * @see MPU6050_GCONFIG_FS_SEL_LENGTH
// */
//uint8_t MPU6050_GetFullScaleGyroRange()
//{
//    uint8_t tmp;
//    I2C_ReadBits(MPU6050_SlaveAddress, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, &tmp);
//    return tmp;
//}
//
///** Get full-scale accelerometer range.
// * The FS_SEL parameter allows setting the full-scale range of the accelerometer
// * sensors, as described in the table below.
// *
// * <pre>
// * 0 = +/- 2g
// * 1 = +/- 4g
// * 2 = +/- 8g
// * 3 = +/- 16g
// * </pre>
// *
// * @return Current full-scale accelerometer range setting
// * @see MPU6050_ACCEL_FS_2
// * @see MPU6050_RA_ACCEL_CONFIG
// * @see MPU6050_ACONFIG_AFS_SEL_BIT
// * @see MPU6050_ACONFIG_AFS_SEL_LENGTH
// */
//uint8_t MPU6050_GetFullScaleAccelRange()
//{
//    uint8_t tmp;
//    I2C_ReadBits(MPU6050_SlaveAddress, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, &tmp);
//    return tmp;
//}
//
///** Set full-scale accelerometer range.
// * @param range New full-scale accelerometer range setting
// * @see MPU6050_GetFullScaleAccelRange()
// */
//void MPU6050_SetFullScaleAccelRange(uint8_t range)
//{
//    I2C_WriteBits(MPU6050_SlaveAddress, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
//}
//
///** Get sleep mode status.
// * Setting the SLEEP bit in the register puts the device into very low power
// * sleep mode. In this mode, only the serial interface and internal registers
// * remain active, allowing for a very low standby current. Clearing this bit
// * puts the device back into normal mode. To save power, the individual standby
// * selections for each of the gyros should be used if any gyro axis is not used
// * by the application.
// * @return Current sleep mode enabled status
// * @see MPU6050_RA_PWR_MGMT_1
// * @see MPU6050_PWR1_SLEEP_BIT
// */
//uint8_t MPU6050_GetSleepModeStatus()
//{
//    uint8_t tmp;
//    I2C_ReadBit(MPU6050_SlaveAddress, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, &tmp);
//    return tmp == 0x00 ? 0x00 : 0x01;
//}
//
//
//void MPU6050_SleepMode(FunctionalState NewState)
//{
//    I2C_WriteBit(MPU6050_SlaveAddress, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, NewState);
//}
//
//void MPU6050_GetRawAccelGyro(raw_data* data)
//{
//    uint8_t tmpBuffer[14];
//
//    I2C_ReadData(MPU6050_SlaveAddress, MPU6050_RA_ACCEL_XOUT_H, &tmpBuffer[0],  14);
//    /*
//     * 10010111 00000000
//     * 00000000 10011001
//     * ------------------ OR
//     * 10010111 10011001
//     */
//    data->raw_accel_x = ( ((int16_t)tmpBuffer[0] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[1] & 0xFFFF);
//    data->raw_accel_y = ( ((int16_t)tmpBuffer[2] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[3] & 0xFFFF);
//    data->raw_accel_z = ( ((int16_t)tmpBuffer[4] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[5] & 0xFFFF);
//
//    data->raw_gyro_x = ( ((int16_t)tmpBuffer[6] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[7] & 0xFFFF);
//    data->raw_gyro_y = ( ((int16_t)tmpBuffer[8] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[9] & 0xFFFF);
//    data->raw_gyro_z = ( ((int16_t)tmpBuffer[10] << 8) & 0xFFFF) | ((int16_t)tmpBuffer[11] & 0xFFFF);
//
//    data->raw_temp = ( ((int16_t)tmpBuffer[12] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[13] & 0xFFFF);
//}
//
//void MPU6050_ConvertToFloat(raw_data* raw, accel_data *a_data, gyro_data *g_data )
//{
//   	a_data->accel_x = (raw->raw_accel_x / 32767)*MPU6050_Accel_Range;
//   	a_data->accel_y = (raw->raw_accel_y / 32767)*MPU6050_Accel_Range;
//   	a_data->accel_z = (raw->raw_accel_z / 32767)*MPU6050_Accel_Range;
//
//   	g_data->gyro_x = (raw->raw_gyro_x / 32767)*MPU6050_Gyro_Range;
//   	g_data->gyro_y = (raw->raw_gyro_y / 32767)*MPU6050_Gyro_Range;
//   	g_data->gyro_z = (raw->raw_gyro_z / 32767)*MPU6050_Gyro_Range;
//
//}
//void MPU6050_CalibrateSensor(init_data *init)
//{
//	int i;
//	int num_readings = 10;
//	raw_data raw;
//	//vTaskDelay(200);
//	MPU6050_GetRawAccelGyro(&raw);
//
//	  // Read and average the raw values from the IMU
//	for (i = 0; i < num_readings; i++) {
//
//		MPU6050_GetRawAccelGyro(&raw);
//	    init->init_accel_x += raw.raw_accel_x;
//	    init->init_accel_y += raw.raw_accel_y;
//	    init->init_accel_z += raw.raw_accel_z;
//	    init->init_gyro_x += raw.raw_gyro_x;
//	    init->init_gyro_y += raw.raw_gyro_y;
//	    init->init_gyro_z += raw.raw_gyro_z;
//	   // vTaskDelay(100);
//	  }
//	init->init_accel_x /= num_readings;
//	init->init_accel_y /= num_readings;
//	init->init_accel_z /= num_readings;
//	init->init_gyro_x /= num_readings;
//	init->init_gyro_y /= num_readings;
//	init->init_gyro_z /= num_readings;
//}
//
//
//void MPU6050_Init(void)
//{
//
//	MPU6050_SetFullScaleAccelRange(1);
//	MPU6050_SetFullScaleGyroRange(1);
//	//MPU6050_CalibrateSensor();
//	MPU6050_SleepMode(DISABLE);
//}
//
//
