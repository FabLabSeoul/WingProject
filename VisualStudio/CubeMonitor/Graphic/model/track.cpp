
#include "stdafx.h"
#include "track.h"

using namespace graphic;


cTrack::cTrack(const sRawAni *rawAni) :
	m_rawAni(rawAni)
,	m_curFrame(0)
,	m_keyPosIdx(-1)
,	m_keyRotIdx(-1)
,	m_keyScaleIdx(-1)
{
	InitAnimation();
}

cTrack::~cTrack()
{
}


// Animation
void cTrack::Move( const int curFrame, OUT Matrix44 &out )
{
	RET(!m_rawAni);

	m_curFrame = curFrame;

	Quaternion q;
	if (GetRotKey(curFrame, q))
		out = q.GetMatrix();

	Vector3 p(0,0,0);
	if (GetPosKey(curFrame, p))
	{
		out._41 = p.x;
		out._42 = p.y;
		out._43 = p.z;
	}

	Vector3 s(1,1,1);
	if (GetScaleKey(curFrame, s))
	{
		out._11 *= s.x;
		out._22 *= s.y;
		out._33 *= s.z;
	}
}


// curFrame �� ���� ��ġ ���� ���´�.
bool cTrack::GetPosKey( const int curFrame, OUT Vector3 &out )
{
	RETV(!m_curKeyPos[ 0], false);

	if (!m_curKeyPos[ 1])
	{
		out = m_curKeyPos[ 0]->p;
		return true;
	}

	// �������� ���� ��带 ����Ű�� ���� ���ϸ��̼����� �Ѿ��.
	if (curFrame > m_curKeyPos[ 1]->t)
	{
		// ���ϸ��̼��� �������̸� ������ Ű���� �����Ѵ�.
		if ((int)m_rawAni->pos.size() <= m_keyPosIdx+1 )
		{
			out = m_curKeyPos[ 1]->p;
			return true;
		}

		// ���� ���ϸ��̼����� �Ѿ��.
		++m_keyPosIdx;
		m_curKeyPos[ 0] = m_curKeyPos[ 1];
		m_curKeyPos[ 1] = (sKeyPos*)&m_rawAni->pos[ m_keyPosIdx];
	}

	// curKeyPos[0]->t ���� curKeyPos[1]->t ���̿��� curFrame�� ��� ��ġ�ϴ��� 0~1 ���� ������ 
	// �����Ѵ�.
	const float alpha = GetAlpha( m_curKeyPos[ 0]->t, m_curKeyPos[ 1]->t, (float)curFrame );
	out = m_curKeyPos[ 0]->p.Interpolate( m_curKeyPos[ 1]->p, alpha );
	return true;
}


//  curFrame �� ���� ȸ�� ���� ���´�.
bool cTrack::GetRotKey( const int curFrame, OUT Quaternion &out )
{
	RETV(!m_curKeyRot[ 0], false);

	if (!m_curKeyRot[ 1])
	{
		out = m_curKeyRot[ 0]->q;
		return true;
	}

	// �������� ���� ��带 ����Ű�� ���� ���ϸ��̼����� �Ѿ��.
	if (curFrame > m_curKeyRot[ 1]->t)
	{
		// ���ϸ��̼��� �������̸� ������ Ű���� �����Ѵ�.
		if ((int)m_rawAni->rot.size() <= m_keyRotIdx+1 )
		{
			out = m_curKeyRot[ 1]->q;
			return true;
		}

		// ���� ���ϸ��̼����� �Ѿ��.
		++m_keyRotIdx;
		m_curKeyRot[ 0] = m_curKeyRot[ 1];
		m_curKeyRot[ 1] = (sKeyRot*)&m_rawAni->rot[ m_keyRotIdx];
	}

	const float alpha = GetAlpha( m_curKeyRot[ 0]->t, m_curKeyRot[ 1]->t, (float)curFrame );
	out = m_curKeyRot[ 0]->q.Interpolate( m_curKeyRot[ 1]->q, alpha );
	return true;
}


// curFrame �� ���� ������ ���� ���´�.
bool cTrack::GetScaleKey( const int curFrame, OUT Vector3 &out )
{
	return false;

	//RETV(!m_curKeyScale[ 0], false);

	// //�ϴ� ��а� �����ϸ��ϴ� ���ϸ��̼��� �����Ѵ�.
	//if (!m_curKeyScale[ 1])
	//{
	//	out = m_curKeyScale[ 0]->s;
	//	return true;
	//}

	//// �������� ���� ��带 ����Ű�� ���� ���ϸ��̼����� �Ѿ��.
	//if (curFrame > m_curKeyScale[ 1]->t)
	//{
	//	// ���ϸ��̼��� �������̸� ������ Ű���� �����Ѵ�.
	//	if ((int)m_rawAni->scale.size() <= m_keyScaleIdx+1)
	//	{
	//		out = m_curKeyScale[ 1]->s;
	//		return true;
	//	}

	//	// ���� ���ϸ��̼����� �Ѿ��.
	//	++m_keyScaleIdx;
	//	m_curKeyScale[ 0] = m_curKeyScale[ 1];
	//	m_curKeyScale[ 1] = (sKeyScale*)&m_rawAni->scale[ m_keyScaleIdx];
	//}

	//const float alpha = GetAlpha( m_curKeyScale[ 0]->t, m_curKeyScale[ 1]->t, (float)curFrame );
	//out = m_curKeyScale[ 0]->s.Interpolate( m_curKeyScale[ 1]->s, alpha );
	//return true;
}


// �ִϸ��̼� �ʱ�ȭ, ó������ ���ư���.
// curFrame : ���� ������.
void cTrack::InitAnimation(const int curFrame ) // curFrame=0
{
	RET(!m_rawAni);

	if (curFrame > 0)
	{
		SetCurrentFramePos(curFrame);
		return;
	}

	ZeroMemory( m_curKeyPos, sizeof(m_curKeyPos) );
	ZeroMemory( m_curKeyRot, sizeof(m_curKeyRot) );
	ZeroMemory( m_curKeyScale, sizeof(m_curKeyScale) );

	if (1 <= m_rawAni->pos.size())
	{
		m_keyPosIdx = 1;
		m_curKeyPos[ 0] = (sKeyPos*)&m_rawAni->pos[ 0];
		if (2 <= m_rawAni->pos.size())
			m_curKeyPos[ 1] = (sKeyPos*)&m_rawAni->pos[ 1];
	}

	if (1 <= m_rawAni->rot.size())
	{
		m_keyRotIdx = 1;
		m_curKeyRot[ 0] = (sKeyRot*)&m_rawAni->rot[ 0];
		if (2 <= m_rawAni->rot.size())
			m_curKeyRot[ 1] = (sKeyRot*)&m_rawAni->rot[ 1];
	}
	
	if (1 <= m_rawAni->scale.size())
	{
		m_keyScaleIdx = 1;
		m_curKeyScale[ 0] = (sKeyScale*)&m_rawAni->scale[ 0];
		if (2 <= m_rawAni->scale.size())
			m_curKeyScale[ 1] = (sKeyScale*)&m_rawAni->scale[ 1];
	}
}


// ���ϸ��̼� ����Ÿ �ε�.
// ���ϸ��̼��� ó�����·� ���´�.
bool cTrack::Load( const sRawAni *rawAni )
{
	m_rawAni = rawAni;	
	InitAnimation(); 
	return true;
}


// Get Alpha
float cTrack::GetAlpha(float f1, float f2, float frame ) const 
{
	if (FLOAT_EQ(f1, f2))
		return 0;

	const float f = (frame-f1) / (f2-f1);
	return max(0, min(1, f)); 
}


// curFrame �� �´� m_keyPosIdx, m_keyRotIdx, m_keyScaleIdx �� �����Ѵ�.
void cTrack::SetCurrentFramePos( const int curFrame )
{
	RET(!m_rawAni);

	m_keyPosIdx = 0;
	m_keyRotIdx = 0;
	m_keyScaleIdx = 0;

	ZeroMemory( m_curKeyPos, sizeof(m_curKeyPos) );
	ZeroMemory( m_curKeyRot, sizeof(m_curKeyRot) );
	ZeroMemory( m_curKeyScale, sizeof(m_curKeyScale) );

	// position index setting
	for (u_int i=0; i < m_rawAni->pos.size(); ++i)
	{
		if (m_rawAni->pos[ i].t < curFrame)
		{
			++m_keyPosIdx;
		}
	}

	if (m_keyPosIdx <= 0)
	{
		if (1 <= m_rawAni->pos.size())
			m_curKeyPos[ 0] = (sKeyPos*)&m_rawAni->pos[ 0];
	}
	else
	{
		m_curKeyPos[ 0] = (sKeyPos*)&m_rawAni->pos[ m_keyPosIdx-1];
		if (m_keyPosIdx < (int)m_rawAni->pos.size())
			m_curKeyPos[ 1] = (sKeyPos*)&m_rawAni->pos[ m_keyPosIdx];
	}


	// rotation index setting
	for (u_int i=0; i < m_rawAni->rot.size(); ++i)
	{
		if (m_rawAni->rot[ i].t < curFrame)
		{
			++m_keyRotIdx;
		}
	}

	if (m_keyRotIdx <= 0)
	{
		if (1 <= m_rawAni->rot.size())
			m_curKeyRot[ 0] = (sKeyRot*)&m_rawAni->rot[ 0];
	}
	else
	{
		m_curKeyRot[ 0] = (sKeyRot*)&m_rawAni->rot[ m_keyRotIdx-1];
		if (m_keyRotIdx < (int)m_rawAni->rot.size())
			m_curKeyRot[ 1] = (sKeyRot*)&m_rawAni->rot[ m_keyRotIdx];
	}


	// scale index setting
	for (u_int i=0; i < m_rawAni->scale.size(); ++i)
	{
		if (m_rawAni->scale[ i].t < curFrame)
		{
			++m_keyScaleIdx;
		}
	}

	if (m_keyScaleIdx <= 0)
	{
		if (1 <= m_rawAni->scale.size())
			m_curKeyScale[ 0] = (sKeyScale*)&m_rawAni->scale[ 0];
	}
	else
	{
		m_curKeyScale[ 0] = (sKeyScale*)&m_rawAni->scale[ m_keyScaleIdx-1];
		if (m_keyScaleIdx < (int)m_rawAni->scale.size())
			m_curKeyScale[ 1] = (sKeyScale*)&m_rawAni->scale[ m_keyScaleIdx];
	}
}
