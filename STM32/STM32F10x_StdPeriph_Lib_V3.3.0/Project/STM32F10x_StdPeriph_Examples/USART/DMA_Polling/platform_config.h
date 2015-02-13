/**
  ******************************************************************************
  * @file    USART/DMA_Polling/platform_config.h 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Evaluation board specific configuration file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
#if !defined (USE_STM32100B_EVAL) && !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL) &&  !defined (USE_STM3210C_EVAL)
 //#define USE_STM32100B_EVAL
 //#define USE_STM3210B_EVAL
 //#define USE_STM3210E_EVAL
 #define USE_STM3210C_EVAL 
#endif

/* Define the STM32F10x hardware depending on the used evaluation board */
#if defined(USE_STM3210B_EVAL) || defined(USE_STM32100B_EVAL)

  #define USARTy                   USART1
  #define USARTy_GPIO              GPIOA
  #define USARTy_CLK               RCC_APB2Periph_USART1
  #define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define USARTy_RxPin             GPIO_Pin_10
  #define USARTy_TxPin             GPIO_Pin_9
  #define USARTy_Tx_DMA_Channel    DMA1_Channel4
  #define USARTy_Tx_DMA_FLAG       DMA1_FLAG_TC4
  #define USARTy_Rx_DMA_Channel    DMA1_Channel5
  #define USARTy_Rx_DMA_FLAG       DMA1_FLAG_TC5  
  #define USARTy_DR_Base           0x40013804

  #define USARTz                   USART2
  #define USARTz_GPIO              GPIOD
  #define USARTz_CLK               RCC_APB1Periph_USART2
  #define USARTz_GPIO_CLK          RCC_APB2Periph_GPIOD
  #define USARTz_RxPin             GPIO_Pin_6
  #define USARTz_TxPin             GPIO_Pin_5
  #define USARTz_Tx_DMA_Channel    DMA1_Channel7
  #define USARTz_Tx_DMA_FLAG       DMA1_FLAG_TC7
  #define USARTz_Rx_DMA_Channel    DMA1_Channel6
  #define USARTz_Rx_DMA_FLAG       DMA1_FLAG_TC6 
  #define USARTz_DR_Base           0x40004404

#elif defined USE_STM3210E_EVAL

  #define USARTy                   USART1
  #define USARTy_GPIO              GPIOA
  #define USARTy_CLK               RCC_APB2Periph_USART1
  #define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define USARTy_RxPin             GPIO_Pin_10
  #define USARTy_TxPin             GPIO_Pin_9
  #define USARTy_Tx_DMA_Channel    DMA1_Channel4
  #define USARTy_Tx_DMA_FLAG       DMA1_FLAG_TC4
  #define USARTy_Rx_DMA_Channel    DMA1_Channel5
  #define USARTy_Rx_DMA_FLAG       DMA1_FLAG_TC5  
  #define USARTy_DR_Base           0x40013804

  #define USARTz                   USART2
  #define USARTz_GPIO              GPIOA
  #define USARTz_CLK               RCC_APB1Periph_USART2
  #define USARTz_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define USARTz_RxPin             GPIO_Pin_3
  #define USARTz_TxPin             GPIO_Pin_2
  #define USARTz_Tx_DMA_Channel    DMA1_Channel7
  #define USARTz_Tx_DMA_FLAG       DMA1_FLAG_TC7
  #define USARTz_Rx_DMA_Channel    DMA1_Channel6
  #define USARTz_Rx_DMA_FLAG       DMA1_FLAG_TC6  
  #define USARTz_DR_Base           0x40004404

#elif defined USE_STM3210C_EVAL

  #define USARTy                   USART2
  #define USARTy_GPIO              GPIOD
  #define USARTy_CLK               RCC_APB1Periph_USART2
  #define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOD
  #define USARTy_RxPin             GPIO_Pin_6
  #define USARTy_TxPin             GPIO_Pin_5
  #define USARTy_Tx_DMA_Channel    DMA1_Channel7
  #define USARTy_Tx_DMA_FLAG       DMA1_FLAG_TC7
  #define USARTy_Rx_DMA_Channel    DMA1_Channel6
  #define USARTy_Rx_DMA_FLAG       DMA1_FLAG_TC6  
  #define USARTy_DR_Base           0x40004404

  #define USARTz                   USART3
  #define USARTz_GPIO              GPIOC
  #define USARTz_CLK               RCC_APB1Periph_USART3
  #define USARTz_GPIO_CLK          RCC_APB2Periph_GPIOC
  #define USARTz_RxPin             GPIO_Pin_11
  #define USARTz_TxPin             GPIO_Pin_10
  #define USARTz_Tx_DMA_Channel    DMA1_Channel2
  #define USARTz_Tx_DMA_FLAG       DMA1_FLAG_TC2
  #define USARTz_Rx_DMA_Channel    DMA1_Channel3
  #define USARTz_Rx_DMA_FLAG       DMA1_FLAG_TC3  
  #define USARTz_DR_Base           0x40004804
  
#endif /* USE_STM3210B_EVAL */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
