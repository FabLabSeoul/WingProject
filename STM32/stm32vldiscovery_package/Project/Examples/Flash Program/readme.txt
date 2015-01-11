/**
  @page FLASH_Program FLASH Program
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    FLASH Program/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Description of the FLASH Program Example.
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

This example provides a description of how to program the STM32F10x FLASH.

After Reset, the Flash memory Program/Erase Controller is locked. To unlock it,
the FLASH_Unlock function is used.
Before programming the desired addresses, an erase operation is performed using 
the flash erase page feature. The erase procedure starts with the calculation of
the number of pages to be used. Then all these pages will be erased one by one by
calling FLASH_ErasePage function. 

Once this operation is finished, the programming operation will be performed by 
using the FLASH_ProgramWord function. The written data is then checked and the
result of the programming operation is stored into the MemoryProgramStatus variable.

@par Directory contents 

  - FLASH Program/stm32f10x_conf.h   Library Configuration file
  - FLASH Program/stm32f10x_it.h     Interrupt handlers header file
  - FLASH Program/stm32f10x_it.c     Interrupt handlers
  - FLASH Program/main.c             Main program

@par Hardware and Software environment 
  
  - This example has been tested with STM32VLDISCOVERY board.
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_flash.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x) 
  - Link all compiled files and load your image into target memory
- Run the example
   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
