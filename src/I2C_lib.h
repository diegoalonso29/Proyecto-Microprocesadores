#ifndef I2C_LIB_H_
#define I2C_LIB_H_

#include "stm32l1xx_nucleo.h"
#include "stm32l1xx.h"
#include "stm32l1xx_gpio.h"

#define I2C_TIMEOUT				20000

#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

#define FLAG_MASK               ((uint32_t)0x00FFFFFF)  /*<! I2C FLAG mask */

/*********************************************** ERROR FLOW CONTROL ******************************************/


typedef enum {

	I2C_NoError = 0,
	I2C_DeviceNotConnected = 1,
	I2C_AddressDeviceInvalid = 2,
	I2C_StartBit_TimeOut = 3,
	I2C_AddressTransfer_Timeout = 4,
	I2C_TXE_Timeout = 5,
	I2C_ReceiveEvent_Timeout = 6,
	I2C_End_Timeout = 7,
	I2C_WhoIam_Error = 8

} I2C_Error_Code;

/************************************** PUBLIC FUNCTIIONS ************************************************************/

void I2C_InitConfig();
I2C_Error_Code I2C_IsConnected(I2C_TypeDef* I2Cx, uint8_t SlaveAddress);

/* General purpose I2C functions */
I2C_Error_Code I2C_ReadByte(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t* data);
I2C_Error_Code I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t* data, uint16_t count);

I2C_Error_Code I2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t data);
I2C_Error_Code I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t* data, uint16_t count);


/* Functions focused on communication with slave registers */
I2C_Error_Code I2C_ReadByte_Reg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t* data);
I2C_Error_Code I2C_ReadByte_MultiReg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint16_t count);
I2C_Error_Code I2C_ReadBits_Reg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t BitStart, uint8_t length, uint8_t *data);

I2C_Error_Code I2C_WriteByte_Reg(I2C_TypeDef* I2Cx, uint8_t SlavAddress, uint8_t reg, uint8_t data);
I2C_Error_Code I2C_WriteByte_MultiReg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint16_t count);
I2C_Error_Code I2C_WriteBits_Reg(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t reg, uint8_t BitStart, uint8_t length, uint8_t data);


/********************************************* PRIVATE FUNCTIONS *****************************************************/

I2C_Error_Code I2C_Start(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t direction, uint8_t ack);
I2C_Error_Code I2C_Stop(I2C_TypeDef* I2Cx);
I2C_Error_Code I2C_ReadAck(I2C_TypeDef* I2Cx, uint8_t* data);
I2C_Error_Code I2C_ReadNack(I2C_TypeDef* I2Cx, uint8_t* data);
I2C_Error_Code I2C_Write(I2C_TypeDef* I2Cx, uint8_t data);

ErrorStatus I2C_TestEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);

#endif /* I2C_LIB_H_ */
