/**
  @page TIM15_ComplementarySignals TIM15_ComplementarySignals
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    TIM/TIM15_ComplementarySignals/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the TIM15 ComplementarySignals example.
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

This example shows how to configure the TIM15 peripheral to generate one 
complementary TIM15 signal, to insert a defined dead time value, to use the break 
feature and to lock the desired parameters.

TIM15CLK is fixed to 24 MHz, the TIM15 Prescaler is equal to 0 so the TIM15 counter
clock used is 24 MHz.

TIM15 frequency is defined as follow:
TIM15 frequency = TIM15 counter clock / (TIM15_Period + 1) = 366 Hz.

The Duty cycles is computed as the following description: 

- TIM15 Channel1 duty cycle = TIM15_CCR1 / (TIM15_Period + 1) = 50% 
- TIM15 Channel1N duty cycle = (TIM15_Period - TIM15_CCR1) / (TIM15_Period + 1) = 50%

A dead time of 1.62 us is inserted between the different complementary signals, 
and the Lock level 1 is selected.
The break Polarity is used at High level.

The TIM15 waveform can be displayed using an oscilloscope.

@par Directory contents 

  - TIM/TIM15_ComplementarySignals/stm32f10x_conf.h  Library Configuration file
  - TIM/TIM15_ComplementarySignals/stm32f10x_it.c    Interrupt handlers
  - TIM/TIM15_ComplementarySignals/stm32f10x_it.h    Interrupt handlers header file
  - TIM/TIM15_ComplementarySignals/main.c            Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Medium-Density Value line and Low-Density 
    Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL 
    (STM32F10x Medium-Density Value line) evaluation boards and can be easily 
    tailored to any other supported device and development board.
    
  - STM32100B-EVAL Set-up 
    - Connect the TIM15 pins to an oscilloscope to monitor the different waveforms:
      - TIM15_CH1  pin (PA.02)  
      - TIM15_CH1N pin (PB.15)  

    - Connect the TIM15 break pin TIM15_BKIN pin (PA.09) to the GND. To generate 
      a break event, switch this pin level from 0V to 3.3V.  
  
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c 
  - stm32f10x_tim.c 
  - stm32f10x_rcc.c
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
