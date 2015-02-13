/**
  @page PWR_STANDBY PWR_STANDBY
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    PWR/STANDBY/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the PWR STANDBY Example.
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

This example shows how to enters the system to STANDBY mode and wake-up from this
mode using: external RESET, RTC Alarm or WKUP pin.

In the associated software, the system clock is set to 72 MHz, an EXTI line
is configured to generate an interrupt on falling edge and the SysTick is programmed
to generate an interrupt each 250 ms. In the SysTick interrupt handler, the LED1 is
toggled, this is used to indicate whether the MCU is in STANDBY or RUN mode.

When a falling edge is detected on the EXTI line an interrupt is generated. In the 
EXTI handler routine the RTC is configured to generate an Alarm event in 3 second
then the system enters STANDBY mode causing the LED1 to stop toggling. 
A rising edge on WKUP pin or an external RESET will wake-up the system from
STANDBY. If within 3 second neither rising edge on WKUP pin nor external RESET
are generated, the RTC Alarm will wake-up the system. 

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET, the LED1 restarts toggling, LED2 is turned on and the RTC configuration
(clock source, enable, prescaler,...) is kept.
As result there is no need to configure the RTC.

Two leds LED1 and LED2 are used to monitor the system state as following:
 - LED1 toggling: system in RUN mode
 - LED1 off / LED2 off: system in STANDBY mode
 - LED2 on: system resumed from STANDBY mode

@par Directory contents 

  - PWR/STANDBY/stm32f10x_conf.h     Library Configuration file
  - PWR/STANDBY/stm32f10x_it.c       Interrupt handlers
  - PWR/STANDBY/stm32f10x_it.h       Header for stm32f10x_it.c
  - PWR/STANDBY/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100B-EVAL Set-up 
    - Use LED1 and LED2 connected respectively to PC.06 and PC.07 pins    
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use the Wakeup push-button connected to WKUP(PA.00) pin  
    
  - STM3210C-EVAL Set-up 
    - Use LED1 and LED2 connected respectively to PD.07 and PD.13 pins
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use the Wakeup push-button connected to WKUP(PA.00) pin, make sure
      that jumper JP14 is in position 2-3.

  - STM3210E-EVAL Set-up 
    - Use LED1 and LED2 connected respectively to PF.06 and PF.07 pins
    - Use the Key push-button connected to pin PG.08 (EXTI Line8).
    - Use the Wakeup push-button connected to WKUP(PA.00) pin
    @note  the jumper JP4 must be not fit to be able to use the Wakeup push-button

  - STM3210B-EVAL Set-up 
    - Use LED1 and LED2 connected respectively to PC.06 and PC.07 pins    
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use the Wakeup push-button connected to WKUP(PA.00) pin        

@note For power consumption measurement in STANDBY mode, you have to replace
      jumper JP9 in the STM3210B-EVAL board, JP12 in the STM3210E-EVAL or 
      JP23 (position 1-2) in the STM3210C-EVAL board by an ampermeter.

         
@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_rcc.c 
  - stm32f10x_gpio.c 
  - stm32f10x_rtc.c   
  - stm32f10x_pwr.c   
  - stm32f10x_bkp.c 
  - stm32f10x_exti.c 
  - misc.c 
  - stm32f10x_usart.c
  - stm32f10x_i2c.c
  - stm32f10x_spi.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
  - stm32_eval.c (under Utilities\STM32_EVAL)
     
- Edit stm32f10x.h file to select the device you are working on.
- Edit stm32_eval.h file to select the evaluation board you will use.
  
@b Tip: You can tailor the provided project template to run this example, for 
        more details please refer to "stm32f10x_stdperiph_lib_um.chm" user 
        manual; select "Peripheral Examples" then follow the instructions 
        provided in "How to proceed" section.   
- Link all compiled files and load your image into target memory
- Run the example in standalone mode (without debugger connection)

@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
