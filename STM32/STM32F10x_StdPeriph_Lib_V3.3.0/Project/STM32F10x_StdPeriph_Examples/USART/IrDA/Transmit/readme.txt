/**
  @page USART_IrDA_Transmit USART_IrDA_Transmit
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    USART/IrDA/Transmit/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the USART IrDA Transmit Example.
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

This example provides a basic communication USARTy IrDA transmit mode. Five pins,
configured in input floating mode, are used to select the byte to be send at 
each pin state change. USARTy can be USART3 or USART2 depending on the
STMicroelectronics EVAL board you are using.

These bytes are:
  - 0x00 if no key (JOY_NONE) pressed
  - 0x01 if JOY_SEL pin state change
  - 0x02 if JOY_DOWN pin state change 
  - 0x03 if JOY_LEFT pin state change 
  - 0x04 if JOY_RIGHT pin state change 
  - 0x05 if JOY_UP pin state change 

USARTy configured as follow:
   - BaudRate = 115200 baud  
   - Word Length = 8 Bits
   - One Stop Bit
   - No parity
   - Hardware flow control disabled (RTS and CTS signals)
   - Receive and transmit enabled

The USART IrDA example provides two IrDA program: transmitter&receiver and requires two boards
to be able to run the full demonstration:
 - one board will act as IrDA transmitter
 - one board will act as IrDA receiver


@par Directory contents 

  - USART/IrDA/Transmit/platform_config.h    Evaluation board specific configuration file
  - USART/IrDA/Transmit/stm32f10x_conf.h     Library Configuration file
  - USART/IrDA/Transmit/stm32f10x_it.h       Interrupt handlers header file
  - USART/IrDA/Transmit/stm32f10x_it.c       Interrupt handlers
  - USART/IrDA/Transmit/main.c               Main program

  
@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in USART/IrDA/Transmit/platform_config.h or the stm32_eval.h file

  - STM32100B-EVAL Set-up  
    - Use DOWN push-button connected to pin PD.14
    - Use UP push-button connected to pin PD.08
    - Use SEL push-button connected to pin PD.12
    - Use RIGHT push-button connected to pin PE.01
    - Use RIGHT push-button connected to pin PE.00                   
    - Use an IrDA tranceiver connected to the USART3 Tx and Rx pins (U14 
      connector, JP11 jumper must be fitted).

  - STM3210C-EVAL Set-up 
    - The JoyStick push buttons are connected to the IO Expander on I2C.  
    - Use an IrDA tranceiver connected to the USART2 Tx and Rx pins (U12 
      connector, JP16 should be in position 1<-->2).
          
  - STM3210E-EVAL Set-up 
    - Use DOWN push-button connected to pin PD.03
    - Use UP push-button connected to pin PG.15
    - Use SEL push-button connected to pin PG.07
    - Use RIGHT push-button connected to pin PG.13
    - Use RIGHT push-button connected to pin PG.14   
    - Use an IrDA tranceiver connected to the USART3 Tx and Rx pinsn (U13 
      connector, JP21 and JP22 jumper must be fitted).

  - STM3210B-EVAL Set-up  
    - Use DOWN push-button connected to pin PD.14
    - Use UP push-button connected to pin PD.08
    - Use SEL push-button connected to pin PD.12
    - Use RIGHT push-button connected to pin PE.00
    - Use RIGHT push-button connected to pin PE.01                   
    - Use an IrDA tranceiver connected to the USART3 Tx and Rx pins (U11 
      connector, JP5 jumper must be fitted).
   
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_exti.c  
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - stm32f10x_usart.c 
  - stm32f10x_i2c.c
  - stm32f10x_spi.c
  - stm32_eval.c         (under Utilities\STM32_EVAL)
  - system_stm32f10x.c   (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)     
If you are using STM3210C-EVAL 
    - stm3210c_eval_ioe.c (under Utilities\STM32_EVAL\STM3210C_EVAL)

- Edit stm32f10x.h file to select the device you are working on.
- Edit stm32_eval.h file to select the evaluation board you will use.
- or edit USART/IrDA/Transmit/platform_config.h file to select the evaluation board you will use.
  
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
