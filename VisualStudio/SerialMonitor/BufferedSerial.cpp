
#include "stdafx.h"
#include "BufferedSerial.h"
#include <string>
using std::string;


CBufferedSerial::CBufferedSerial()
{
}

CBufferedSerial::~CBufferedSerial()
{
}


// ch문자가 있을 때까지 시리얼정보를 읽어서 리턴한다.
// ch문자가 없다면 빈문자열을 리턴한다.
bool CBufferedSerial::ReadStringUntil(const char ch, OUT string &out)
{
	const int readBytes = ReadData(&m_ringBuffer[m_tailIndex], MAX_BUFFERSIZE - m_tailIndex);
	if (readBytes < 0) // error !!
		return false;
	if (0 == readBytes) // nothing to read
		return true;

	// 읽은 위치부터 ch 문자가 있는지 검사한다.
	int findChIndex = -1;
	for (int i = m_tailIndex; i < (m_tailIndex + readBytes); ++i)
	{
		if (ch == m_ringBuffer[i])
		{
			findChIndex = i;
			break;
		}
	}

	// 버퍼가 가득찬 상태라면, readByte만큼 headIndex를 증가시킨다.
	// m_headIndex는 tailIndex의 다음을 가르키게 되낟.
	if (m_isFullBuffer)
	{
		m_headIndex += readBytes;
	}

	// ch를 찾았다면, head부터 findChIndex까지 out 저장한다.
	if (findChIndex >= 0)
	{
		out.reserve(MAX_BUFFERSIZE);
		for (int i = m_headIndex; i != findChIndex; i = ++i % MAX_BUFFERSIZE)
			out += m_ringBuffer[i];
		out += ch;

		// headIndex를 증가 시킨다.
		m_headIndex = (findChIndex + 1) % MAX_BUFFERSIZE;
		// tailIndex를 증가 시킨다.
		m_tailIndex += readBytes;
		m_tailIndex %= MAX_BUFFERSIZE;

		// 버퍼가 완전히 비었다면, 0으로 초기화 한다.
		if (m_tailIndex == m_headIndex)
			m_tailIndex = m_headIndex = 0;

		m_isFullBuffer = false;
	}
	else
	{
		// tailIndex를 증가 시킨다.
		m_tailIndex += readBytes;
		m_tailIndex %= MAX_BUFFERSIZE;

		// 버퍼가 끝까지 찾다면, 
		if (m_tailIndex == m_headIndex)
		{
			m_isFullBuffer = true;
		}
	}

	return true;
}


// 링버퍼 초기화.
void CBufferedSerial::ClearBuffer()
{
	m_isFullBuffer = false;
	m_headIndex = m_tailIndex = 0;
}
