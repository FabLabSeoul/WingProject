/**
  @page SysTick_Example SysTick
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    SysTick/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Description of the SysTick Example.
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

This example shows how to configure the SysTick to generate a time base equal to
1 ms. The system clock is set to 24 MHz. The SysTick is clocked by the AHB clock (HCLK).

A "Delay" function is implemented based on the SysTick end-of-count event.
Two LEDs are toggled with a timing defined by the Delay function.

@par Directory contents 

  - SysTick/stm32f10x_conf.h     Library Configuration file
  - SysTick/stm32f10x_it.c       Interrupt handlers
  - SysTick/stm32f10x_it.h       Header for stm32f10x_it.c
  - SysTick/main.c               Main program
  - SysTick/main.h               Header for main.c

@par Hardware and Software environment 

  
  - This example has been tested with STM32VLDISCOVERY board.
      
  - STM32VLDISCOVERY board
    - Use LD3 and LD4 connected respectively to PC.09 and PC.08 pins
      

          
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - misc.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
  - STM32vldiscovery.c (under Utilities)
- Link all compiled files and load your image into target memory
- Run the example


   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
