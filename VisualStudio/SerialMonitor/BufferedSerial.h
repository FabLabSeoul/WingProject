//
// author: jjuiddong
//  2015-04-04
//
// CSerial Ŭ������ ��� �޾�, ReadStringUntil() �Լ��� ������ Ŭ������.
// Ư�� ���� (���๮��) ������ ������ �о� �� ��, ������� �����ϴ� CSerial Ŭ������
// ������� ����, ���۸� ȿ�������� �����ϸ鼭, ���ϴ� ����� �����ϴ� ������ �Ѵ�.
//
// �����۷� �����Ǿ, ���� ���Ҵ�, ���簡 �Ͼ�� �ʰ� �ߴ�.
//
// �� Ŭ������ ����� ���, ReadData()�Լ� ���, ReadStringUntil() �Լ��� ��� �Ѵ�.
//
#pragma once
#include "Serial.h"


class CBufferedSerial : public CSerial
{
public:
	CBufferedSerial();
	virtual ~CBufferedSerial();

	bool ReadStringUntil(const char ch, OUT string &out);
	void ClearBuffer();


protected:
	enum {MAX_BUFFERSIZE = 256};

	char m_ringBuffer[MAX_BUFFERSIZE]; 
	// ������ head ���� tail �� ����ȴ�. ���� ���� ������ tail ���� ����ȴ�.
	// headIndex == tailIndex ������ �����ϸ�, ���۰� ����ٴ� ���̴�.
	int m_headIndex = 0;  // ����� ������ ���۵Ǵ� index�� ����Ų��.
	int m_tailIndex = 0; // ������ ���� ������ index�� ����Ų��.
	bool m_isFullBuffer = false; // ���۰� ����á�� ��, true�� �ȴ�. ȿ������ �޸� ������ ���� �ʿ���.
};
