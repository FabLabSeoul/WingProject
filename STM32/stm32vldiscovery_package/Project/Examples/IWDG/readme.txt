/**
  @page IWDG_Example IWDG
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    IWDG/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Description of the IWDG Example.
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

This example shows how to reload at regulate period the IWDG counter using the
SysTick interrupt. The IWDG timeout is set to 280 ms (the timeout may vary due
to LSI frequency dispersion).

SysTick is configured to generate an interrupt every 250 ms. In the SysTick interrupt
service routine (ISR), the IWDG counter is reloaded to prevent an IWDG reset, and
LD4 is toggled.
An EXTI is connected to a specific GPIO pin and configured to generate an interrupt
on its falling edge.
In the NVIC, this EXTI line corresspondant interrupt vector is enabled with a 
priority equal to 0, and the SysTick interrupt vector priority is set to 1 
(EXTI IT > SysTick IT).

The EXTI Line is used to simulate a software failure: when the EXTI Line event is
triggered (by pressing the USER push-button on the board), the corresponding interrupt
is served. In the ISR, the LD3 turns off and the EXTI line pending bit is 
not cleared. So the CPU executes the EXTI line ISR indefinitely and the SysTick 
ISR is never entered (IWDG counter not reloaded).
As a result, when the IWDG counter reaches 00h, the IWDG generates a reset.
If the IWDG reset is generated, LD3 is turned on after the system resumes operation.

If the EXTI Line event does not occur, the IWDG counter is indefinitely reloaded in
the SysTick ISR, which prevents any IWDG reset.

In this example the system clock is set to 24 MHz.

@par Directory contents 

  - IWDG/stm32f10x_conf.h     Library Configuration file
  - IWDG/stm32f10x_it.c       Interrupt handlers
  - IWDG/stm32f10x_it.h       Header for stm32f10x_it.c
  - IWDG/main.c               Main program

@par Hardware and Software environment 

  - This example has been tested with STMicroelectronics STM32VLDISCOVERYoard

  - STM32VLDISCOVERY board Set-up  
    - Use LD3 and LD4 leds connected respectively to PC.09 and PC.08 pins
    - Use the USER push button connected to PA.0 pin (EXTI Line0).
    
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - misc.c
  - stm32f10x_iwdg.c 
  - stm32f10x_exti.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
  - STM32vldiscovery.c (under Utilities)

- Link all compiled files and load your image into target memory
- Run the example in standalone mode (without debugger connection)

   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
