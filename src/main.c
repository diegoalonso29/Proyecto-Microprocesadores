#include "I2C_lib.h"
#include "MPU6050.h"

void Systick_Configuration(void);
void TimingDelay_Decrement(void);
void Delay(uint32_t nTime);
void SysTickConfig(void);

static volatile uint32_t TimingDelay;

int main(void){

	I2C_Config();
	SysTickConfig();

	raw_data raw;
	init_data init;
	gyro_data gyro;
	accel_data accel;
	uint8_t id;




    Write_Byte(MPU6050_Address, 0x6B, 0x40);
    //NVIC_Config();
     // Delay(100);
    //MPU6050_SetClockSource(1);
   // MPU6050_SetFullScaleAccelRange(uint8_t range)
   // MPU6050_SleepMode(DISABLE);
//    while(1)
//      {
//      id = MPU6050_GetDeviceID();
//      }
//		MPU6050_Init();
//      MPU6050_CalibrateSensor(&init);
//
//      while(1){
//      MPU6050_GetRawAccelGyro(&raw);
//      MPU6050_ConvertToFloat(&raw, &accel, &gyro);
//      }
while(1);

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
