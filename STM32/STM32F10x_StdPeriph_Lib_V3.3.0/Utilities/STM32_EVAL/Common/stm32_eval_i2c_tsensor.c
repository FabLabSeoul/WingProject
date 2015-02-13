/**
  ******************************************************************************
  * @file    stm32_eval_i2c_tsensor.c
  * @author  MCD Application Team
  * @version V4.2.0
  * @date    04/16/2010
  * @brief   This file provides a set of functions needed to manage the I2C LM75 
  *          temperature sensor mounted on STM32xx-EVAL board (refer to stm32_eval.h
  *          to know about the boards supporting this sensor). 
  *          It implements a high level communication layer for read and write 
  *          from/to this sensor. The needed STM32 hardware resources (I2C and 
  *          GPIO) are defined in stm32xx_eval.h file, and the initialization is 
  *          performed in LM75_LowLevel_Init() function declared in stm32xx_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          LM75_LowLevel_Init() function.
  *
  *     +-----------------------------------------------------------------+
  *     |                        Pin assignment                           |                 
  *     +---------------------------------------+-----------+-------------+
  *     |  STM32 I2C Pins                       |   STLM75  |   Pin       |
  *     +---------------------------------------+-----------+-------------+
  *     | LM75_I2C_SDA_PIN/ SDA                 |   SDA     |    1        |
  *     | LM75_I2C_SCL_PIN/ SCL                 |   SCL     |    2        |
  *     | LM75_I2C_SMBUSALERT_PIN/ SMBUS ALERT  |   OS/INT  |    3        |
  *     | .                                     |   GND     |    4  (0V)  |
  *     | .                                     |   GND     |    5  (0V)  |
  *     | .                                     |   GND     |    6  (0V)  |
  *     | .                                     |   GND     |    7  (0V)  |
  *     | .                                     |   VDD     |    8  (3.3V)|
  *     +---------------------------------------+-----------+-------------+
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32_eval_i2c_tsensor.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */
  
/** @addtogroup STM32_EVAL_I2C_TSENSOR
  * @brief      This file includes the LM75 Temperature Sensor driver of 
  *             STM32-EVAL boards.
  * @{
  */ 

/** @defgroup STM32_EVAL_I2C_TSENSOR_Private_Types
  * @{
  */ 
/**
  * @}
  */

/** @defgroup STM32_EVAL_I2C_TSENSOR_Private_Defines
  * @{
  */ 
#define LM75_SD_SET                0x01 /*!< Set SD bit in the configuration register */
#define LM75_SD_RESET              0xFE /*!< Reset SD bit in the configuration register */
/**
  * @}
  */ 

/** @defgroup STM32_EVAL_I2C_TSENSOR_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 
  
/** @defgroup STM32_EVAL_I2C_TSENSOR_Private_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32_EVAL_I2C_TSENSOR_Private_Function_Prototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_I2C_TSENSOR_Private_Functions
  * @{
  */ 

/**
  * @brief  DeInitializes the LM75_I2C.
  * @param  None
  * @retval None
  */
void LM75_DeInit(void)
{
  LM75_LowLevel_DeInit();
}

/**
  * @brief  Initializes the LM75_I2C.
  * @param  None
  * @retval None
  */
void LM75_Init(void)
{
  I2C_InitTypeDef   I2C_InitStructure;
  
  LM75_LowLevel_Init();
  
  I2C_DeInit(LM75_I2C);

  /*!< LM75_I2C Init */
  I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = LM75_I2C_SPEED;
  I2C_Init(LM75_I2C, &I2C_InitStructure);

  /*!< Enable SMBus Alert interrupt */
  I2C_ITConfig(LM75_I2C, I2C_IT_ERR, ENABLE);

  /*!< LM75_I2C Init */
  I2C_Cmd(LM75_I2C, ENABLE);
}

/**
  * @brief  Checks the LM75 status.
  * @param  None
  * @retval ErrorStatus: LM75 Status (ERROR or SUCCESS).
  */
ErrorStatus LM75_GetStatus(void)
{
  uint32_t I2C_TimeOut = I2C_TIMEOUT;

  /*!< Clear the LM75_I2C AF flag */
  I2C_ClearFlag(LM75_I2C, I2C_FLAG_AF);

  /*!< Enable LM75_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(LM75_I2C, ENABLE);

  /*---------------------------- Transmission Phase ---------------------------*/

  /*!< Send LM75_I2C START condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on LM75_I2C EV5 and clear it */
  while ((!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT)) && I2C_TimeOut)  /*!< EV5 */
  {
    I2C_TimeOut--;
  }
  if (I2C_TimeOut == 0)
  {
    return ERROR;
  }
  I2C_TimeOut = I2C_TIMEOUT;


  /*!< Send STLM75 slave address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);

  while ((!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && I2C_TimeOut)/* EV6 */
  {
    I2C_TimeOut--;
  }

  if ((I2C_GetFlagStatus(LM75_I2C, I2C_FLAG_AF) != 0x00) || (I2C_TimeOut == 0))
  {
    return ERROR;
  }
  else
  {
    return SUCCESS;
  }
}
/**
  * @brief  Read the specified register from the LM75.
  * @param  RegName: specifies the LM75 register to be read.
  *              This member can be one of the following values:  
  *                  - LM75_REG_TEMP: temperature register
  *                  - LM75_REG_TOS: Over-limit temperature register
  *                  - LM75_REG_THYS: Hysteresis temperature register
  * @retval LM75 register value.
  */
uint16_t LM75_ReadReg(uint8_t RegName)
{
  __IO uint16_t RegValue = 0;

  /*!< Enable LM75_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(LM75_I2C, ENABLE);

  /*--------------------------- Transmission Phase ----------------------------*/
  /*!< Send LM75_I2C START condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on LM75_I2C EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);

  /*!< Test on LM75_I2C EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /*!< EV6 */
  {
  }

  /*!< Send the specified register data pointer */
  I2C_SendData(LM75_I2C, RegName);

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*------------------------------ Reception Phase ----------------------------*/
  /*!< Send Re-STRAT condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for read */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Receiver);

  /*!< Test on EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /*!< EV6 */
  {
  }

  /*!< Test on EV7 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))  /*!< EV7 */
  {
  }

  /*!< Store LM75_I2C received data */
  RegValue = (uint16_t)(I2C_ReceiveData(LM75_I2C) << 8);

  /*!< Disable LM75_I2C acknowledgement */
  I2C_AcknowledgeConfig(LM75_I2C, DISABLE);

  /*!< Send LM75_I2C STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);

  /*!< Test on RXNE flag */
  while (I2C_GetFlagStatus(LM75_I2C, I2C_FLAG_RXNE) == RESET)
  {
  }

  /*!< Store LM75_I2C received data */
  RegValue |= I2C_ReceiveData(LM75_I2C);

  /*!< Return register value */
  return (RegValue);
}

/**
  * @brief  Write to the specified register of the LM75.
  * @param  RegName: specifies the LM75 register to be written.
  *              This member can be one of the following values:    
  *                  - LM75_REG_TOS: Over-limit temperature register
  *                  - LM75_REG_THYS: Hysteresis temperature register
  * @param  RegValue: value to be written to LM75 register.  
  * @retval None
  */
void LM75_WriteReg(uint8_t RegName, uint16_t RegValue)
{
  /*-------------------------------- Transmission Phase -----------------------*/
  /*!< Send LM75_I2C START condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on LM75_I2C EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);

  /*!< Test on LM75_I2C EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /*!< EV6 */
  {
  }

  /*!< Send the specified register data pointer */
  I2C_SendData(LM75_I2C, RegName);

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*!< Send LM75_I2C data */
  I2C_SendData(LM75_I2C, (uint8_t)(RegValue >> 8));

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*!< Send LM75_I2C data */
  I2C_SendData(LM75_I2C, (uint8_t)RegValue);

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*!< Send LM75_I2C STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);
}

/**
  * @brief  Read Temperature register of LM75: double temperature value.
  * @param  None
  * @retval LM75 measured temperature value.
  */
uint16_t LM75_ReadTemp(void)
{
  __IO uint16_t RegValue = 0;

  /*!< Enable LM75_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(LM75_I2C, ENABLE);

  /*------------------------------------- Transmission Phase ------------------*/
  /*!< Send LM75_I2C START condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on LM75_I2C EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);

  /*!< Test on LM75_I2C EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /*!< EV6 */
  {
  }

  /*!< Send the temperature register data pointer */
  I2C_SendData(LM75_I2C, LM75_REG_TEMP);

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*-------------------------------- Reception Phase --------------------------*/
  /*!< Send Re-STRAT condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for read */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Receiver);

  /*!< Test on EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /*!< EV6 */
  {
  }

  /*!< Test on EV7 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))  /*!< EV7 */
  {
  }

  /*!< Store LM75_I2C received data */
  RegValue = I2C_ReceiveData(LM75_I2C) << 8;

  /*!< Disable LM75_I2C acknowledgement */
  I2C_AcknowledgeConfig(LM75_I2C, DISABLE);

  /*!< Send LM75_I2C STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);

  /*!< Test on RXNE flag */
  while (I2C_GetFlagStatus(LM75_I2C, I2C_FLAG_RXNE) == RESET)
  {
  }

  /*!< Store LM75_I2C received data */
  RegValue |= I2C_ReceiveData(LM75_I2C);

  /*!< Return Temperature value */
  return (RegValue >> 7);
}

/**
  * @brief  Read the configuration register from the LM75.
  * @param  None
  * @retval LM75 configuration register value.
  */
uint8_t LM75_ReadConfReg(void)
{
  __IO uint8_t RegValue = 0;

  /*!< Enable LM75_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(LM75_I2C, ENABLE);
  /*----------------------------- Transmission Phase --------------------------*/
  /*!< Send LM75_I2C START condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on LM75_I2C EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);

  /*!< Test on LM75_I2C EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /*!< EV6 */
  {
  }

  /*!< Send the configuration register data pointer */
  I2C_SendData(LM75_I2C, LM75_REG_CONF);

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*----------------------------- Reception Phase -----------------------------*/
  /*!< Send Re-STRAT condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for read */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Receiver);

  /*!< Test on EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /*!< EV6 */
  {
  }

  /*!< Disable LM75_I2C acknowledgement */
  I2C_AcknowledgeConfig(LM75_I2C, DISABLE);

  /*!< Send LM75_I2C STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);

  /*!< Test on RXNE flag */
  while (I2C_GetFlagStatus(LM75_I2C, I2C_FLAG_RXNE) == RESET);

  /*!< Store LM75_I2C received data */
  RegValue = I2C_ReceiveData(LM75_I2C);

  /*!< Return configuration register value */
  return (RegValue);
}

/**
  * @brief  Write to the configuration register of the LM75.
  * @param  RegValue: sepecifies the value to be written to LM75 configuration 
  *         register.
  * @retval None
  */
void LM75_WriteConfReg(uint8_t RegValue)
{
  /*-------------------------------- Transmission Phase -----------------------*/
  /*!< Send LM75_I2C START condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on LM75_I2C EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);

  /*!< Test on LM75_I2C EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /*!< EV6 */
  {
  }

  /*!< Send the configuration register data pointer */
  I2C_SendData(LM75_I2C, LM75_REG_CONF);

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*!< Send LM75_I2C data */
  I2C_SendData(LM75_I2C, RegValue);

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*!< Send LM75_I2C STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);
}

/**
  * @brief  Enables or disables the LM75.
  * @param  NewState: specifies the LM75 new status. This parameter can be ENABLE
  *         or DISABLE.  
  * @retval None
  */
void LM75_ShutDown(FunctionalState NewState)
{
  __IO uint8_t RegValue = 0;

  /*---------------------------- Transmission Phase ---------------------------*/
  /*!< Send LM75_I2C START condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on LM75_I2C EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);

  /*!< Test on LM75_I2C EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /*!< EV6 */
  {
  }

  /*!< Send the configuration register data pointer */
  I2C_SendData(LM75_I2C, LM75_REG_CONF);

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  /*!< EV8 */
  {
  }

  /*-------------------------------- Reception Phase --------------------------*/
  /*!< Send Re-STRAT condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for read */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Receiver);

  /*!< Test on EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /*!< EV6 */
  {
  }

  /*!< Disable LM75_I2C acknowledgement */
  I2C_AcknowledgeConfig(LM75_I2C, DISABLE);

  /*!< Send LM75_I2C STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);

  /*!< Test on RXNE flag */
  while (I2C_GetFlagStatus(LM75_I2C, I2C_FLAG_RXNE) == RESET);

  /*!< Store LM75_I2C received data */
  RegValue = I2C_ReceiveData(LM75_I2C);

  /*------------------------------------ Transmission Phase -------------------*/
  /*!< Send LM75_I2C START condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on LM75_I2C EV5 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /*!< EV5 */
  {
  }

  /*!< Send STLM75 slave address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);

  /*!< Test on LM75_I2C EV6 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /*!< EV6 */
  {
  }

  /*!< Send the configuration register data pointer */
  I2C_SendData(LM75_I2C, LM75_REG_CONF);

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*!< Enable or disable SD bit */
  if (NewState != DISABLE)
  {
    /*!< Enable LM75 */
    I2C_SendData(LM75_I2C, RegValue & LM75_SD_RESET);
  }
  else
  {
    /*!< Disable LM75 */
    I2C_SendData(LM75_I2C, RegValue | LM75_SD_SET);
  }

  /*!< Test on LM75_I2C EV8 and clear it */
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /*!< EV8 */
  {
  }

  /*!< Send LM75_I2C STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);

}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
