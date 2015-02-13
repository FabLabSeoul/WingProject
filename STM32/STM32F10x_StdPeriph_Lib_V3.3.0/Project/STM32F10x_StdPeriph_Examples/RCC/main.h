/**
  ******************************************************************************
  * @file    RCC/main.h 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source) */
//#define SYSCLK_HSE
#define SYSCLK_FREQ_24MHz

#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL
  //#define SYSCLK_FREQ_36MHz
  //#define SYSCLK_FREQ_48MHz
  //#define SYSCLK_FREQ_56MHz
  #define SYSCLK_FREQ_72MHz
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
