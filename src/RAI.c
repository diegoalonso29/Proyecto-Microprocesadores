#include "RAI.h"

void I2C1_ER_IRQHandler(void)
{

  /* Check on I2C1 Time out flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_TIMEOUT);
  }
  /* Check on I2C1 Arbitration Lost flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_ARLO))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_ARLO);
  }

  /* Check on I2C1 PEC error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_PECERR))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_PECERR);
  }
  /* Check on I2C1 Overrun/Underrun error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_OVR))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_OVR);
  }
  /* Check on I2C1 Acknowledge failure error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_AF))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
  }
  /* Check on I2C1 Bus error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_BERR))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
  }
}



