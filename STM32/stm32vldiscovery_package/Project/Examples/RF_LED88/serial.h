

#include "stm32f10x.h"



void SerialSetup(uint16_t baudRate);
void DMA_SerialSetup(uint16_t baudRate);
	
void print_byte(short  c);

void print_short(short  c);

uint16_t receiveData(void);
char receiveData2(void);


void sendData(void);

void uartStartTxDMA(DMA_Channel_TypeDef *txDMAChannel);

bool isReceiveData(void);
void uartStartRxDMA(void);



#include <stm32f10x.h>
void __attribute__((weak)) osa_mdelay(unsigned int msec);

void putc ( void* p, char c);


static const uint16_t TxBufferSize1 = 14;
extern uint8_t TxBuffer1[TxBufferSize1];
extern uint8_t RxBuffer1[TxBufferSize1];


