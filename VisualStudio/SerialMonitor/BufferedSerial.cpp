
#include "stdafx.h"
#include "BufferedSerial.h"
#include <string>
using std::string;


CBufferedSerial::CBufferedSerial() :
	m_maxWaitTime(500)
	, m_lastReturnTime(0)
{
}

CBufferedSerial::~CBufferedSerial()
{
}


// ch문자가 있을 때까지 시리얼정보를 읽어서 리턴한다.
// ch문자가 없다면 빈문자열을 리턴한다.
// 버퍼가 가득찼다면, 그 때까지 받은 데이타를 리턴한다.
bool CBufferedSerial::ReadStringUntil(const char ch, OUT string &out)
{
	const int curT = timeGetTime();
	if (0 == m_lastReturnTime)
		m_lastReturnTime = curT;
	const int elapseTime = curT - m_lastReturnTime;

	const bool isFull = (m_headIndex == (m_tailIndex + 1) % MAX_BUFFERSIZE); // is full?

	int readBytes = 0;
	if (isFull || (elapseTime > m_maxWaitTime))
	{
		m_lastReturnTime = 0;

		if (m_headIndex == m_tailIndex) // is empty?
			return false;

		// 버퍼가 가득찼다면, 모두 복사해서 리턴한다.
		out.reserve(MAX_BUFFERSIZE);
		for (int i = m_headIndex; i != m_tailIndex; i = (i + 1) % MAX_BUFFERSIZE)
			out += m_ringBuffer[i];

		m_headIndex = m_tailIndex = 0;

		return true; // 종료
	}
	else
	{
		int remainBufferSize = 0;
		if (m_tailIndex == m_headIndex)
			remainBufferSize = MAX_BUFFERSIZE - 1;
		else if (m_tailIndex > m_headIndex)
			remainBufferSize = MAX_BUFFERSIZE - m_tailIndex - 1;
		else
			remainBufferSize = m_tailIndex - m_headIndex - 1;

		readBytes = ReadData(&m_ringBuffer[m_tailIndex], remainBufferSize);
	}

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

		m_lastReturnTime = 0;
	}
	else
	{
		// tailIndex를 증가 시킨다.
		m_tailIndex += readBytes;
		m_tailIndex %= MAX_BUFFERSIZE;

		return false;
	}

	return true;
}


// 링버퍼 초기화.
void CBufferedSerial::ClearBuffer()
{
	m_lastReturnTime = 0;
	m_headIndex = m_tailIndex = 0;
}


// 최대 대기시간을 설정한다.
// ReadStringUntil() 함수가 ch가 오기전에도 최대 대기시간을 넘기면,
// 그 때 까지 받은 정보를 리턴한다.
// 가장 최근에 정보를 받은 이후부터, 대기시간을 계산한다.
void CBufferedSerial::SetMaxWaitTime(const int milliseconds)
{
	m_maxWaitTime = milliseconds;
}
