#include "MPU6050.h"
#include "I2C_lib.h"

uint8_t MPU6050_GetDeviceID()
{
    uint8_t tmp;
    I2C_ReadBits(MPU6050_Address, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, &tmp);
    return tmp;
}

uint8_t MPU6050_TestConnection()
{
    return MPU6050_GetDeviceID() == 0x34 ? 0x01 : 0x00; //0b110100; 8-bit representation in hex = 0x34
}

/** Set clock source setting.
 * An internal 8MHz oscillator, gyroscope based clock, or external sources can
 * be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator
 * or an external source is chosen as the clock source, the MPU-60X0 can operate
 * in low power modes with the gyroscopes disabled.
 *
 * Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
 * However, it is highly recommended that the device be configured to use one of
 * the gyroscopes (or an external clock source) as the clock reference for
 * improved stability. The clock source can be selected according to the following table:
 *
 * <pre>
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
 * </pre>
 *
 * @param source New clock source setting
 * @see MPU6050_GetClockSource()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_CLKSEL_BIT
 * @see MPU6050_PWR1_CLKSEL_LENGTH
 */
void MPU6050_SetClockSource(uint8_t source)
{
    I2C_WriteBits(MPU6050_Address, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see MPU6050_GetFullScaleGyroRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
void MPU6050_SetFullScaleGyroRange(uint8_t range)
{
    I2C_WriteBits(MPU6050_Address, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

// GYRO_CONFIG register

/** Get full-scale gyroscope range.
 * The FS_SEL parameter allows setting the full-scale range of the gyro sensors,
 * as described in the table below.
 *
 * <pre>
 * 0 = +/- 250 degrees/sec
 * 1 = +/- 500 degrees/sec
 * 2 = +/- 1000 degrees/sec
 * 3 = +/- 2000 degrees/sec
 * </pre>
 *
 * @return Current full-scale gyroscope range setting
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
uint8_t MPU6050_GetFullScaleGyroRange()
{
    uint8_t tmp;
    I2C_ReadBits(MPU6050_Address, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, &tmp);
    return tmp;
}

/** Get full-scale accelerometer range.
 * The FS_SEL parameter allows setting the full-scale range of the accelerometer
 * sensors, as described in the table below.
 *
 * <pre>
 * 0 = +/- 2g
 * 1 = +/- 4g
 * 2 = +/- 8g
 * 3 = +/- 16g
 * </pre>
 *
 * @return Current full-scale accelerometer range setting
 * @see MPU6050_ACCEL_FS_2
 * @see MPU6050_RA_ACCEL_CONFIG
 * @see MPU6050_ACONFIG_AFS_SEL_BIT
 * @see MPU6050_ACONFIG_AFS_SEL_LENGTH
 */
uint8_t MPU6050_GetFullScaleAccelRange()
{
    uint8_t tmp;
    I2C_ReadBits(MPU6050_Address, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, &tmp);
    return tmp;
}

/** Set full-scale accelerometer range.
 * @param range New full-scale accelerometer range setting
 * @see MPU6050_GetFullScaleAccelRange()
 */
void MPU6050_SetFullScaleAccelRange(uint8_t range)
{
    I2C_WriteBits(MPU6050_Address, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/** Get sleep mode status.
 * Setting the SLEEP bit in the register puts the device into very low power
 * sleep mode. In this mode, only the serial interface and internal registers
 * remain active, allowing for a very low standby current. Clearing this bit
 * puts the device back into normal mode. To save power, the individual standby
 * selections for each of the gyros should be used if any gyro axis is not used
 * by the application.
 * @return Current sleep mode enabled status
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_SLEEP_BIT
 */
uint8_t MPU6050_GetSleepModeStatus()
{
    uint8_t tmp;
    I2C_ReadBit(MPU6050_Address, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, &tmp);
    return tmp == 0x00 ? 0x00 : 0x01;
}


void MPU6050_SleepMode(FunctionalState NewState)
{
    I2C_WriteBit(MPU6050_Address, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, !NewState);
}

void MPU6050_GetRawAccelGyro(raw_data* data)
{
    uint8_t tmpBuffer[14];

    I2C_ReadData(MPU6050_Address, MPU6050_RA_ACCEL_XOUT_H, &tmpBuffer[0],  14);
    /*
     * 10010111 00000000
     * 00000000 10011001
     * ------------------ OR
     * 10010111 10011001
     */
    data->raw_accel_x = ( ((int16_t)tmpBuffer[0] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[1] & 0xFFFF);
    data->raw_accel_y = ( ((int16_t)tmpBuffer[2] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[3] & 0xFFFF);
    data->raw_accel_z = ( ((int16_t)tmpBuffer[4] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[5] & 0xFFFF);

    data->raw_gyro_x = ( ((int16_t)tmpBuffer[6] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[7] & 0xFFFF);
    data->raw_gyro_y = ( ((int16_t)tmpBuffer[8] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[9] & 0xFFFF);
    data->raw_gyro_z = ( ((int16_t)tmpBuffer[10] << 8) & 0xFFFF) | ((int16_t)tmpBuffer[11] & 0xFFFF);

    data->raw_temp = ( ((int16_t)tmpBuffer[12] << 8) & 0xFFFF ) | ((int16_t)tmpBuffer[13] & 0xFFFF);
}

void MPU6050_CalibrateSensor()
{
	  int                   num_readings = 10;
	  float                 x_accel = 0;
	  float                 y_accel = 0;
	  float                 z_accel = 0;
	  float                 x_gyro = 0;
	  float                 y_gyro = 0;
	  float                 z_gyro = 0;
	  IMU data;

	  os_printf("Starting Calibration\n");
	  // Give MPU6050 time to initialize.
	  vTaskDelay(200);
	  // Discard the first set of values read from the IMU
	  readIMU(&data);

	  // Read and average the raw values from the IMU
	  for (int i = 0; i < num_readings; i++) {
		  readIMU(&data);
	    x_accel += data.x_accel;
	    y_accel += data.y_accel;
	    z_accel += data.z_accel;
	    x_gyro += data.x_gyro;
	    y_gyro += data.y_gyro;
	    z_gyro += data.z_gyro;
	    vTaskDelay(100);
	  }
	  x_accel /= num_readings;
	  y_accel /= num_readings;
	  z_accel /= num_readings;
	  x_gyro /= num_readings;
	  y_gyro /= num_readings;
	  z_gyro /= num_readings;

	  // Store the raw calibration values globally
	  base_x_accel = x_accel;
	  base_y_accel = y_accel;
	  base_z_accel = z_accel;
	  base_x_gyro = x_gyro;
	  base_y_gyro = y_gyro;
	  base_z_gyro = z_gyro;

	  os_printf("Finishing Calibration\n");
}


void MPU6050_Init(void)
{
	MPU6050_SleepMode(DISABLE);
	MPU6050_SetFullScaleAccelRange(1);
	MPU6050_SetFullScaleGyroRange(1);
	MPU6050_CalibrateSensor();
}
