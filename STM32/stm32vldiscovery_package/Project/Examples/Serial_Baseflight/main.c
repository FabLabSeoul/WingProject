
#include "global.h"


int main()
{
	// 시리얼 통신 초기화.
	serialInit(9600);
	
	// 루프
	while (1)
	{
		// 시리얼 통신 처리.
		serialCom();
	}
		
}

