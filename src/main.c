#include "main.h"

int main(void)
{
		MPU6050_t MPU6050_Data;
	    char str[120];
	    I2C_Error_Code status;

	    /* Initialize system */
	    SystemInit();
	    Systick_Configuration();
	    USART2_Init(9600);
	    USART_Send(USART2, "Arranque\n");
	    /* Initialize MPU6050 sensor 0, address = 0xD0, AD0 pin on sensor is low */

	    status = MPU6050_InitConfig(&MPU6050_Data, MPU6050_ACCEL_FS_2, MPU6050_GYRO_FS_250);
		if(status)
		{
	    	DisplayErrorCode(status);
	    	return 0;
		}


	    Delay(1000);

	    while (1){

	    	Delay(200);

	    	status = MPU6050_ReadAll(&MPU6050_Data);
			if(status)
			{
		    	DisplayErrorCode(status);
		    	return 0;
			}
			/* Format data */
			sprintf(str, "1. Accelerometer\n- X:%d\n- Y:%d\n- Z:%d\nGyroscope\n- X:%d\n- Y:%d\n- Z:%d\nTemperature\n- %3.4f\n\n\n",
				MPU6050_Data.raw_accel_x,
				MPU6050_Data.raw_accel_y,
				MPU6050_Data.raw_accel_z,
				MPU6050_Data.raw_gyro_x,
				MPU6050_Data.raw_gyro_y,
				MPU6050_Data.raw_gyro_z,
				MPU6050_Data.raw_temp
				);
			USART_Send(USART2, str);

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
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
}


void reverse(uint8_t *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

int intToStr(int x, uint8_t str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

void ftoa(float n, uint8_t *res, int afterpoint)
{

    int ipart = (int)n;
    float fpart = n - (float)ipart;
    int i = intToStr(ipart, res, 0);
    if (afterpoint != 0)
    {
        res[i] = '.';
        fpart = fpart * pow(10, afterpoint);
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
