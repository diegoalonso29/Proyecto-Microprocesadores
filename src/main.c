#include "I2C_lib.h"
#include "MPU6050.h"
#include "USART_Lib.h"

#include <math.h>
#include <stdio.h>

void Systick_Configuration(void);
void TimingDelay_Decrement(void);
void Delay(uint32_t nTime);
void SysTickConfig(void);
void ftoa(float n, uint8_t *res, int afterpoint);
int intToStr(int x, uint8_t str[], int d);
void reverse(uint8_t *str, int len);

static volatile uint32_t TimingDelay;

int main(void){

//	uint8_t ptr;
	uint8_t high;
	uint8_t low;
	uint8_t wakeup = 0x00;

	I2C_Config();
	USART2_Init(9600);

	USART_Send(USART2, "Correcto encendido\n");

//	raw_data raw;
//	init_data init;
//	gyro_data gyro;
//	accel_data accel;
//	uint8_t id;
	I2C_ReadByte(MPU6050_Address, MPU6050_RA_PWR_MGMT_1, &wakeup);
	USART_Send(USART2,"El registro del PWR primero vale: ");
	I2C_WriteByte(MPU6050_Address, MPU6050_RA_PWR_MGMT_1, &wakeup);
	I2C_ReadByte(MPU6050_Address, MPU6050_RA_PWR_MGMT_1, &wakeup);

	USART_Send(USART2,"El registro del PWR luego vale: ");
	USART_Send(USART2,&wakeup);

	I2C_ReadByte(MPU6050_Address, MPU6050_RA_ACCEL_ZOUT_H, &high);
	I2C_ReadByte(MPU6050_Address, MPU6050_RA_ACCEL_ZOUT_L, &low);

	USART_Send(USART2,&high);
	USART_Send(USART2,&low);

	//ftoa(float n, uint8_t *res, int afterpoint);
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

void SysTickConfig(void)
{
  /* Setup SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
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
