
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


// ch���ڰ� ���� ������ �ø��������� �о �����Ѵ�.
// ch���ڰ� ���ٸ� ���ڿ��� �����Ѵ�.
// ���۰� ����á�ٸ�, �� ������ ���� ����Ÿ�� �����Ѵ�.
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

		// ���۰� ����á�ٸ�, ��� �����ؼ� �����Ѵ�.
		out.reserve(MAX_BUFFERSIZE);
		for (int i = m_headIndex; i != m_tailIndex; i = (i + 1) % MAX_BUFFERSIZE)
			out += m_ringBuffer[i];

		m_headIndex = m_tailIndex = 0;

		return true; // ����
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


	// ���� ��ġ���� ch ���ڰ� �ִ��� �˻��Ѵ�.
	int findChIndex = -1;
	for (int i = m_tailIndex; i < (m_tailIndex + readBytes); ++i)
	{
		if (ch == m_ringBuffer[i])
		{
			findChIndex = i;
			break;
		}
	}

	// ch�� ã�Ҵٸ�, head���� findChIndex���� out �����Ѵ�.
	if (findChIndex >= 0)
	{
		out.reserve(MAX_BUFFERSIZE);
		for (int i = m_headIndex; i != findChIndex; i = ++i % MAX_BUFFERSIZE)
			out += m_ringBuffer[i];
		out += ch;

		// headIndex�� ���� ��Ų��.
		m_headIndex = (findChIndex + 1) % MAX_BUFFERSIZE;
		// tailIndex�� ���� ��Ų��.
		m_tailIndex += readBytes;
		m_tailIndex %= MAX_BUFFERSIZE;

		// ���۰� ������ ����ٸ�, 0���� �ʱ�ȭ �Ѵ�.
		if (m_tailIndex == m_headIndex)
			m_tailIndex = m_headIndex = 0;

		m_lastReturnTime = 0;
	}
	else
	{
		// tailIndex�� ���� ��Ų��.
		m_tailIndex += readBytes;
		m_tailIndex %= MAX_BUFFERSIZE;

		return false;
	}

	return true;
}


// ������ �ʱ�ȭ.
void CBufferedSerial::ClearBuffer()
{
	m_lastReturnTime = 0;
	m_headIndex = m_tailIndex = 0;
}


// �ִ� ���ð��� �����Ѵ�.
// ReadStringUntil() �Լ��� ch�� ���������� �ִ� ���ð��� �ѱ��,
// �� �� ���� ���� ������ �����Ѵ�.
// ���� �ֱٿ� ������ ���� ���ĺ���, ���ð��� ����Ѵ�.
void CBufferedSerial::SetMaxWaitTime(const int milliseconds)
{
	m_maxWaitTime = milliseconds;
}
