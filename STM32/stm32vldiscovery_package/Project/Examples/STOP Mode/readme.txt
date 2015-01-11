/**
  @page STOP_Mode STOP Mode
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    STOP Mode/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Description of the STOP Mode Example.
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

This example shows how to enter the system to STOP mode and wake-up using EXTI
Line interrupts. The EXTI Line sources are PA.0 and RTC Alarm.

The EXTI line0 is configured to generate interrupt on falling edge.
The EXTI line17(RTC Alarm) is configured to generate interrupt on rising edge and
the RTC time base is set to 1 second using the external low speed oscillator(LSE).

The system clock is set to 24 MHz using the external high speed oscillator(HSE).

The system enters and exits STOP mode as following:
After 2 seconds from system start-up, the RTC is configured to generate an Alarm
event in 3 seconds then the system enters STOP mode. To wake-up from STOP mode you
have to apply a falling edge on EXTI line0, otherwise the  RTC Alarm will wake-up
the system within 3 seconds. After exit from STOP, the system clock is reconfigured
to its previous state (as HSE and PLL are disabled in STOP mode).
Then, after a delay the system will enter again in STOP mode and exit in the way
described above. This behavior is repeated in an infinite loop.

Three leds LD3 and LD4 are used to monitor the system state as following:
 - LD3 on: system in RUN mode
 - LD3 off: system in STOP mode
 - LD4 is on if EXTI Line0 is used to exit from STOP
 - LD4 is off if EXTI line17(RTC Alarm) is used to exit from STOP 
 
@par Directory contents 

  - STOP Mode/stm32f10x_conf.h     Library Configuration file
  - STOP Mode/stm32f10x_it.c       Interrupt handlers
  - STOP Mode/stm32f10x_it.h       Header for stm32f10x_it.c
  - STOP Mode/main.c               Main program

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
