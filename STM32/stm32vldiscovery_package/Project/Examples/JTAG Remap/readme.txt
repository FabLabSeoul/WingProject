/**
  @page GPIO_JTAG_Remap GPIO_JTAG_Remap
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    JTAG Remap/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010  
  * @brief   Description of the GPIO JTAG Remap Example.
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

This example provides a short description of how to use the JTAG IOs as standard 
GPIOs and gives a configuration sequence. 

First, the SWJ-DP is disabled. The SWJ-DP pins are configured as output push-pull.
Five LEDs connected to the PA.13(JTMS/SWDAT), PA.14(JTCK/SWCLK), PA.15(JTDI), 
PB.03(JTDO) and PB.04(JTRST) pins are toggled in an infinite loop.

Note that once the JTAG IOs are disabled, the connection with the host debugger is
lost and cannot be re-established as long as the JTAG IOs remain disabled. 

To avoid this situation, a specified pin is connected to a push-button that is used
to disable or not the JTAG IOs:
 1. push-button pressed at reset: JTAG IOs disabled	and LD3 turned on
 2. push-button not pressed at reset: JTAG IOs unchanged and LD4 turned on

Before starting this example, you should disconnect your tool chain debugging probe
and run the example in standalone mode.

@par Directory contents 

  - JTAG Remap/stm32f10x_conf.h     Library Configuration file
  - JTAG Remap/stm32f10x_it.c       Interrupt handlers
  - JTAG Remap/stm32f10x_it.h       Header for stm32f10x_it.c
  - JTAG Remap/main.c               Main program

@par Hardware and Software environment 
   - This example has been tested with STM32VLDISCOVERY board.
   
   - STM32F100 Discovery board Set-up  
    - User LD3 and LD4 connected respectively to pins PC.09 and PC.08. 
    - Use the USER push-button connected to pin PA.00 (EXTI Line0).

@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_exti.c   
  - stm32f10x_gpio.c
  - stm32f10x_rcc.c
  - misc.c
  - STM32vldiscovery.c       (under Utilities)
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x) 
- Link all compiled files and load your image into target memory
- Run the example


 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
