/**
    @page Demo Demo
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    DMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Description for STM32VLDISCOVERY board demonstration software.
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

This software demonstration aims to use various peripherals and connectors on
STM32VLDISCOVERY  board:

- 8M Hz HSE crystal (X2)
- 32KHz LSE crystal (X3)
- Push buttons: B1( USER), B2 (RST)
- 2 LEDs: LD3,LD4
- Embedded ST-Link (via CN1) & LD1

In this demo, a check on the LSE status is performed:

* In the case of failure, the led LD4 is turned ON. 
* In the case of success, the led LD3 toggles and its blinking changes according 
to the USER push button clicks. 
Each click on the USER push button is confirmed by the LD4.

@par Directory contents 
  - Demo/inc/stm32f10x_conf.h  Library Configuration file
  - Demo/inc/stm32f10x_it.h    Interrupt handlers header file
  - Demo/src/stm32f10x_it.c    Interrupt handlers
  - Demo/src/main.c            Main program

@par Hardware and Software environment

   - This Demo has been tested with STM32VLDISCOVERY board.

@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_pwr.c 
  - stm32f10x_rcc.c 
  - stm32f10x_exti.c 
  - stm32f10x_gpio.c 
  - misc.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)   
- Link all compiled files and load your image into target memory
- Run the example

   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
