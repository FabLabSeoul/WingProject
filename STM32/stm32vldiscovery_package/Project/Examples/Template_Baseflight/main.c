
#include "global.h"


#ifdef USE_LAME_PRINTF
// gcc/GNU version
static void _putc(void *p, char c)
{
    (void)p;
    serialWrite(g_mainport, c);
}
#else
// keil/armcc version
int fputc(int c, FILE *f)
{
    // let DMA catch up a bit when using set or dump, we're too fast.
   while (!isSerialTransmitBufferEmpty(g_mainport));
   serialWrite(g_mainport, c);
   return c;
}
#endif



int main()
{
	// 클럭, 타이머 초기화
	systemInit();
	
	// 시리얼 통신 초기화.
	serialInit(9600);
	
	// 루프
	while (1)
	{
		// 시리얼 통신 처리.
		serialCom();
		
		printf( "test\n" );
		
		delay(1000);
	}
		
}

