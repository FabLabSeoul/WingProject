/**
  ******************************************************************************
  * @file    SPI/Simplex_Interrupt/platform_config.h 
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
#if defined (USE_STM3210B_EVAL) || defined (USE_STM3210E_EVAL) || defined(USE_STM32100B_EVAL)
  #define SPI_MASTER                   SPI1
  #define SPI_MASTER_CLK               RCC_APB2Periph_SPI1
  #define SPI_MASTER_GPIO              GPIOA
  #define SPI_MASTER_GPIO_CLK          RCC_APB2Periph_GPIOA  
  #define SPI_MASTER_PIN_SCK           GPIO_Pin_5
  #define SPI_MASTER_PIN_MISO          GPIO_Pin_6
  #define SPI_MASTER_PIN_MOSI          GPIO_Pin_7
  #define SPI_MASTER_IRQn              SPI1_IRQn
  
  #define SPI_SLAVE                    SPI2
  #define SPI_SLAVE_CLK                RCC_APB1Periph_SPI2
  #define SPI_SLAVE_GPIO               GPIOB
  #define SPI_SLAVE_GPIO_CLK           RCC_APB2Periph_GPIOB 
  #define SPI_SLAVE_PIN_SCK            GPIO_Pin_13
  #define SPI_SLAVE_PIN_MISO           GPIO_Pin_14
  #define SPI_SLAVE_PIN_MOSI           GPIO_Pin_15 
  #define SPI_SLAVE_IRQn               SPI2_IRQn
          
#elif defined (USE_STM3210C_EVAL)
  #define SPI_MASTER                   SPI3   /* SPI pins are remapped by software */
  #define SPI_MASTER_CLK               RCC_APB1Periph_SPI3
  #define SPI_MASTER_GPIO              GPIOC
  #define SPI_MASTER_GPIO_CLK          RCC_APB2Periph_GPIOC 
  #define SPI_MASTER_PIN_SCK           GPIO_Pin_10
  #define SPI_MASTER_PIN_MISO          GPIO_Pin_11
  #define SPI_MASTER_PIN_MOSI          GPIO_Pin_12
  #define SPI_MASTER_IRQn              SPI3_IRQn
  
  #define SPI_SLAVE                    SPI2
  #define SPI_SLAVE_CLK                RCC_APB1Periph_SPI2
  #define SPI_SLAVE_GPIO               GPIOB
  #define SPI_SLAVE_GPIO_CLK           RCC_APB2Periph_GPIOB
  #define SPI_SLAVE_PIN_SCK            GPIO_Pin_13
  #define SPI_SLAVE_PIN_MISO           GPIO_Pin_14
  #define SPI_SLAVE_PIN_MOSI           GPIO_Pin_15
  #define SPI_SLAVE_IRQn               SPI2_IRQn

#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
