#include "I2C_lib.h"
#include "MPU6050.h"
#include "USART_Lib.h"
//#include "stm32l1xx.h"

#include <math.h>
#include <stdio.h>

void Systick_Configuration(void);
void TimingDelay_Decrement(void);
void Delay(uint32_t nTime);

void ftoa(float n, uint8_t *res, int afterpoint);
int intToStr(int x, uint8_t str[], int d);
void reverse(uint8_t *str, int len);

static volatile uint32_t TimingDelay;

int main(void)
{
		MPU6050_t MPU6050_Data0;
	    MPU6050_t MPU6050_Data1;
	    uint8_t sensor1 = 0, sensor2 = 0;
	    char str[120];

	    /* Initialize system */
	    SystemInit();
	    Systick_Configuration();
	    USART2_Init(9600);
	    USART_Send(USART2, "Arranque\n");
	    /* Initialize MPU6050 sensor 0, address = 0xD0, AD0 pin on sensor is low */
	    if (MPU6050_InitConfig(&MPU6050_Data0, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_250s) == MPU6050_Result_Ok) {
	    		/* Display message to user */
	    		USART_Send(USART2, "MPU6050 sensor 0 is ready to use!\n");

	    		/* Sensor 1 OK */
	    		sensor1 = 1;
	    	}


	    while (1) {
	    		Delay(50);

	            /* If sensor 1 is connected */
	            if (sensor1) {
	                /* Read all data from sensor 1 */
	                MPU6050_ReadAll(&MPU6050_Data0);

	                /* Format data */
	                sprintf(str, "1. Accelerometer\n- X:%d\n- Y:%d\n- Z:%d\nGyroscope\n- X:%d\n- Y:%d\n- Z:%d\nTemperature\n- %3.4f\n\n\n",
	                    MPU6050_Data0.Accelerometer_X,
	                    MPU6050_Data0.Accelerometer_Y,
	                    MPU6050_Data0.Accelerometer_Z,
	                    MPU6050_Data0.Gyroscope_X,
	                    MPU6050_Data0.Gyroscope_Y,
	                    MPU6050_Data0.Gyroscope_Z,
	                    MPU6050_Data0.Temperature
	                );

	                /* Show to usart */
	                USART_Send(USART2, str);
	            }

	            /* If sensor 2 is connected */
	            if (sensor2) {
	                /* Read all data from sensor 1 */
	                MPU6050_ReadAll(&MPU6050_Data1);

	                /* Format data */
	                sprintf(str, "2. Accelerometer\n- X:%d\n- Y:%d\n- Z:%d\nGyroscope\n- X:%d\n- Y:%d\n- Z:%d\nTemperature\n- %3.4f\n\n\n",
	                    MPU6050_Data1.Accelerometer_X,
	                    MPU6050_Data1.Accelerometer_Y,
	                    MPU6050_Data1.Accelerometer_Z,
	                    MPU6050_Data1.Gyroscope_X,
	                    MPU6050_Data1.Gyroscope_Y,
	                    MPU6050_Data1.Gyroscope_Z,
	                    MPU6050_Data1.Temperature
	                );

	                /* Show to usart */
	                USART_Send(USART2, str);
	            }
	        }
	       return 0;


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
