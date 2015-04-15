/*
 * This file is part of baseflight
 * Licensed under GPL V3 or modified DCL - see https://github.com/multiwii/baseflight/blob/master/README.md
 */

//#include "board.h"
//#include "mw.h"
#include "global.h"

#define INBUF_SIZE 128

typedef struct box_t {
    const uint8_t boxIndex;         // this is from boxnames enum
    const char *boxName;            // GUI-readable box name
    const uint8_t permanentId;      // Permanent ID for reporting to GUI
} box_t;

typedef enum serialState_t {
    IDLE,
    HEADER_START,
    HEADER_M,
    HEADER_ARROW,
    HEADER_SIZE,
    HEADER_CMD,
} serialState_t;

typedef  struct mspPortState_t {
    serialPort_t *port;
    uint8_t checksum;
    uint8_t indRX;
    uint8_t inBuf[INBUF_SIZE];
    uint8_t cmdMSP;
    uint8_t offset;
    uint8_t dataSize;
    serialState_t c_state;
} mspPortState_t;


static mspPortState_t ports[2];
static mspPortState_t *currentPortState = &ports[0];
static int numTelemetryPorts = 0;


serialPort_t *g_mainport;

// static uint8_t checksum, indRX, inBuf[INBUF_SIZE];
// static uint8_t cmdMSP;

void serialize8(uint8_t a)
{
    serialWrite(currentPortState->port, a);
    //currentPortState->checksum ^= a;
}

void serialize32(uint32_t a)
{
    serialize8(a & 0xFF);
    serialize8((a >> 8) & 0xFF);
    serialize8((a >> 16) & 0xFF);
    serialize8((a >> 24) & 0xFF);
}

void serialize16(int16_t a)
{
    serialize8(a & 0xFF);
    serialize8((a >> 8) & 0xFF);
}

uint8_t read8(void)
{
    return currentPortState->inBuf[currentPortState->indRX++] & 0xff;
}

uint16_t read16(void)
{
    uint16_t t = read8();
    t += (uint16_t)read8() << 8;
    return t;
}

uint32_t read32(void)
{
    uint32_t t = read16();
    t += (uint32_t)read16() << 16;
    return t;
}


void serialInit(uint32_t baudrate)
{
	  // DMA clock enable
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
  numTelemetryPorts = 0;
  g_mainport = uartOpen(USART1, NULL, baudrate, MODE_RXTX);
	ports[0].port = g_mainport;
  numTelemetryPorts++;
	
}


void serialCom(void)
{
    int i;
	static int first = 1;
	
	if (first)
	{
		// 프로그램이 시작하면 Start 문자를 시리얼통신으로 보낸다.
		first = 0;
		serialPrint(currentPortState->port, "Start\n" );
	}
	

    for (i = 0; i < numTelemetryPorts; i++) 
	{
		currentPortState = &ports[i];

		// 시리얼통신으로 부터 받은 정보를 저장한다.
        while (serialTotalBytesWaiting(currentPortState->port)) 
		{
			uint8_t c = serialRead(currentPortState->port);

			currentPortState->inBuf[currentPortState->offset++] = c;
					
			if (currentPortState->offset +1 > INBUF_SIZE)
			{
				currentPortState->offset = 0;
			}
		}
				
		// 열 개의 문자를 받으면, 그 정보를 그대로 다시 전달한다.
		if (currentPortState->offset >= 10)
		{
			currentPortState->inBuf[ currentPortState->offset] = 0;
			serialPrint(currentPortState->port, (const char*)currentPortState->inBuf);
			
			currentPortState->offset = 0;
		}				
    }		
}
