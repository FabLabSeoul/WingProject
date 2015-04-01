
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


// 에니메이션 시작
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

	// 만약 pos키값이 없으면 local TM의 좌표를 사용한다
	if ((m_option&0x01) || 
		(aniTm._41 == 0.0f && aniTm._42 == 0.0f && aniTm._43 == 0.0f))
	{
		out._41 = localTm._41;
		out._42 = localTm._42;
		out._43 = localTm._43;
	}
	else	// pos키값을 좌표값으로 적용한다(이렇게 하지 않으면 TM의 pos성분이 두번적용된다)
	{
		out._41 = aniTm._41;
		out._42 = aniTm._42;
		out._43 = aniTm._43;
	}
}


// 애니메이션 계산.
// aniOut : 애니메이션 결과 행렬
// accOut : accTm행렬 = localTm * aniTm
bool cBlendTrack::Move( const float elapseTime, const Matrix44 &tm, 
	OUT Matrix44 &aniOut, OUT Matrix44 &accOut )
{
	RETV(!m_track, false);

	if (!CheckAnimationFrame(elapseTime))
		return false;

	m_track->Move( m_curPlayFrame, aniOut );
	
	// 최종 행렬을 연산한 후 리턴한다.
	GetCalculateTM(m_localTm, aniOut, tm, accOut);

	return true;
}


// 애니메이션 프레임을 계산해서, 애니메이션을 끝낼지, 계속 진행할지를 판단한다.
// 애니메이션 중이라면 true를 리턴하고, 애니메이션이 종료 되었다면 false
// 를 리턴한다.
bool cBlendTrack::CheckAnimationFrame( const float elapseTime )
{
	m_curPlayTime += elapseTime;
	m_incPlayTime += elapseTime;
	m_curPlayFrame = (int)(m_curPlayTime * 30.f);
	m_incPlayFrame = (int)(m_incPlayTime * 30.f);

	const bool ani_loop_end = (m_curPlayFrame > m_aniEnd);	// 에니메이션 끝까지 갔다면 TRUE
	const bool ani_total_end = (!m_isLoop) && (m_incPlayFrame > m_totalPlayFrame);	// 총에니메이션 시간이 지났다면 TRUE

	if (ani_loop_end || ani_total_end)
	{
		if (m_isBlend)
		{
			// 애니메이션 블렌딩이 끝났다면 다음 애니메이션으로 전환 시킨다.
			PlayAnimation(m_tempLocalTm, *m_nextAni, m_totalPlayFrame, m_isLoop, false);
		}
		else if (m_isLoop)
		{
			// 보간에니메이션이 종료된후 반복 에니메이션이라면
			// 보간없이 에니메이션을 처음으로 돌린다.
			m_curPlayFrame = m_aniStart;
			m_curPlayTime = m_aniStart * 0.03334f;
			m_track->InitAnimation(m_aniStart);
		}
		else
		{
			// 반복 에니메이션이 아니라면 
			// 총 에니메이션 시간이 지나면 에니메이션을 종료하고 FALSE를 리턴한다.
			// 그렇지 않다면 에니메이션을 처음으로 돌린다.				
			if (ani_loop_end)
			{
				// 애니메이션의 가장 마지막을 향하게 한다.
				m_curPlayFrame = m_aniEnd;
				m_curPlayTime = m_aniEnd * 0.03334f;

				// 총 에니메이션이 끝나지 않았다면 에니메이션 정보를 처음으로 되돌린다.
				// 총 에니메이션이 끝났다면 정보를 되돌리지 않고 마지막 프레임을 향하게 내버려둔다.
				// 다음 에니메이션에서 보간되기 위해서 마지막 프레임으로 두어야 한다.
				if (!ani_total_end)
					m_track->InitAnimation(m_aniStart);
			}

			if (ani_total_end)
			{
				m_isAni = false;
				return false; // 애니메이션 종료.
			}
		}
	}

	return true;
}


// 현재 애니메이션과 다음 애니메이션을 보간하는 정보를 생성한다.
// 결과 정보는 sRawAni 타입으로 저장되어 리턴된다.
bool cBlendTrack::MakeBlendAni(	
	const int curFrame,
	const Matrix44 &nextLocalTm,
	const sRawAni &nextRawAni, 
	const int blendTime, // =0
	sRawAni &out)
{
	RETV(!m_track, false);

	// 현재 애니메이션 정보를 가져온다.
	out.rot[ 0].t = 0;
	out.rot[ 0].q = Quaternion(0,0,0,1);
	m_track->GetRotKey( curFrame, out.rot[ 0].q );
	out.pos[ 0].t = 0;
	out.pos[ 0].p = Vector3( 0, 0, 0 );
	m_track->GetPosKey( curFrame, out.pos[ 0].p );
	out.scale[ 0].t = 0;
	out.scale[ 0].s = Vector3( 1, 1, 1 );
	m_track->GetScaleKey( curFrame, out.scale[ 0].s );


	// 두번째 보간될 애니메이션 정보를 가져온다.
	// A X B = C
	// A : 현재 localTM
	// B : ?
	// C : 다음 애니메이션의 본 localTM
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


// 현재 애니메이션 프래임을 설정하고, 그 결과를 리턴한다.
// aniOut : 애니메이션 결과 행렬
// accOut : accTm행렬 = localTm * aniTm
void cBlendTrack::SetCurrentFrame( const int curFrame,  const Matrix44 &tm, 
	OUT Matrix44 &aniOut, OUT Matrix44 &accOut  )
{
	RET(!m_track);

	m_curPlayTime = curFrame / 30.f;
	m_curPlayFrame = curFrame; 

	m_track->SetCurrentFramePos(curFrame);
	m_track->Move(curFrame, aniOut);

	// 최종 행렬을 연산한 후 리턴한다.
	GetCalculateTM(m_localTm, aniOut, tm, accOut);
}
