/**
  @page STANDBY STANDBY Mode
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    STANDBY Mode/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Description of the STANDBY Mode Example.
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

This example shows how to enter the system to STANDBY mode and wake-up from this
mode using either external RESET or RTC Alarm.

In the associated software, the system clock is set to 24 MHz, the EXTI line 0
is configured to generate an interrupt on falling edge and the SysTick is programmed
to generate an interrupt each 250 ms. In the SysTick interrupt handler, the LED3 is
toggled, this is used to indicate whether the MCU is in STANDBY or RUN mode.

When a falling edge is detected on the EXTI line, an interrupt is generated. In the 
EXTI handler routine, the RTC is configured to generate an Alarm event in 3 seconds
then the system enters STANDBY mode causing the LED3 to stop toggling. 
An external RESET will wake-up the system from STANDBY. If within 3 seconds an external RESET
is not generated, the RTC Alarm will wake-up the system. 

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET, the LED3 restarts toggling, LED4 is turned on and the RTC configuration
(clock source, enable, prescaler,...) is kept.
As result there is no need to configure the RTC.

Two leds LED3 and LED4 are used to monitor the system state as following:
 - LED3 toggling: system in RUN mode
 - LED3 off / LED4 off: system in STANDBY mode
 - LED4 on: system resumed from STANDBY mode

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
  - stm32f10x_rtc.c
  - stm32f10x_bkp.c      
  - misc.c
  - STM32vldiscovery.c (under Utilities)
- Link all compiled files and load your image into target memory
- Run the example

   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
