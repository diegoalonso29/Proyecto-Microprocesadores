#include "I2C_lib.h"
#include "MPU6050.h"

void Systick_Configuration(void);
void TimingDelay_Decrement(void);
void Delay(uint32_t nTime);

static volatile uint32_t TimingDelay;

int main(void){

	Systick_Configuration();
	raw_data raw;
	init_data init;
	gyro_data gyro;
	accel_data accel;
	uint8_t id;

    I2C_Config(100000);
    //NVIC_Config();
     // Delay(100);
    //MPU6050_SleepMode(DISABLE);
    while(1)
      {
      id = MPU6050_GetDeviceID();
      }
//		MPU6050_Init();
//      MPU6050_CalibrateSensor(&init);
//
//      while(1){
//      MPU6050_GetRawAccelGyro(&raw);
//      MPU6050_ConvertToFloat(&raw, &accel, &gyro);
//      }


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

void Systick_Configuration()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
}
