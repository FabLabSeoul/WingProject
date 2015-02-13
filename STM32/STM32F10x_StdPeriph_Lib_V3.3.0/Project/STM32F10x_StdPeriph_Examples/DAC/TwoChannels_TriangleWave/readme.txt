/**
  @page DAC_TwoChannels_TriangleWave DAC_TwoChannels_TriangleWave
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    DAC/TwoChannels_TriangleWave/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the DAC two channels triangle wave example.
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

This example describes how to use two DAC channels to generate two different
signals with triangle waves on each DAC Channel output.

DAC channels conversion are configured to be triggered by TIM2 TRGO with triangle
wave generation. 12bit right data alignement is selected since we choose to acces
both DAC_DHR12R1 and DAC_DHR12R2 registers.
A triangle amplitude of 2047 is selected for DAC channel1 and 1023 for DAC channel2.
Both DAC channels are then enabled. DAC Channel1 DHR12R1 and DAC channel2 DHR12R2
registers are set to have a base output voltage of VREF/16 on each output.

Once TIM2 is enabled, each TIM2 TRGO update event trigger both DAC channels start
of conversion. The triangle counter is incremented, added to the base value and
applied to the corresponding DAC channel output. The same calculation is repeated
on each trigger.
The triangle waves can be visualized by connecting both PA.04 and PA.05 pins to
an oscilloscope.


@par Directory contents 

  - DAC/TwoChannels_TriangleWave/stm32f10x_conf.h     Library Configuration file
  - DAC/TwoChannels_TriangleWave/stm32f10x_it.c       Interrupt handlers
  - DAC/TwoChannels_TriangleWave/stm32f10x_it.h       Header for stm32f10x_it.c
  - DAC/TwoChannels_TriangleWave/main.c               Main program


@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, XL-Density,
    Medium-Density Value line and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL 
    (Medium-Density Value line), STM3210C-EVAL (Connectivity line) and 
    STM3210E-EVAL (High-Density and XL-Density) evaluation boards and can be 
    easily tailored to any other supported device and development board.  

  - STM32100B-EVAL Set-up  
    - Connect PA.04 and PA.05 pins to an oscilloscope 
    @note JP2 should be open
    
  - STM3210C-EVAL Set-up  
    - Only PA.04 can be monitored on an oscilloscope, PA.05 is used by other
      module (Motor control connector) that prevents to have DAC channel output
      on it. However, if you don't use Motor control connector, you can remove
      the 0ohm resistor R84 and thus PA.05 can be used for DAC output.
    @note Make shure that jumper JP15 is open.
    
  - STM3210E-EVAL Set-up 
    - Connect PA.04 and PA.05 pins to an oscilloscope  
    
    
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_dac.c
  - stm32f10x_gpio.c   
  - stm32f10x_rcc.c 
  - stm32f10x_tim.c
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
