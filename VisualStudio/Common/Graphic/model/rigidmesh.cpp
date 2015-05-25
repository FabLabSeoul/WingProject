
#include "stdafx.h"
#include "rigidmesh.h"

using namespace graphic;


cRigidMesh::cRigidMesh(const int id, const sRawMesh &raw) : 
	cMesh(id, raw)
,	m_track(NULL)
,	m_aniTime(0)
,	m_aniStart(0)
,	m_aniEnd(0)
,	m_aniFrame(0)
{

}

cRigidMesh::~cRigidMesh()
{
	SAFE_DELETE(m_track);
}


// �ִϸ��̼� �ε�.
void cRigidMesh::LoadAnimation( const sRawAni &rawAni )
{
	SAFE_DELETE(m_track);

	m_track = new cTrack(&rawAni);
	m_aniStart = (int)rawAni.start;
	m_aniEnd = (int)rawAni.end;
	m_aniFrame = (int)rawAni.start;
}


// �ִϸ��̼� ó��
bool cRigidMesh::Move(const float elapseTime)
{
	RETV(!m_track, false);

	//  ������ ������ ��ȯ�Ѵ�.
	m_aniTime += elapseTime;
	m_aniFrame = (int)(m_aniTime * 30.f);

	if (m_aniFrame > m_aniEnd)
	{
		m_aniTime = m_aniStart * 30.f;
		m_aniFrame = m_aniStart;
		m_track->InitAnimation();
	}

	m_aniTM.SetIdentity();
	m_track->Move(m_aniFrame, m_aniTM);
	return true;
}


// ���.
void cRigidMesh::Render(const Matrix44 &parentTm)
{
	if (m_shader)
	{
		cMesh::RenderShader(*m_shader, parentTm);
		return;
	}

	cMesh::Render(parentTm);
}


// ���̴� ���.
//void cRigidMesh::RenderShader( const Matrix44 &parentTm )
//{
//	if (cShader *shader = cResourceManager::Get()->LoadShader("rigid.fx"))
//	{
//		cMesh::RenderShader(*shader, parentTm);
//	}
//}
