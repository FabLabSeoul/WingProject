/**
  @page GPIO_IOToggle GPIO_IOToggle
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    GPIO/IOToggle/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the GPIO IOToggle Example.
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

  - GPIO/IOToggle/stm32f10x_conf.h     Library Configuration file
  - GPIO/IOToggle/stm32f10x_it.c       Interrupt handlers
  - GPIO/IOToggle/stm32f10x_it.h       Header for stm32f10x_it.c
  - GPIO/IOToggle/main.c               Main program

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
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
      
  - STM3210C-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins

  - STM3210E-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PF.06, PF0.7, PF.08
      and PF.09 pins

  - STM3210B-EVAL Set-up  
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins


@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c 
  - stm32f10x_exti.c
  - stm32f10x_rcc.c
  - stm32f10x_usart.c     
  - misc.c
  - stm32f10x_i2c.c
  - stm32f10x_spi.c
  - stm32_eval.c       (under Utilities\STM32_EVAL)
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)   

- Edit stm32f10x.h file to select the device you are working on.
- Edit stm32_eval.h file to select the evaluation board you will use.
  
@b Tip: You can tailor the provided project template to run this example, for 
        more details please refer to "stm32f10x_stdperiph_lib_um.chm" user 
        manual; select "Peripheral Examples" then follow the instructions 
        provided in "How to proceed" section.   
- Link all compiled files and load your image into target memory
- Run the example

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
