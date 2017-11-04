#include "I2C.h"

void i2c_begin(){

	GPIO_InitTypeDef gpio_init_struct;
    I2C_InitTypeDef i2c_init_struct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/*Asignamos la funcion alterna de GPIO_AF_I2C1 a ambos pines para realizar comunicacion*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9, GPIO_AF_I2C1);

	/* Configuracion Pin8 (SCL) */
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType =GPIO_OType_OD;
	gpio_init_struct.GPIO_Pin= GPIO_Pin_8;
	gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_Init(GPIOB,&gpio_init_struct);

	/*Configuracion Pin9 (SDA)*/
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType =GPIO_OType_OD;
	gpio_init_struct.GPIO_Pin= GPIO_Pin_9;
	gpio_init_struct.GPIO_PuPd= GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_Init(GPIOB,&gpio_init_struct);

    /*Configuracion de la comunicacion I2C */
	i2c_init_struct.I2C_Ack = I2C_Ack_Enable;
	i2c_init_struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_init_struct.I2C_ClockSpeed = 2000;
	i2c_init_struct.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_init_struct.I2C_Mode = I2C_Mode_I2C;
	i2c_init_struct.I2C_OwnAddress1 = 0;

	I2C_Cmd(I2C1,ENABLE);

}

void NVIC_config(){

	/*Configuracion de NVIC para la interrupcion I2C */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



