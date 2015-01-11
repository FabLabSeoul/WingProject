/**
  @page RCC_Example RCC
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    RCC/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Description of the RCC Example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description 

This example shows how to configure the System clock(SYSCLK) to be 24MHz or HSE 
frequency.
The SYSCLK frequency is selected by user in main.h file.

It shows how to use, for debug purpose, the RCC_GetClocksFreq function to retrieve
the current status and frequencies of different on chip clocks. You can see the
RCC_ClockFreq structure content, which hold the frequencies of different on chip
clocks, using your toolchain debugger.

This example handles also the High Speed External clock (HSE) failure detection:
when the HSE clock disappears (broken or disconnected external Quartz); HSE, PLL
are disabled (but no change on PLL config), HSI selected as system clock source
and an interrupt (NMI) is generated. In the NMI ISR, the HSE, HSE ready interrupt
are enabled and once HSE clock recover, the HSERDY interrupt is generated and in
the RCC ISR routine the system clock is reconfigured to its previous state (before
HSE clock failure). 

Two LEDs are toggled with a timing defined by the Delay function.

@note To adjust the External High Speed oscillator (HSE) Startup Timeout value,
use HSEStartUp_TimeOut variable defined in the stm32f10x.h file.


@par Directory contents 

  - RCC/stm32f10x_conf.h     Library Configuration file
  - RCC/stm32f10x_it.c       Interrupt handlers
  - RCC/stm32f10x_it.h       Header for stm32f10x_it.c
  - RCC/main.h               Main header file
  - RCC/main.c               Main program


@par Hardware and Software environment 

  
  - This example has been tested with STM32VLDISCOVERY board.

  - STM32VLDISCORY board Set-up  
    - Use LD3 and LD4 connected respectively to PC.09 and PC.08 pins
      
   
@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_gpio.c 
  - stm32f10x_flash.c 
  - stm32f10x_rcc.c 
  - misc.c
  - STM32vldiscovery.c (under Utilities)
- Link all compiled files and load your image into target memory
- Run the example
   

   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
