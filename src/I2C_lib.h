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

/***************************************** I2C EVENTS ****************************************************************/
/* --EV5 */
#define  I2C_EVENT_MASTER_MODE_SELECT                      ((uint32_t)0x00030001)  /* BUSY, MSL and SB flag */
/* --EV6 */
#define  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED        ((uint32_t)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED           ((uint32_t)0x00030002)  /* BUSY, MSL and ADDR flags */
/* --EV9 */
#define  I2C_EVENT_MASTER_MODE_ADDRESS10                   ((uint32_t)0x00030008)  /* BUSY, MSL and ADD10 flags */

/* Master RECEIVER mode -----------------------------*/
/* --EV7 */
#define  I2C_EVENT_MASTER_BYTE_RECEIVED                    ((uint32_t)0x00030040)  /* BUSY, MSL and RXNE flags */

/* Master TRANSMITTER mode --------------------------*/
/* --EV8 */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTING                 ((uint32_t)0x00070080) /* TRA, BUSY, MSL, TXE flags */
/* --EV8_2 */
#define  I2C_EVENT_MASTER_BYTE_TRANSMITTED                 ((uint32_t)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */

/* --EV1  (all the events below are variants of EV1) */
/* 1) Case of One Single Address managed by the slave */
#define  I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED          ((uint32_t)0x00020002) /* BUSY and ADDR flags */
#define  I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED       ((uint32_t)0x00060082) /* TRA, BUSY, TXE and ADDR flags */

/* 2) Case of Dual address managed by the slave */
#define  I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED    ((uint32_t)0x00820000)  /* DUALF and BUSY flags */
#define  I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED ((uint32_t)0x00860080)  /* DUALF, TRA, BUSY and TXE flags */

/* 3) Case of General Call enabled for the slave */
#define  I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED        ((uint32_t)0x00120000)  /* GENCALL and BUSY flags */

/* Slave RECEIVER mode --------------------------*/
/* --EV2 */
#define  I2C_EVENT_SLAVE_BYTE_RECEIVED                     ((uint32_t)0x00020040)  /* BUSY and RXNE flags */
/* --EV4  */
#define  I2C_EVENT_SLAVE_STOP_DETECTED                     ((uint32_t)0x00000010)  /* STOPF flag */

/* Slave TRANSMITTER mode -----------------------*/
/* --EV3 */
#define  I2C_EVENT_SLAVE_BYTE_TRANSMITTED                  ((uint32_t)0x00060084)  /* TRA, BUSY, TXE and BTF flags */
#define  I2C_EVENT_SLAVE_BYTE_TRANSMITTING                 ((uint32_t)0x00060080)  /* TRA, BUSY and TXE flags */
/* --EV3_2 */
#define  I2C_EVENT_SLAVE_ACK_FAILURE                       ((uint32_t)0x00000400)  /* AF flag */


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
