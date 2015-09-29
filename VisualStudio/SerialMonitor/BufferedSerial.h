//
// author: jjuiddong
//  2015-04-04
//
// CSerial 클래스를 상속 받아, ReadStringUntil() 함수를 구현한 클래스다.
// 특정 문자 (개행문자) 단위로 정보를 읽어 올 때, 고속으로 동작하는 CSerial 클래스를
// 오버헤드 없이, 버퍼를 효율적으로 관리하면서, 원하는 기능을 제공하는 역할을 한다.
//
// 링버퍼로 구현되어서, 버퍼 재할당, 복사가 일어나지 않게 했다.
//
// 이 클래스를 사용할 경우, ReadData()함수 대신, ReadStringUntil() 함수를 써야 한다.
//
//
// 2015-08-25
// 일정 시간 이상동안 ch문자가 들어오지 않으면, 버퍼를 리턴한다.
//
#pragma once
#include "Serial.h"


class CBufferedSerial : public CSerial
{
public:
	CBufferedSerial();
	virtual ~CBufferedSerial();

	bool ReadStringUntil(const char ch, OUT string &out);
	void SetMaxWaitTime(const int milliseconds);
	void ClearBuffer();


protected:
	enum {MAX_BUFFERSIZE = 256};

	char m_ringBuffer[MAX_BUFFERSIZE]; 
	// 정보는 head 에서 tail 로 저장된다. 새로 들어온 정보는 tail 부터 저장된다.
	// headIndex == tailIndex 조건이 성립하면, 버퍼가 비었다는 뜻이다.
	int m_headIndex = 0;  // 저장된 정보가 시작되는 index를 가르킨다.
	int m_tailIndex = 0; // 정보를 쓰기 시작할 index를 가르킨다.
	int m_maxWaitTime; // default = 500
	int m_lastReturnTime; // m_maxWaitTime 시간이 지나도록 문자 ch가 오지 않는다면, 리턴한다.
};
