#pragma once

#include <time.h>    // time()


namespace common
{

	inline void initrand()
	{
		srand((unsigned)time(NULL) );
	}


	// -1 ~ +1 random float
	// 소수점 3 째 자리 까지 랜덤.
	inline float randfloat() 
	{
		return ((float)(rand() % 2000) / 1000.f) - 1.f;
	}


	// 0 ~ +1 random float
	// 소수점 3 째 자리 까지 랜덤.
	inline float randfloatpositive()
	{
		return ((float)(rand() % 1000) / 1000.f);
	}

}
