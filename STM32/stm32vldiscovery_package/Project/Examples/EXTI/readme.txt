/**
  @page EXTI_Example EXTI
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    EXTI/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Description of the EXTI Example.
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

This example shows how to configure an external interrupt line.
In this example, an EXTI line is configured to generate an interrupt on each
falling edge. In the interrupt routine a led connected to a specific GPIO pin is
toggled.
This led will be toggled at each rising edge.

@par Directory contents 

  - EXTI/stm32f10x_conf.h   Library Configuration file
  - EXTI/stm32f10x_it.h     Interrupt handlers header file
  - EXTI/stm32f10x_it.c     Interrupt handlers
  - EXTI/main.c             Main program

@par Hardware and Software environment 

  
  - This example has been tested with STM32VLDISCOVERY board.

  - STM32VLDISCOVERY board
    - Use LD3 led connected to PC.09 pin
    - Use the User button connected to pin PA.0 (EXTI Line0). 
    

@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - stm32f10x_exti.c 
  - misc.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
- Link all compiled files and load your image into target memory
- Run the example


   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
