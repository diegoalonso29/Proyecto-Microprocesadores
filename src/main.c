#include "main.h"

int main(void)
{
		MPU6050_t MPU6050_Data;
	    //char str[120];
	    I2C_Error_Code status;

	    /* Initialize system */
	    SystemInit();
	    Systick_Configuration();
	    USART2_Init(9600);
	    USART_Send(USART2, "Arranque\n");

	    status = MPU6050_InitConfig(&MPU6050_Data, MPU6050_ACCEL_FS_4, MPU6050_GYRO_FS_250);
		if(status)
		{
	    	DisplayErrorCode(status);
	    	return 0;
		}


	    Delay(1000);
	    while (1){

	    	Delay(20);


	    	MPU6050_Read_Raw_Values(&MPU6050_Data);

//	    	status = MPU6050_Read_Raw_Values(&MPU6050_Data);
//			if(status)
//			{
//		    	DisplayErrorCode(status);
//		    	return 0;
//			}

			USART_SendFloat(USART2, MPU6050_Data.accel_z,2);

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
