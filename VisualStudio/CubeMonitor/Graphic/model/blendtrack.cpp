
#include "stdafx.h"
#include "blendtrack.h"

using namespace graphic;


cBlendTrack::cBlendTrack() 
:	m_track(NULL)
,	m_curAni(NULL)
,	m_nextAni(NULL)
,	m_aniStart(0)
,	m_aniEnd(0)
,	m_curPlayFrame(0)
,	m_incPlayFrame(0)
,	m_totalPlayFrame(0)
,	m_curPlayTime(0)
,	m_incPlayTime(0)
,	m_isAni(false)
,	m_isLoop(false)
,	m_option(0)
,	m_isBlend(false)
{
	InitBlend();
}

cBlendTrack::~cBlendTrack()
{
	SAFE_DELETE( m_track );
}


// ���ϸ��̼� ����
void cBlendTrack::PlayAnimation( 
	const Matrix44 &localTm, 
	const sRawAni &rawAni, 
	const int totalAniFrame, 
	const bool isLoop,  // false
	const bool isBlend  // false
	)
{
	m_aniStart = (int)rawAni.start;
	m_aniEnd = (int)rawAni.end;
	m_isLoop = isLoop;
	m_isAni = true;

	m_totalPlayFrame = (0 == totalAniFrame)? (rawAni.end - rawAni.start): totalAniFrame;
	m_curPlayFrame = rawAni.start;
	m_curPlayTime = rawAni.start * 0.03334f;
	m_incPlayFrame = 0;
	m_incPlayTime = 0;
	m_isBlend = false;
	m_nextAni = NULL;

	if (m_track)
	{
		const int blendTime = 5;
		if (isBlend && MakeBlendAni(m_curPlayFrame, localTm, rawAni, blendTime, m_blendAni))
		{
			m_isBlend = true;
			m_aniStart = 0;
			m_aniEnd = blendTime;
			m_curPlayFrame = 0;
			m_curPlayTime = 0;

			m_nextAni = &rawAni;
			m_tempLocalTm = localTm;

			m_track->Load(&m_blendAni);
		}
		else
		{
			m_localTm = localTm;
			m_track->Load(&rawAni);
		}
	}
	else
	{
		m_localTm = localTm;
		m_track = new cTrack(&rawAni);
	}
}


void cBlendTrack::GetCalculateTM(const Matrix44 &localTm, 
	const Matrix44 &aniTm, const Matrix44 &tm, OUT Matrix44 &out) const
{
	out = localTm * aniTm * tm;

	// ���� posŰ���� ������ local TM�� ��ǥ�� ����Ѵ�
	if ((m_option&0x01) || 
		(aniTm._41 == 0.0f && aniTm._42 == 0.0f && aniTm._43 == 0.0f))
	{
		out._41 = localTm._41;
		out._42 = localTm._42;
		out._43 = localTm._43;
	}
	else	// posŰ���� ��ǥ������ �����Ѵ�(�̷��� ���� ������ TM�� pos������ �ι�����ȴ�)
	{
		out._41 = aniTm._41;
		out._42 = aniTm._42;
		out._43 = aniTm._43;
	}
}


// �ִϸ��̼� ���.
// aniOut : �ִϸ��̼� ��� ���
// accOut : accTm��� = localTm * aniTm
bool cBlendTrack::Move( const float elapseTime, const Matrix44 &tm, 
	OUT Matrix44 &aniOut, OUT Matrix44 &accOut )
{
	RETV(!m_track, false);

	if (!CheckAnimationFrame(elapseTime))
		return false;

	m_track->Move( m_curPlayFrame, aniOut );
	
	// ���� ����� ������ �� �����Ѵ�.
	GetCalculateTM(m_localTm, aniOut, tm, accOut);

	return true;
}


// �ִϸ��̼� �������� ����ؼ�, �ִϸ��̼��� ������, ��� ���������� �Ǵ��Ѵ�.
// �ִϸ��̼� ���̶�� true�� �����ϰ�, �ִϸ��̼��� ���� �Ǿ��ٸ� false
// �� �����Ѵ�.
bool cBlendTrack::CheckAnimationFrame( const float elapseTime )
{
	m_curPlayTime += elapseTime;
	m_incPlayTime += elapseTime;
	m_curPlayFrame = (int)(m_curPlayTime * 30.f);
	m_incPlayFrame = (int)(m_incPlayTime * 30.f);

	const bool ani_loop_end = (m_curPlayFrame > m_aniEnd);	// ���ϸ��̼� ������ ���ٸ� TRUE
	const bool ani_total_end = (!m_isLoop) && (m_incPlayFrame > m_totalPlayFrame);	// �ѿ��ϸ��̼� �ð��� �����ٸ� TRUE

	if (ani_loop_end || ani_total_end)
	{
		if (m_isBlend)
		{
			// �ִϸ��̼� ������ �����ٸ� ���� �ִϸ��̼����� ��ȯ ��Ų��.
			PlayAnimation(m_tempLocalTm, *m_nextAni, m_totalPlayFrame, m_isLoop, false);
		}
		else if (m_isLoop)
		{
			// �������ϸ��̼��� ������� �ݺ� ���ϸ��̼��̶��
			// �������� ���ϸ��̼��� ó������ ������.
			m_curPlayFrame = m_aniStart;
			m_curPlayTime = m_aniStart * 0.03334f;
			m_track->InitAnimation(m_aniStart);
		}
		else
		{
			// �ݺ� ���ϸ��̼��� �ƴ϶�� 
			// �� ���ϸ��̼� �ð��� ������ ���ϸ��̼��� �����ϰ� FALSE�� �����Ѵ�.
			// �׷��� �ʴٸ� ���ϸ��̼��� ó������ ������.				
			if (ani_loop_end)
			{
				// �ִϸ��̼��� ���� �������� ���ϰ� �Ѵ�.
				m_curPlayFrame = m_aniEnd;
				m_curPlayTime = m_aniEnd * 0.03334f;

				// �� ���ϸ��̼��� ������ �ʾҴٸ� ���ϸ��̼� ������ ó������ �ǵ�����.
				// �� ���ϸ��̼��� �����ٸ� ������ �ǵ����� �ʰ� ������ �������� ���ϰ� �������д�.
				// ���� ���ϸ��̼ǿ��� �����Ǳ� ���ؼ� ������ ���������� �ξ�� �Ѵ�.
				if (!ani_total_end)
					m_track->InitAnimation(m_aniStart);
			}

			if (ani_total_end)
			{
				m_isAni = false;
				return false; // �ִϸ��̼� ����.
			}
		}
	}

	return true;
}


// ���� �ִϸ��̼ǰ� ���� �ִϸ��̼��� �����ϴ� ������ �����Ѵ�.
// ��� ������ sRawAni Ÿ������ ����Ǿ� ���ϵȴ�.
bool cBlendTrack::MakeBlendAni(	
	const int curFrame,
	const Matrix44 &nextLocalTm,
	const sRawAni &nextRawAni, 
	const int blendTime, // =0
	sRawAni &out)
{
	RETV(!m_track, false);

	// ���� �ִϸ��̼� ������ �����´�.
	out.rot[ 0].t = 0;
	out.rot[ 0].q = Quaternion(0,0,0,1);
	m_track->GetRotKey( curFrame, out.rot[ 0].q );
	out.pos[ 0].t = 0;
	out.pos[ 0].p = Vector3( 0, 0, 0 );
	m_track->GetPosKey( curFrame, out.pos[ 0].p );
	out.scale[ 0].t = 0;
	out.scale[ 0].s = Vector3( 1, 1, 1 );
	m_track->GetScaleKey( curFrame, out.scale[ 0].s );


	// �ι�° ������ �ִϸ��̼� ������ �����´�.
	// A X B = C
	// A : ���� localTM
	// B : ?
	// C : ���� �ִϸ��̼��� �� localTM
	// B = A.inverse() * C

	Matrix44 local = m_localTm;
	local._41 = local._42 = local._43 = 0;
	local.Transpose(); // inverse localTm
	Matrix44 itm = local * nextLocalTm;

	out.pos[ 1] = nextRawAni.pos[ 0];
	out.rot[ 1].q = itm.GetQuaternion();
	out.scale[ 1].s = Vector3(1,1,1);
	out.pos[ 1].t = (float)blendTime;
	out.rot[ 1].t = (float)blendTime;
	out.scale[ 1].t = (float)blendTime;

	return true;
}


void cBlendTrack::InitBlend()
{
	m_blendAni.start = 0;
	m_blendAni.end = 0;
	m_blendAni.pos.resize(2);
	m_blendAni.rot.resize(2);
	m_blendAni.scale.resize(2);
}


// ���� �ִϸ��̼� �������� �����ϰ�, �� ����� �����Ѵ�.
// aniOut : �ִϸ��̼� ��� ���
// accOut : accTm��� = localTm * aniTm
void cBlendTrack::SetCurrentFrame( const int curFrame,  const Matrix44 &tm, 
	OUT Matrix44 &aniOut, OUT Matrix44 &accOut  )
{
	RET(!m_track);

	m_curPlayTime = curFrame / 30.f;
	m_curPlayFrame = curFrame; 

	m_track->SetCurrentFramePos(curFrame);
	m_track->Move(curFrame, aniOut);

	// ���� ����� ������ �� �����Ѵ�.
	GetCalculateTM(m_localTm, aniOut, tm, accOut);
}
