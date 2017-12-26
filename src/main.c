#include "main.h"

int main(void)
{
		int i;

		MPU6050_Data_Raw MPU6050_Data;
	    I2C_Error_Code status;
	    MPU6050_Data_Float cipotes;
	    MPU6050_Data_RPY rpy;

	    /* Initialize system */
	    SystemInit();
	    Systick_Configuration();

	    USART2_Init(9600);
	    USART_Send(USART2, "Arranque\n");

	    status = MPU6050_InitConfig(MPU6050_ACCEL_FS_4, MPU6050_GYRO_FS_250, SAMPLE_FREQ);
		if(status)
		{
	    	DisplayErrorCode(status);
	    	return 0;
		}

		MPU6050_Config_ContinuousMeasurement();

		rpy.pitch = 0;
		rpy.roll = 0;

	    while (1)
	    {

	    	if(data_available)
	    		{
	    			uint8_t cipote_actual = data_available;

	    			for(i=0;i<cipote_actual; i++)
	    			{
//	    				cipotes = getFloat(Buffer_Data[i]);
//	    				USART_SendFloat(USART2, cipotes.accel_z,3);
	    				MPU6050_Get_RPY_Data(&rpy, &Buffer_Data[i]);
	    				USART_Send(USART2, "Roll: ");
	    				USART_SendFloat(USART2, rpy.roll,2);
	    				USART_Send(USART2, "\tPitch: ");
	    				USART_SendFloat(USART2, rpy.pitch,2);
	    				USART_Send(USART2, "\n");
	    				data_available--;
	    			}
	    			pos_buffer = 0;
	    		}

//	    	status = MPU6050_Read_Raw_Values(&MPU6050_Data);
//			if(status)
//			{
//		    	DisplayErrorCode(status);
//		    	return 0;
//			}

//			USART_SendFloat(USART2, MPU6050_Data.accel_z,2);

	        }


	    return 0;
}


void Delay(uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{

  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

void Systick_Configuration(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency/1000);
}
