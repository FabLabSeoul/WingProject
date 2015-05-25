
#include "stdafx.h"
#include "dbg.h"

using namespace common;
using namespace dbg;

//------------------------------------------------------------------------
// ���â�� ��Ʈ���� ����Ѵ�.
//------------------------------------------------------------------------
void dbg::Print( const std::string &str)
{
	OutputDebugStringA(str.c_str());
	OutputDebugStringA("\n");
}


//------------------------------------------------------------------------
// ���â�� ��Ʈ���� ����Ѵ�. (�������� ���)
//------------------------------------------------------------------------
void dbg::Print( const char* fmt, ...)
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );
	OutputDebugStringA(textString);
	OutputDebugStringA("\n");
}
