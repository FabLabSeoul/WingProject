/**
  @page TIM_OnePulse TIM_OnePulse
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    TIM/OnePulse/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the TIM OnePulse example.
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

This example shows how to use the TIM peripheral to generate a One pulse Mode 
after a Rising edge of an external signal is received in Timer Input pin.

TIM2CLK = SystemCoreClock, we want to get TIM2 counter clock at 24 MHz:
  - Prescaler = (TIM2CLK / TIM2 counter clock) - 1
SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
and Connectivity line devices and to 24 MHz for Low-Density Value line and
Medium-Density Value line devices

The Autoreload value is 65535 (TIM4->ARR), so the maximum frequency value to 
trigger the TIM4 input is 24000000/65535 = 300 Hz.

The TIM4 is configured as follows: 
The One Pulse mode is used, the external signal is connected to TIM4 CH2 pin (PB.07), 
the rising edge is used as active edge, the One Pulse signal is output 
on TIM4_CH1 (PB.06).

The TIM_Pulse defines the delay value, the delay value is fixed to:
delay =  CCR1/TIM4 counter clock = 16383 / 24000000 = 682.6 us. 
The (TIM_Period - TIM_Pulse) defines the One Pulse value, the pulse value is fixed to:
One Pulse value = (TIM_Period - TIM_Pulse)/TIM4 counter clock 
                = (65535 - 16383) / 24000000 = 2.048 ms.

@par Directory contents 

  - TIM/OnePulse/stm32f10x_conf.h  Library Configuration file
  - TIM/OnePulse/stm32f10x_it.c    Interrupt handlers
  - TIM/OnePulse/stm32f10x_it.h    Interrupt handlers header file
  - TIM/OnePulse/main.c            Main program 

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.

  - STM32100B-EVAL, STM3210E-EVAL, STM3210B-EVAL and STM3210C-EVAL  Set-up 
   - Connect the external signal to the TIM4_CH2 pin (PB.07)
   - Connect the TIM4_CH1 (PB.06) pin to an oscilloscope to monitor the waveform.   
  
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - stm32f10x_tim.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x) 

- Edit stm32f10x.h file to select the device you are working on.
  
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
