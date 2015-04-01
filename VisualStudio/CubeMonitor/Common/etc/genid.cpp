
#include "stdafx.h"
#include "genid.h"


namespace common
{
	int g_seedId = 10000;
}

using namespace common;


// 아이디 처음값을 설정한다.
void common::SetSeedId(const int seed)
{
	g_seedId = seed;
}


// 중복되지 않는 아이디 값을 생성해 리턴한다.
// g_seedId 값이 최대치를 넘어가면 문제가 발생할 수 있다.
// 이에 대한 처리는 아직 없다.
int common::GenerateId()
{
	return g_seedId++;
}
