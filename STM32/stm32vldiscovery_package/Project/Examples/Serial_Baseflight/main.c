//
// USART1_TX    PA9
 // USART1_RX    PA10
//

#include "global.h"


int main()
{
	// �ø��� ��� �ʱ�ȭ.
	serialInit(9600);
	
	// ����
	while (1)
	{
		// �ø��� ��� ó��.
		serialCom();
	}
		
}

