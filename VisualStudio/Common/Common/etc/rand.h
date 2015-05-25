#pragma once

#include <time.h>    // time()


namespace common
{

	inline void initrand()
	{
		srand((unsigned)time(NULL) );
	}


	// -1 ~ +1 random float
	// �Ҽ��� 3 ° �ڸ� ���� ����.
	inline float randfloat() 
	{
		return ((float)(rand() % 2000) / 1000.f) - 1.f;
	}


	// 0 ~ +1 random float
	// �Ҽ��� 3 ° �ڸ� ���� ����.
	inline float randfloatpositive()
	{
		return ((float)(rand() % 1000) / 1000.f);
	}

}
