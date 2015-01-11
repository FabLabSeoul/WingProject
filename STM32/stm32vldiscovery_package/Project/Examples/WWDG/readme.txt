/**
  @page WWDG_Example WWDG
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    WWDG/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010  
  * @brief   Description of the WWDG Example.
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

This example shows how to update at regulate period the WWDG counter using the
Early Wakeup interrupt (EWI).

The WWDG timeout is set to 87.42ms. The refresh window is set to 65 and the EWI is enabled. 
When the WWDG counter reaches 64, the EWI is generated. In the WWDG ISR, the counter is 
refreshed to prevent a WWDG reset and LED4 is toggled.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the falling edge of the signal.
In the NVIC (nested vectored interrupt controller), the EXTI Line interrupt
vector is enabled with a priority equal to 0 and the WWDG interrupt vector is
enabled with a priority equal to 1 (EXTI IT > WWDG IT).

The EXTI Line is used to simulate a software failure: once the EXTI Line event 
occurs, by pressing the USER push-button, the corresponding interrupt is served.
In the ISR, the LED4 turns off and the EXTI Line pending bit is not cleared.
So the CPU executes the EXTI Line ISR indefinitely and the WWDG ISR is never
executed (the WWDG counter is not updated).
As a result, when the WWDG counter falls to 63, the WWDG reset occurs.
If the WWDG reset is generated, after the system resumes from reset, LED3 turns on.

If the EXTI Line event does not occur, the WWDG counter is indefinitely refreshed
in the WWDG ISR, and there is no WWDG reset.

In this example the system clock is set to 24 MHz.


@par Directory contents 

  - WWDG/stm32f10x_conf.h     Library Configuration file
  - WWDG/stm32f10x_it.c       Interrupt handlers
  - WWDG/stm32f10x_it.h       Header for stm32f10x_it.c
  - WWDG/main.c               Main program
  - WWDG/main.h               Header for main.c

@par Hardware and Software environment 

  
  - This example has been tested with STM32VLDISCOVERY board.
      
  - STM32VLDISCOVERY board
    - Use LD3 and LD4 connected respectively to PC.09 and PC.08 pins
    - Use USER push button connected to PA.0 pin.
      

          
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c
  - stm32f10x_exti.c 
  - stm32f10x_wwdg.c  
  - misc.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
  - STM32vldiscovery.c (under Utilities)
- Link all compiled files and load your image into target memory
- Run the example


   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
