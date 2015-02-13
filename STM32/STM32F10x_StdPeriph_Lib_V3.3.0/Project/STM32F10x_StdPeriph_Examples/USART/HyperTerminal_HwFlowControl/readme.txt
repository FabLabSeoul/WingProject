/**
  @page USART_HyperTerminal_HwFlowControl USART_HyperTerminal_HwFlowControl
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    USART/HyperTerminal_HwFlowControl/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the USART Hyperterminal Hardware Flow Control
  *          Example.
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

This example provides a description of how to use the USART with hardware flow
control and communicate with the Hyperterminal.
First, the USART2 sends the TxBuffer to the hyperterminal and still waiting for
a string from the hyperterminal that you must enter which must end by '\r'
character (keypad ENTER button). The communication will end if received data exceed
255 without sending the '\r' character. Each byte received is retransmitted to the
Hyperterminal.
The string that you have entered is stored in the RxBuffer array. The receive
buffer have a RxBufferSize bytes as maximum.

The USART2 is configured as follow:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control enabled (RTS and CTS signals)
    - Receive and transmit enabled

@par Directory contents 

  - USART/HyperTerminal_HwFlowControl/platform_config.h    Evaluation board specific configuration file
  - USART/HyperTerminal_HwFlowControl/stm32f10x_conf.h     Library Configuration file
  - USART/HyperTerminal_HwFlowControl/stm32f10x_it.h       Interrupt handlers header file
  - USART/HyperTerminal_HwFlowControl/stm32f10x_it.c       Interrupt handlers
  - USART/HyperTerminal_HwFlowControl/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL 
    (Medium-Density Value line), STM3210E-EVAL (High-Density and XL-Density)
    and STM3210B-EVAL (Medium-Density) evaluation boards and can be 
    easily tailored to any other supported device and development board.
    This example can't be tested with STM3210C-EVAL (Connectivity-Line) evaluation board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in USART/HyperTerminal_HwFlowControl/platform_config.h file

  - STM32100B-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between CN9 and PC serial 
      port.
      @note In this case USART2 Tx, Rx, RTS and CTS pins are remapped by software
            on PD.05, PD.06, PD.04 and PD.03 respectively. 

  - STM3210E-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between CN8 and PC serial 
      port.
      @note Make sure the Jumper 4 (JP4) is in position 2<-->3.  

  - STM3210B-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between CN5 and PC serial 
      port.
      @note In this case USART2 Tx, Rx, RTS and CTS pins are remapped by software
            on PD.05, PD.06, PD.04 and PD.03 respectively. 
 
  - Hyperterminal configuration: 
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: Hardware 
    
@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_gpio.c
  - stm32f10x_rcc.c  
  - stm32f10x_usart.c  
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
        
- Edit stm32f10x.h file to select the device you are working on.
- Edit USART/HyperTerminal_HwFlowControl/platform_config.h file to select the
  evaluation board you will use.
  
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
