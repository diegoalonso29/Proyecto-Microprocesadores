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

	status = MPU6050_SetLPF(LPF_BW);
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
	DataStruct->raw_accel_x = (((int16_t)data[0]) << 8) | data[1];
	DataStruct->raw_accel_y = (((int16_t)data[2]) << 8) | data[3];
	DataStruct->raw_accel_z = (((int16_t)data[4]) << 8) | data[5];

	/* Format temperature */
	DataStruct->raw_temp = (((int16_t)data[6]) << 8) | data[7];

	/* Format gyroscope data */
	DataStruct->raw_gyro_x = (((int16_t)data[8]) << 8) | data[9];
	DataStruct->raw_gyro_y = (((int16_t)data[10]) << 8) | data[11];
	DataStruct->raw_gyro_z = (((int16_t)data[12]) << 8) | data[13];


	/* Return OK */
	return I2C_NoError;
}

MPU6050_Data_Float getFloat (MPU6050_Data_Raw DataStruct)
{
	MPU6050_Data_Float DataFloat;

	DataFloat.accel_x = ((float)DataStruct.raw_accel_x) / MPU6050_ACCE_SENS_4;
	DataFloat.accel_y = ((float)DataStruct.raw_accel_y) / MPU6050_ACCE_SENS_4;
	DataFloat.accel_z = ((float)DataStruct.raw_accel_z) / MPU6050_ACCE_SENS_4;

	DataFloat.gyro_x = ((float)DataStruct.raw_gyro_x) / MPU6050_GYRO_SENS_250;
	DataFloat.gyro_y = ((float)DataStruct.raw_gyro_y) / MPU6050_GYRO_SENS_250;
	DataFloat.gyro_z = ((float)DataStruct.raw_gyro_z) / MPU6050_GYRO_SENS_250;

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
    float pitchAcc, rollAcc;

    DataRPY->pitch += ((float)DataRaw->raw_gyro_x / GYRO_SENS) * (1/SAMPLE_FREQ); 	// Angle around the X-axis
    DataRPY->roll  -= ((float)DataRaw->raw_gyro_x / GYRO_SENS) * (1/SAMPLE_FREQ);   // Angle around the Y-axis


    int forceMagnitudeApprox = abs(DataRaw->raw_accel_x) + abs(DataRaw->raw_accel_y) + abs(DataRaw->raw_accel_z);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
    {
	// Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2f((float)DataRaw->raw_accel_y, (float)DataRaw->raw_accel_z) * 180 / M_PI;
        DataRPY->pitch = DataRPY->pitch * 0.98 + pitchAcc * 0.02;

	// Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2f((float)DataRaw->raw_accel_x, (float)DataRaw->raw_accel_z) * 180 / M_PI;
        DataRPY->roll = DataRPY->roll * 0.98 + rollAcc * 0.02;
    }
}


I2C_Error_Code MPU6050_Calibration(MPU6050_Data_Float* DataStruct)
{
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3] = {0, 0, 0};
  int32_t accel_bias[3] = {0, 0, 0};
  I2C_Error_Code status;

  I2C_InitConfig(MPU6050_I2C);

/* Comprobación de la conexión del dispositivo*/
  status = I2C_IsConnected(MPU6050_I2C, MPU6050_I2C_ADDR);
  if(status) {return status;}

  status = MPU6050_SleepMode(DISABLE);
  if(status) {return status;}
  Delay(200);
  status = MPU6050_ResetDevice();
  if(status) {return status;}
  Delay(200);
// get stable time source
// Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
  status = MPU6050_Set_ClockSel(MPU6050_CLOCK_PLL_XGYRO);
  if(status) {return status;}

  USART_Send(USART2, "1");
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_PWR_MGMT_2, 0x00);
  if(status) {return status;}
  Delay(100);

// Configure device for bias calculation
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_INT_ENABLE, 0x00);   // Disable all interrupts
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_EN, 0x00);      // Disable FIFO
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_PWR_MGMT_1, 0x00);   // Turn on internal clock source
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_I2C_MST_CTRL, 0x00); // Disable I2C master
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_USER_CTRL, 0x0C);    // Reset FIFO and DMP
  if(status) {return status;}

  Delay(15);
  USART_Send(USART2, "2");
// Configure MPU6050 gyro and accelerometer for bias calculation
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_CONFIG, MPU6050_DLPF_BW_188);      // Set low-pass filter to 188 Hz
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity
  if(status) {return status;}

  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

// Configure FIFO to capture accelerometer and gyro data for bias calculation
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_USER_CTRL, 0x40);   // Enable FIFO
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO  (max size 1024 bytes in MPU-6050)
  if(status) {return status;}
  Delay(80); // accumulate 80 samples in 80 milliseconds = 960 bytes

// At end of sample accumulation, turn off FIFO sensor read
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
  if(status) {return status;}
  status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_COUNTH, &data[0], 2); // read FIFO sample count
  if(status) {return status;}
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging

  USART_Send(USART2, "3");

  for (ii = 0; ii < packet_count; ii++) {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_FIFO_R_W, &data[0], 12); // read data for averaging

    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];

}
    accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0]  /= (int32_t) packet_count;
    gyro_bias[1]  /= (int32_t) packet_count;
    gyro_bias[2]  /= (int32_t) packet_count;

  if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
  else {accel_bias[2] += (int32_t) accelsensitivity;}

// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
  data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

// Push gyro biases to hardware registers
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_XG_OFFS_USRH, data[0]);
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_XG_OFFS_USRL, data[1]);
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_YG_OFFS_USRH, data[2]);
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_YG_OFFS_USRL, data[3]);
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_ZG_OFFS_USRH, data[4]);
  if(status) {return status;}
  status = I2C_WriteByte_Reg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_ZG_OFFS_USRL, data[5]);
  if(status) {return status;}

  DataStruct->gyro_x = (float) gyro_bias[0]/(float) gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
  DataStruct->gyro_y = (float) gyro_bias[1]/(float) gyrosensitivity;
  DataStruct->gyro_z = (float) gyro_bias[2]/(float) gyrosensitivity;

// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
// the accelerometer biases calculated above must be divided by 8.

  int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
  status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_XA_OFFS_H, &data[0], 2); // Read factory accelerometer trim values
  if(status) {return status;}
  accel_bias_reg[0] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_YA_OFFS_H, &data[0], 2);
  if(status) {return status;}
  accel_bias_reg[1] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  status = I2C_ReadByte_MultiReg(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_RA_ZA_OFFS_H, &data[0], 2);
  if(status) {return status;}
  accel_bias_reg[2] = (int16_t) ((int16_t)data[0] << 8) | data[1];

  uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
  uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

  for(ii = 0; ii < 3; ii++) {
    if(accel_bias_reg[ii] & mask) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
  }

  // Construct total accelerometer bias, including calculated average accelerometer bias from above
  accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

  // Push accelerometer biases to hardware registers
//  writeByte(MPU6050_ADDRESS, XA_OFFSET_H, data[0]);
//  writeByte(MPU6050_ADDRESS, XA_OFFSET_L_TC, data[1]);
//  writeByte(MPU6050_ADDRESS, YA_OFFSET_H, data[2]);
//  writeByte(MPU6050_ADDRESS, YA_OFFSET_L_TC, data[3]);
//  writeByte(MPU6050_ADDRESS, ZA_OFFSET_H, data[4]);
//  writeByte(MPU6050_ADDRESS, ZA_OFFSET_L_TC, data[5]);

// Output scaled accelerometer biases for manual subtraction in the main program
   DataStruct->accel_x_trim = (float)accel_bias[0]/(float)accelsensitivity;
   DataStruct->accel_y_trim = (float)accel_bias[1]/(float)accelsensitivity;
   DataStruct->accel_z_trim = (float)accel_bias[2]/(float)accelsensitivity;

   return I2C_NoError;
}

