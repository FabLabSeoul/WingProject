/**
  @page Sleep Sleep Mode
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    Sleep Mode/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    09/13/2010
  * @brief   Description of the Sleep mode Example.
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

This example shows how to enter the system to Sleep mode and wake-up from this
mode using EXTI line interrupt.

In the associated software, the system clock is set to 24 MHz, the EXTI line 0
is configured to generate an interrupt on falling edge.
The system is entered in Sleep mode using the WFI entry mode.

When a falling edge is detected on the EXTI line0, an interrupt is generated and the
system wakes up from Sleep mode and LD4 is turned on.

Two leds LED3 and LED4 are used to monitor the system state as following:
 - LED3 on: system in Sleep mode
 - LED4 on: system resumed from Sleep mode

@par Directory contents 

  - STANDBY Mode/stm32f10x_conf.h     Library Configuration file
  - STANDBY Mode/stm32f10x_it.c       Interrupt handlers
  - STANDBY Mode/stm32f10x_it.h       Header for stm32f10x_it.c
  - STANDBY Mode/main.c               Main program
    

@par Hardware and Software environment 

  
  - This example has been tested with STM32VLDISCOVERY board.

  - STM32VLDISCORY board Set-up  
    - Use LD3 and LD4 connected respectively to PC.09 and PC.08 pins
    - Use USER push button connected to PA.0 pin.
      
   
@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_gpio.c 
  - stm32f10x_exti.c 
  - stm32f10x_rcc.c
  - stm32f10x_pwr.c  
  - misc.c
  - STM32vldiscovery.c (under Utilities)
- Link all compiled files and load your image into target memory
- Run the example
   

   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */

