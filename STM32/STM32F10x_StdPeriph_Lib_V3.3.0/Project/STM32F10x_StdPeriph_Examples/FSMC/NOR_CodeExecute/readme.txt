/**
  @page FSMC_NOR_CodeExecute FSMC_NOR_CodeExecute
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    FSMC/NOR_CodeExecute/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the FSMC NOR_CodeExecute Example.
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

This directory contains a set of sources files that describes how to build an
application to be loaded into the NOR memory mounted on STM3210E-EVAL board then
execute it from internal Flash.

@par Directory contents 

 - FSMC/NOR_CodeExecute/binary: Contains a set of sources files that build the 
                                application to be loaded into the NOR memory 
                                mounted on STM3210E-EVAL board.
 - FSMC/NOR_CodeExecute/stm32f10x_conf.h  Library Configuration file 
 - FSMC/NOR_CodeExecute/stm32f10x_it.c    Interrupt handlers 
 - FSMC/NOR_CodeExecute/stm32f10x_it.h    Header for stm32f10x_it.c 
 - FSMC/NOR_CodeExecute/main.c            Main program 
 - stm3210e_eval_fsmc_nor.c               Driver for NOR memory
 - stm3210e_eval_fsmc_nor.h               Header for stm3210e_eval_fsmc_nor.c  
  
@par Hardware and Software environment 

  - This example runs only on STM32F10x High-Density and XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (High-Density
    and XL-Density) evaluation board and can be easily tailored to any other 
    supported device and development board.
    
@par How to use it ? 

In order to make the program work, you must do the following:

1. Program the NOR memory with the example provided in the "binary" directory

2. Program the internal Flash with the code that will jump to the NOR memory to execute 
    the loaded example, for this you have to:
      - Create a project and setup all project configuration
      - Add the required Library files:
          - stm32f10x_fsmc.c 
          - stm32f10x_gpio.c
          - stm32f10x_rcc.c
          - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
          - core_cm3.c (under Libraries\CMSIS\CM3\CoreSupport)
          - stm3210e_eval_fsmc_nor.c (under Utilities\STM32_EVAL\STM3210E_EVAL)
      - Edit stm32f10x.h file to select the device you are working on (#define 
        STM32F10X_HD, in this case).
  
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
