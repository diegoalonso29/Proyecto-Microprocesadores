#include "stm32l1xx_nucleo.h"

#ifndef MPU_6050_H_
#define MPU_6050_H_

#define MPU6050_Address 		0x68
#define MPU6050_I2C_Speed 		100000

#define I2C_GPIO				GPIOB
#define I2C_SCL_PinSource		GPIO_PinSource8
#define I2C_SDA_PinSource		GPIO_PinSource9
#define I2C_SCL_Pin				GPIO_Pin_8
#define I2C_SDA_Pin				GPIO_Pin_9

#define I2Cx					I2C1
#define RCC_APB1Periph_I2C		RCC_APB1Periph_I2C1
#define RCC_AHBPeriph_GPIO_I2C	RCC_AHBPeriph_GPIOB
#define GPIO_AF_I2Cx			GPIO_AF_I2C1
#define I2Cx_EV_IRQn			I2C1_EV_IRQn


void i2c_begin(uint32_t I2C_ClockSpeed);
void NVIC_config(void);
void I2C_ByteWrite(uint8_t SlaveAddress, uint8_t* Buffer_ptr, uint8_t AddressWriteReg);
void I2C_ByteRead(uint8_t SlaveAddress, uint8_t* Buffer_ptr, uint8_t AddressReadReg, uint16_t NumByteToRead);


#endif /* MPU_6050_H_ */
