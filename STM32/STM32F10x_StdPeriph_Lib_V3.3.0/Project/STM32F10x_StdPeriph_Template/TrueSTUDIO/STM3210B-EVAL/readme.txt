/**
  @page TrueSTUDIO Project Template for Medium-density devices
 
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   This sub-directory contains all the user-modifiable files 
  *          needed to create a new project linked with the STM32F10x  
  *          Standard Peripheral Library and working with TrueSTUDIO software 
  *          toolchain (Version 1.1.0 and later)
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
  * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
  * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
  * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
 
 @par Directory contents
 
 - project .cproject/.project: A pre-configured project file with the provided 
                               library structure that produces an executable 
                               image with TrueSTUDIO.

 - stm32_flash.ld: 			       This file is the TrueSTUDIO linker script used to 
                               place program code (readonly) in internal FLASH 
                               and data (readwrite, Stack and Heap)in internal 
                               SRAM. 
                               You can customize this file to your need.
                           
 @par How to use it ?
 
 - Open the TrueSTUDIO toolchain.
 - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
 - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
 - Browse to the TrueSTUDIO workspace directory, select the STM3210B-EVAL project and click "OK".
 - Rebuild all project files: Select the project in the "Project explorer" window then click on Project->build project menu.
 - Load project image: 
 	- Run ST-Link gdbserver via the Microsoft Windows Start menu. 
 	- Click on Run->Debug configuration menu. 
 	- Expand the GDB Hardware Debugging tree node, select the STM3210B-EVAL Debug configuration then click on "Debug" button.

@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 32 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 32 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.

 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
