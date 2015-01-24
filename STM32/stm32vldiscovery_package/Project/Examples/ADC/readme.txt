/**
  @page GPIOToggle GPIOToggle
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    Examples/GPIOToggle/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date   09/13/2010
  * @brief   Description of the IOToggle Example.
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

This example describes how to use GPIO BSRR (Port bit set/reset register) and 
BRR (Port bit reset register) for IO toggling.
These registers allow modifying only one or several GPIO pins in a single atomic
write access.
Four LEDs are toggled in an infinite loop.

@par Directory contents 

  - GPIOToggle/stm32f10x_conf.h     Library Configuration file
  - GPIOToggle/stm32f10x_it.c       Interrupt handlers
  - GPIOToggle/stm32f10x_it.h       Header for stm32f10x_it.c
  - GPIOToggle/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32VLDISCOVERY board.
  
  - This example has been tested with STMicroelectronics STM32VLDISCOVERY board
    and can be easily tailored to any other supported device and development board.


  - STM32VLDISCOVERY  Set-up  
    - Use LD3 and LD4 connected respectively to PC.09 and PC.08 pins


@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c 
  - stm32f10x_exti.c
  - stm32f10x_rcc.c	   
  - misc.c	  
  - STM32vldiscovery.c       (under Utilities)
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)   
- Link all compiled files and load your image into target memory
- Run the example

 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
