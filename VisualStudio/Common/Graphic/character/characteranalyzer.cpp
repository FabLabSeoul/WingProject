
#include "stdafx.h"
#include "character.h"
#include "characteranalyzer.h"

using namespace graphic;


cCharacterAnalyzer::cCharacterAnalyzer() :
	m_selectBone(NULL)
,	m_character(NULL)
{

}

cCharacterAnalyzer::~cCharacterAnalyzer()
{
}


//// ĳ���� ���� �ε�.
//bool cCharacterAnalyzer::Create(const string &modelName)
//{
//	const bool result = cCharacter::Create(modelName);
//
//	m_selectBone = NULL;
//	m_selectBoneCube.SetCube(Vector3(0,0,0), Vector3(0,0,0));
//
//	return result;
//}


bool cCharacterAnalyzer::Move(const float elapseTime)
{
	RETV(!m_character, false);

	m_character->Move(elapseTime);

	if (m_selectBone)
		m_selectBoneCube.SetTransform( m_selectBone->GetAccTM() );

	return true;
}


void cCharacterAnalyzer::Render(const Matrix44 &tm)
{
	RET(!m_character);

	m_character->Render(tm);

	// ���õ� �� ��� �ڽ� ���.
	GetDevice()->SetRenderState(D3DRS_ZENABLE, 0);	
	m_selectBoneCube.Render(m_character->GetTransform() * tm);
	GetDevice()->SetRenderState(D3DRS_ZENABLE, 1);

}


// ���� �����Ѵ�.
// ���ڽ��� ���� ����Ѵ�.
void cCharacterAnalyzer::HighlightBone(const string &boneName)
{
	RET(!m_character);
	cBoneMgr *boneMgr = m_character->GetBoneMgr();
	RET(!boneMgr);
	
	cBoneNode *node = boneMgr->FindBone(boneName);
	if ((!node) || (node->GetId() < 0) || (node->GetId() >= (int)boneMgr->GetBoundingBoxes().size()))
	{
		m_selectBoneCube.SetCube(Vector3(0,0,0), Vector3(0,0,0));
		return;
	}

	m_selectBone = node;
	m_selectBoneCube.SetColor(0x00FF00);
	m_selectBoneCube.SetCube( boneMgr->GetBoundingBoxes()[ node->GetId()] );
	if (m_selectBoneCube.Length() < 0.001f)
	{
		const float len = max(m_character->GetCollisionBox()->Length() / 100.f, 1);
		m_selectBoneCube.SetCube(Vector3(-1,-1,-1)*len, Vector3(1,1,1)*len);
	}
}
