
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
	// Ŭ��, Ÿ�̸� �ʱ�ȭ
	systemInit();
	
	// �ø��� ��� �ʱ�ȭ.
	serialInit(9600);
	
	// ����
	while (1)
	{
		// �ø��� ��� ó��.
		serialCom();
		
		printf( "test\n" );
		
		delay(1000);
	}
		
}

