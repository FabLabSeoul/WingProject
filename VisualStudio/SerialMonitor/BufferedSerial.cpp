
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


// ch���ڰ� ���� ������ �ø��������� �о �����Ѵ�.
// ch���ڰ� ���ٸ� ���ڿ��� �����Ѵ�.
bool CBufferedSerial::ReadStringUntil(const char ch, OUT string &out)
{
	const int readBytes = ReadData(&m_ringBuffer[m_tailIndex], MAX_BUFFERSIZE - m_tailIndex);
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

	// ���۰� ������ ���¶��, readByte��ŭ headIndex�� ������Ų��.
	// m_headIndex�� tailIndex�� ������ ����Ű�� �ǳ�.
	if (m_isFullBuffer)
	{
		m_headIndex += readBytes;
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

		m_isFullBuffer = false;
	}
	else
	{
		// tailIndex�� ���� ��Ų��.
		m_tailIndex += readBytes;
		m_tailIndex %= MAX_BUFFERSIZE;

		// ���۰� ������ ã�ٸ�, 
		if (m_tailIndex == m_headIndex)
		{
			m_isFullBuffer = true;
		}
	}

	return true;
}


// ������ �ʱ�ȭ.
void CBufferedSerial::ClearBuffer()
{
	m_isFullBuffer = false;
	m_headIndex = m_tailIndex = 0;
}
