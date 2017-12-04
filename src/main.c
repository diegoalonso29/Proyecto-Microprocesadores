#include "I2C_lib.h"
#include "MPU6050.h"

void Systick_Configuration(void);
void TimingDelay_Decrement(void);
void Delay(uint32_t nTime);
void SysTickConfig(void);

static volatile uint32_t TimingDelay;

int main(void){

	//uint8_t ptr = 25;

	I2C_Config();
	//SysTickConfig();

	raw_data raw;
	init_data init;
	gyro_data gyro;
	accel_data accel;
	uint8_t id;

	uint8_t g [10];
	uint8_t *ptr;
	ptr = g;
	int i;
	MPU6050_Init();
	for( i = 0; i<10 ; i++)
	{
		I2C_ReadByte(MPU6050_Address, MPU6050_RA_ACCEL_ZOUT_H, ptr++);
		I2C_ReadByte(MPU6050_Address, MPU6050_RA_ACCEL_ZOUT_H, ptr++);
		//ptr ++;
	}

//// Write_Byte(0xD0, 0x6B, 0x00);
////    Delay(100);
//
	I2C_ReadByte(0xD0, 0x6B, ptr);
//
	MPU6050_Init();
//	MPU6050_CalibrateSensor(&init);
//	int x = 0;
//	while(x<10){
//		MPU6050_GetRawAccelGyro(&raw);
//		MPU6050_ConvertToFloat(&raw, &accel, &gyro);
//		x++;
//		}

//while(1);

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
