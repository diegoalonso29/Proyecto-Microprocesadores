#include "MPU6050.h"


I2C_Error_Code MPU6050_InitConfig(uint8_t AccelRange, uint8_t GyroRange, uint8_t  SampleRate)
{

	uint8_t data_temp = 0;
	I2C_Error_Code status;

	offsets.accel_x_trim = 0;
	offsets.accel_y_trim = 0;
	offsets.accel_z_trim = 0;
	offsets.gyro_x_trim = 0;
	offsets.gyro_y_trim = 0;
	offsets.gyro_z_trim = 0;

	/* Inicialización y configuración de la comunicación I2C */
	I2C_InitConfig(MPU6050_I2C);

	/* Comprobación de la conexión del dispositivo*/
	status = I2C_IsConnected(MPU6050_I2C, MPU6050_I2C_ADDR);
	if(status) {return status;}

	/* Comprueba que el sistema se comunica con el dispositivo adecuado */
	status = I2C_ReadByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_WHO_AM_I, &data_temp);
	if(status) {return status;}
	if (data_temp != MPU6050_I_AM) { return I2C_WhoIam_Error; }

	status = MPU6050_SetLPF(LPF_BW);
	if(status) {return status;}

	/* Set sample rate */
	status = MPU6050_Set_SampleRate(SampleRate);
	if(status) {return status;}

	/* Configuración de la escala para el acelerómetro */
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, AccelRange);
	if(status) {return status;}

	/* Configuración de la escala para el giróscopo */
	status = I2C_WriteBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_GYRO_CONFIG,  MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, GyroRange);
	if(status) {return status;}

	status = MPU6050_Set_INT_Config(DISABLE,DISABLE,DISABLE,DISABLE);
	if(status) {return status;}

	status = MPU6050_Set_ClockSel(MPU6050_CLOCK_PLL_XGYRO);
	if(status) {return status;}

	Delay(200);

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
	DataStruct->raw_accel_x = ((((int16_t)data[0]) << 8) | data[1]) - offsets.accel_x_trim;
	DataStruct->raw_accel_y = ((((int16_t)data[2]) << 8) | data[3]) - offsets.accel_y_trim;
	DataStruct->raw_accel_z = ((((int16_t)data[4]) << 8) | data[5]) - offsets.accel_z_trim;

	/* Format temperature */
	DataStruct->raw_temp = (((int16_t)data[6]) << 8) | data[7];

	/* Format gyroscope data */
	DataStruct->raw_gyro_x = ((((int16_t)data[8]) << 8) | data[9]) - offsets.gyro_x_trim;
	DataStruct->raw_gyro_y = ((((int16_t)data[10]) << 8) | data[11]) - offsets.gyro_y_trim;
	DataStruct->raw_gyro_z = ((((int16_t)data[12]) << 8) | data[13]) - offsets.gyro_z_trim;


	/* Return OK */
	return I2C_NoError;
}

MPU6050_Data_Float MPU6050_GetData (MPU6050_Data_Raw DataStruct)
{
	MPU6050_Data_Float df;

	df.accel_x = (((float)DataStruct.raw_accel_x) / ACCEL_SENS);
	df.accel_y = (((float)DataStruct.raw_accel_y) / ACCEL_SENS);
	df.accel_z = (((float)DataStruct.raw_accel_z) / ACCEL_SENS);

	df.gyro_x = (((float)DataStruct.raw_gyro_x) / GYRO_SENS);
	df.gyro_y = (((float)DataStruct.raw_gyro_y) / GYRO_SENS);
	df.gyro_z = (((float)DataStruct.raw_gyro_z) / GYRO_SENS);

	return df;
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

	Delay(100);
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

I2C_Error_Code MPU6050_Config_ContinuousMeasurement(uint8_t enable)
{
	I2C_Error_Code status;
	if(enable != 0)
	{
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
	}
	else
	{
		status = MPU6050_Set_INT_Enable(0x00);
		if(status) {return status;}
	}

	return I2C_NoError;
}

void EXTI15_10_IRQHandler(void)
{
	uint8_t tmp = 0;
	I2C_Error_Code status;

	if(EXTI_GetFlagStatus(EXTI_Line13))
	{
		status = I2C_ReadBits_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_INT_STATUS,0,1,&tmp);
		if(tmp == 0x01 && status == I2C_NoError)
		{
			if(pos_buffer < BUFFER_SIZE)
			{
				Main_State = MPU6050_Get_Raw_Data(&Buffer_Data[pos_buffer++]);
				if(Main_State == I2C_NoError)
				{
					data_available++;
				}
			}
			else
			{
				pos_buffer = 0;
				Main_State = MPU6050_Get_Raw_Data(&Buffer_Data[pos_buffer++]);
				if(Main_State != I2C_NoError) {data_available++;}
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line13);

	}
}

void MPU6050_Get_RPY_Data(MPU6050_Data_RPY* DataRPY, MPU6050_Data_Raw* DataRaw)
{
	DataRPY->pitch = 0.98*(DataRPY->pitch + (((float)DataRaw->raw_gyro_y / GYRO_SENS) * (1/SAMPLE_FREQ)))
					+ 0.02 *(atan2f((float)DataRaw->raw_accel_x, (float)DataRaw->raw_accel_z) * 180 / M_PI);
	DataRPY->roll = 0.98*(DataRPY->roll + (((float)DataRaw->raw_gyro_x / GYRO_SENS) * (1/SAMPLE_FREQ)))
					+ 0.02 *(atan2f((float)DataRaw->raw_accel_y, (float)DataRaw->raw_accel_z) * 180 / M_PI);
}


I2C_Error_Code MPU6050_Calibration()
{
	I2C_Error_Code status;
	int j = 0;
	int i = 0;
	int32_t acc_x_offset = 0;
	int32_t acc_y_offset = 0;
	int32_t acc_z_offset = 0;
	int32_t gyro_x_offset = 0;
	int32_t gyro_y_offset = 0;
	int32_t gyro_z_offset = 0;

	status = MPU6050_Config_ContinuousMeasurement(1);
	if(status) {return status;}

	while(j<600)
	{
	if(data_available)
		{
			uint8_t m = data_available;

			for(i=0;i<m; i++)
			{
				if(j>100)
				{
			   	acc_x_offset += Buffer_Data[i].raw_accel_x;
			   	acc_y_offset += Buffer_Data[i].raw_accel_y;
			   	acc_z_offset += Buffer_Data[i].raw_accel_z - (int)ACCEL_SENS;
			   	gyro_x_offset += Buffer_Data[i].raw_gyro_x;
			   	gyro_y_offset += Buffer_Data[i].raw_gyro_y;
			   	gyro_z_offset += Buffer_Data[i].raw_gyro_z;
				}
			   	data_available--;
				j++;

			}
			pos_buffer = 0;
		}
	}

	status = MPU6050_Set_INT_Enable(0x00);
	if(status) {return status;}

	offsets.accel_x_trim = (int16_t)(acc_x_offset / (j-100));
	offsets.accel_y_trim = (int16_t)(acc_y_offset / (j-100));
	offsets.accel_z_trim = (int16_t)(acc_z_offset / (j-100));

	offsets.gyro_x_trim= (int16_t)(gyro_x_offset / (j-100));
	offsets.gyro_y_trim= (int16_t)(gyro_y_offset / (j-100));
	offsets.gyro_z_trim= (int16_t)(gyro_z_offset / (j-100));

	USART_Send(USART2, "accel x: ");
	USART_SendFloat(USART2, (float)offsets.accel_x_trim,3);
	USART_Send(USART2, "\taccel y: ");
	USART_SendFloat(USART2, (float)offsets.accel_y_trim,3);
	USART_Send(USART2, "\taccel z: ");
	USART_SendFloat(USART2, (float)offsets.accel_z_trim,3);
	USART_Send(USART2, "\tgyro x: ");
	USART_SendFloat(USART2, (float)offsets.gyro_x_trim,3);
	USART_Send(USART2, "\tgyro y: ");
	USART_SendFloat(USART2, (float)offsets.gyro_y_trim,3);
	USART_Send(USART2, "\tgyro z: ");
	USART_SendFloat(USART2, (float)offsets.gyro_z_trim,3);
	USART_Send(USART2, "\n");

	return I2C_NoError;
}



