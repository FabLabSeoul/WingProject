
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

