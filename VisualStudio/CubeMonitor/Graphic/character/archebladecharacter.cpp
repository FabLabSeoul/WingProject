
#include "stdafx.h"
#include "archebladecharacter.h"


using namespace graphic;


cArchebladeCharacter::cArchebladeCharacter(const int id) :
	cCharacter(id)
	,	m_weapon(NULL)
	,	m_weaponNode1(NULL)
	,	m_weaponNode2(NULL)
	,	m_weaponNode3(NULL)
	,	m_weaponBoneNode1(NULL)
	,	m_weaponBoneNode2(NULL)
	,	m_weaponBoneNode3(NULL)
{

}

cArchebladeCharacter::~cArchebladeCharacter()
{
	SAFE_DELETE(m_weapon);
}


bool cArchebladeCharacter::Create(const string &modelName, MODEL_TYPE::TYPE type)
// type = MODEL_TYPE::AUTO
{
	m_weaponNode1 = NULL;
	m_weaponNode2 = NULL;
	m_weaponNode3 = NULL;
	m_weaponBoneNode1 = NULL;
	m_weaponBoneNode2 = NULL;
	m_weaponBoneNode3 = NULL;

	SAFE_DELETE(m_weapon);

	if (cModel::Create(modelName))
	{
		if (GetModelType() == MODEL_TYPE::SKIN)
		{
			SetShader( 
				cResourceManager::Get()->LoadShader("hlsl_skinning_using_texcoord.fx") );
		}

		return true;
	}

	return false;
}


void cArchebladeCharacter::LoadWeapon(const string &fileName)
{
	m_weaponNode1 = NULL;
	m_weaponNode2 = NULL;
	m_weaponNode3 = NULL;
	m_weaponBoneNode1 = NULL;
	m_weaponBoneNode2 = NULL;
	m_weaponBoneNode3 = NULL;

	SAFE_DELETE(m_weapon);
	RET(!m_bone);
	//m_weaponNode = m_bone->FindBone("dummy_weapon");

	// valle
	m_weaponNode1 = m_bone->FindBone("Handle02");

	// tiac1
	if (!m_weaponNode1)
		m_weaponNode1 = m_bone->FindBone("Mace_Hand_Dummy");
	// tiac2
	m_weaponNode2= m_bone->FindBone("Shield_Rotate_Dummy");

	// hyde
	if (!m_weaponNode1)
		m_weaponNode1 = m_bone->FindBone("Bip01-L-Hand");
	if (!m_weaponNode2)
		m_weaponNode2 = m_bone->FindBone("Bip01-R-Hand");
	if (!m_weaponNode3)
		m_weaponNode3 = m_bone->FindBone("Bip01-Spine2");

	// elichea
	if (!m_weaponNode1)
		m_weaponNode1 = m_bone->FindBone("Dummy002_L");
	// tiac2
	if (!m_weaponNode2)
		m_weaponNode2= m_bone->FindBone("Dummy002_R");


	//RET(!m_weaponNode1);
	//RET(!m_weaponNode2);

	if (!m_weapon)
	{
		m_weapon = new cModel(common::GenerateId());
		m_weapon->SetShader( 
			cResourceManager::Get()->LoadShader("hlsl_skinning_using_texcoord_unlit.fx") );
	}

	if (!m_weapon->Create(fileName))
		return;

	RET(!m_weapon->GetBoneMgr());

	// valle
	m_weaponBoneNode1 = m_weapon->GetBoneMgr()->FindBone("Handle02");

	// tiac1
	if (!m_weaponBoneNode1)
		m_weaponBoneNode1 = m_weapon->GetBoneMgr()->FindBone("Mace_Hand_Dummy");
	// tiac2
	m_weaponBoneNode2 = m_weapon->GetBoneMgr()->FindBone("Shield_Rotate_Dummy");


	// hyde
	if (!m_weaponBoneNode1)
		m_weaponBoneNode1 =  m_weapon->GetBoneMgr()->FindBone("Dagger_L_EndHandleBone");
	if (!m_weaponBoneNode2)
		m_weaponBoneNode2 =  m_weapon->GetBoneMgr()->FindBone("Dagger_R_EndHandleBone");
	if (!m_weaponBoneNode3)
		m_weaponBoneNode3 =  m_weapon->GetBoneMgr()->FindBone("Bip01-Spine2");


	// elichea
	if (!m_weaponBoneNode1)
		m_weaponBoneNode1 = m_weapon->GetBoneMgr()->FindBone("Dummy002_L");
	// tiac2
	if (!m_weaponBoneNode2)
		m_weaponBoneNode2= m_weapon->GetBoneMgr()->FindBone("Dummy002_R");
}


bool cArchebladeCharacter::Move(const float elapseTime)
{
	cModel::Move(elapseTime);

	if (m_weapon)
		m_weapon->SetTransform(m_TM);

	if (m_weapon && m_weaponNode1 && m_weaponBoneNode1)
	{
		const Matrix44 mat = m_bone->GetPalette()[ m_weaponNode1->GetId()];
		m_weapon->GetBoneMgr()->GetPalette()[ m_weaponBoneNode1->GetId()] = mat;
		m_weaponBoneNode1->SetAccTM( m_weaponNode1->GetAccTM() );
	}

	if (m_weapon && m_weaponNode2 && m_weaponBoneNode2)
	{
		const Matrix44 mat = m_bone->GetPalette()[ m_weaponNode2->GetId()];
		m_weapon->GetBoneMgr()->GetPalette()[ m_weaponBoneNode2->GetId()] = mat;
		m_weaponBoneNode2->SetAccTM( m_weaponNode2->GetAccTM() );
	}

	if (m_weapon && m_weaponNode3 && m_weaponBoneNode3)
	{
		const Matrix44 mat = m_bone->GetPalette()[ m_weaponNode3->GetId()];
		m_weapon->GetBoneMgr()->GetPalette()[ m_weaponBoneNode3->GetId()] = mat;
		m_weaponBoneNode3->SetAccTM( m_weaponNode3->GetAccTM() );
	}


	return true;
}


void cArchebladeCharacter::Render(const Matrix44 &tm)
{
	cModel::Render(tm);

	if (m_weapon)
		m_weapon->Render(tm);
}


void cArchebladeCharacter::SetRenderWeaponBoundingBox(const bool isRenderBoundingBox)
{
	if (m_weapon)
		m_weapon->SetRenderBoundingBox(isRenderBoundingBox);
}
