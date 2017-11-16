


#include "SPI_lib.h"

/**
 * @addtogroup SPI1
 */

/*
  @brief Initialize SPI1 and SS pin.
 */

void SPI1_Init(void) {
	// Enable GPIO clock for SPI pins
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/*
	 * Initialize pins as alternate function, push-pull.
	 * PA5 = SCK (arduino D13)
	 * PA6 = MISO (arduino D12)
	 * PA7 = MOSI (arduino D11)
	 * PB6 = SS (arduino D10)
	 */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_40MHz;
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Enable alternate functions of pins
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	// Software chip select pin, PB6 = SS
	GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_40MHz;
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_SetBits(GPIOB, GPIO_Pin_6); // Set SS line

	// Enable SPI1 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitTypeDef SPI_InitStruct;

	SPI_InitStruct.SPI_Direction          = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode               = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize           = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL               = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA               = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS                = SPI_NSS_Soft; // software chip select
	SPI_InitStruct.SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_256;
	SPI_InitStruct.SPI_FirstBit           = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial      = 7;
	SPI_Init(SPI1, &SPI_InitStruct);

	SPI_CalculateCRC(SPI1, DISABLE); //disabling of CRC checking
	SPI_Cmd(SPI1, ENABLE); // enable SPI1

}
/**
 * @brief Select chip.
 */
void SPI1_Select(void) {

	GPIO_ResetBits(GPIOB, GPIO_Pin_6); // Reset SS line

}
/**
 * @brief Deselect chip.
 */
void SPI1_Deselect(void) {

	GPIO_SetBits(GPIOB, GPIO_Pin_6); // Set SS line
}
/**
 * @brief Transmit data on SPI1
 * @param data Sent data
 * @return Received data
 * @warning Blocking function!
 */
uint8_t SPI1_Transmit(uint8_t data) {

	uint8_t i;

	// Loop while transmit register in not empty
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	SPI_I2S_SendData(SPI1, data); // Send byte (start transmit)

	// Wait for new data (transmit end)
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	i = SPI_I2S_ReceiveData(SPI1); // Received data

	return i;
}
/**
 * @brief Send multiple data on SPI1.
 * @param buf Buffer to send.
 * @param len Number of bytes to send.
 * @warning Blocking function!
 */
void SPI1_SendBuffer(uint8_t* buf, uint32_t len) {

	while (len--) {
		SPI1_Transmit(*buf++);
	}
}
/**
 * @brief Read multiple data on SPI1.
 * @param buf Buffer to place read data.
 * @param len Number of bytes to read.
 * @warning Blocking function!
 */
void SPI1_ReadBuffer(uint8_t* buf, uint32_t len) {

	while (len--) {
		*buf++ = SPI1_Transmit(0xff);
	}
}
/**
 * @brief Write multiple data on SPI1.
 * @param buf Buffer with write data.
 * @param len Number of bytes to write.
 * @warning Blocking function!
 */
void SPI1_WriteBuffer(uint8_t* buf, uint32_t len) {

	while (len--) {
		SPI1_Transmit(*buf++);
	}
}
/**
 * @brief Transmit multiple data on SPI1.
 * @param rx_buf Receive buffer.
 * @param tx_buf Transmit buffer.
 * @param len Number of bytes to transmit.
 */
void SPI1_TransmitBuffer(uint8_t* rx_buf, uint8_t* tx_buf, uint32_t len) {

	while (len--) {
		*rx_buf = SPI1_Transmit(*tx_buf);
		tx_buf++;
		rx_buf++;
	}
}

/**
 * @}
 */
