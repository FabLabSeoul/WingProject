
#include "stdafx.h"
#include "bonemgr.h"
#include "bonenode.h"

using namespace  graphic;


cBoneMgr::cBoneMgr(const int id, const sRawMeshGroup &rawMeshes) :
	m_root(NULL)
,	m_id(id)
,	m_deltaTime(0)
{
	m_palette.resize(rawMeshes.bones.size(), Matrix44());
	m_bones.resize(rawMeshes.bones.size(), NULL);
	
	sRawBone rootBone;
	rootBone.name = "root bone";
	m_root = new cBoneNode(-1, m_palette, rootBone);

	for (u_int i=0; i < rawMeshes.bones.size(); ++i)
	{
		const int id = rawMeshes.bones[ i].id;
		const int parentId = rawMeshes.bones[ i].parentId;

		if (m_bones[ id])
			continue; // already exist continue;

		cBoneNode *bone = new cBoneNode(id, m_palette, rawMeshes.bones[ i]);
		SAFE_DELETE(m_bones[ id]);
		m_bones[ id] = bone;

		if (-1 >=  parentId) // root
		{
			m_root->InsertChild( bone );
			bone->UpdateAccTM();
		}
		else
		{
			m_bones[ parentId]->InsertChild( bone );
			bone->UpdateAccTM();
		}
	}

	CreateBoundingBox(rawMeshes);

}

cBoneMgr::~cBoneMgr()
{
	Clear();
}


// 에니메이션 설정.
void cBoneMgr::SetAnimation( const sRawAniGroup &rawAnies, 
	const int nAniFrame,  // =0
	const bool isLoop, // = false
	const bool isBlend  //= true
	)
{
	m_deltaTime = 0;
	SetAnimationRec( m_root, rawAnies, nAniFrame, isLoop, isBlend );
}


// 애니메이션 설정.
void cBoneMgr::SetAnimationRec( cBoneNode *node, const sRawAniGroup &rawAnies, const int nAniFrame,
	const bool isLoop, const bool isBlend)
{
	RET(!node);
	RET(node->GetId() >= (int)rawAnies.anies.size());

	if (node->GetId() >= 0)
	{
		if (rawAnies.bones.empty())
		{
			node->SetAnimation( rawAnies.anies[ node->GetId()], nAniFrame, isLoop, false);
		}
		else
		{
			node->SetAnimation( rawAnies.bones[ node->GetId()], rawAnies.anies[ node->GetId()], 
				nAniFrame, isLoop, isBlend );
		}
	}

	BOOST_FOREACH (auto p, node->GetChildren())
	{
		SetAnimationRec((cBoneNode*)p, rawAnies, nAniFrame, isLoop, isBlend);
	}
}


void cBoneMgr::SetCurrentAnimationFrame(const int curFrame)
{
	BOOST_FOREACH (auto p, m_bones)
	{
		if (p)
			p->SetCurrentFrame(curFrame);
	}

	// 교정 된 프래임 위치로 애니메이션 시킨다.
	m_root->Move(0);
}


// 애니메이션
bool cBoneMgr::Move(const float elapseTime)
{
	RETV(!m_root, false);
	m_deltaTime += elapseTime;
	// 시간의 거의 지나가지 않았다면 애니메이션 계산을 하지 않는다.
	RETV(m_deltaTime < 0.02f, false);

	const bool result =m_root->Move(m_deltaTime);
	m_deltaTime = 0;
	return result;
}


// 스켈레톤 출력.
void cBoneMgr::Render(const Matrix44 &parentTm)
{
	RET(!m_root);
	m_root->Render(parentTm);
}

//void cBoneMgr::RenderShader(cShader &shader, const Matrix44 &parentTm)
//{
//	RET(!m_root);
//	m_root->RenderShader(shader, parentTm);
//}


// 경계박스 출력.
void cBoneMgr::RenderBoundingBox(const Matrix44 &parentTm)
{
	const Matrix44 identity;
	for (int i=0; i < (int)m_boundingBox.size(); ++i)
	{
		if (m_bones[ i])
			m_boundingBox[ i].SetTransform( m_bones[ i]->GetAccTM() * parentTm );
		m_boundingBox[ i].Render(identity);
	}
}


// BoneNode 찾아서 리턴.
cBoneNode* cBoneMgr::FindBone(const int id)
{
	RETV(!m_root, NULL);
	return (cBoneNode*)m_root->FindNode(id);
}


cBoneNode* cBoneMgr::FindBone(const string &name)
{
	RETV(!m_root, NULL);
	return (cBoneNode*)m_root->FindNode(name);
}


// 동적으로 할당된 객체 제거.
void cBoneMgr::Clear()
{
	SAFE_DELETE(m_root);
	m_bones.clear();
}


// 경계박스 생성.
void cBoneMgr::CreateBoundingBox(const sRawMeshGroup &rawMeshes)
{
	RET(!m_root);

	const int boneCount = rawMeshes.bones.size();
	vector<sMinMax> boundingBox(boneCount);
	vector<Matrix44> boneInvers(boneCount);
	for (int i=0; i < boneCount; ++i)
	{
		boneInvers[ i] = rawMeshes.bones[ i].worldTm.Inverse();
		//boundingBox[ i] = sMinMax();
	}


	// 특정 뼈대들은 경계박스를 하나로 합친다.
	map<int, int> boneIndices;
	SetBoundingBoxIndex(m_root, boneIndices);


	BOOST_FOREACH (const sRawMesh &mesh, rawMeshes.meshes)
	{
		BOOST_FOREACH (const sVertexWeight &weight, mesh.weights)
		{
			const int vtxIdx = weight.vtxIdx;

			for( int k=0; k < weight.size; ++k )
			{
				const sWeight *w = &weight.w[ k];
				const Vector3 pos = mesh.vertices[ vtxIdx] * boneInvers[ w->bone];

				const int boneIdx = boneIndices[ w->bone];

				boundingBox[ boneIdx].Update(pos);
			}
		}
	}


	m_boundingBox.resize(boneCount);
	for (int i=0; i < boneCount; ++i)
	{
		if (!boundingBox[ i].IsOk())
			continue;
		m_boundingBox[ i].SetCube(  boundingBox[ i]._min,  boundingBox[ i]._max );
	}
}


// 뼈대 인덱스를 설정한다.
void cBoneMgr::SetBoundingBoxIndex(cBoneNode *node, OUT map<int, int> &boneIndices, const int boneIdx) //boneIdx=-1
{
	RET(!node);

	int nextBoneIdx = -1;
	if (boneIdx == -1)
	{
		// 본 자신의 아이디를 저장한다.
		boneIndices[ node->GetId()] = node->GetId();

		// 특정 뼈대는 하나로 합친다.
		// 손목 아래 뼈대 통합.
		// 발목 아래 뼈대 통합.
		if ((node->GetName() ==  "Bip01 L Hand") 
			||  (node->GetName() ==  "Bip01 R Hand")
			|| (node->GetName() ==  "Bip01 L Foot")
			|| (node->GetName() ==  "Bip01 R Foot")
			)
		{
			nextBoneIdx = node->GetId();
		}
	}
	else
	{
		// 부모의 인덱스로 저장한다.
		boneIndices[ node->GetId()] = boneIdx;
		nextBoneIdx = boneIdx;
	}

	BOOST_FOREACH (auto &child, node->GetChildren())
		SetBoundingBoxIndex((cBoneNode*)child, boneIndices, nextBoneIdx);
}


// 애니메이션 옵션 설정.
void cBoneMgr::SetAnimationOption(DWORD option)
{
	BOOST_FOREACH (auto &bone, m_bones)
		bone->GetTrack()->SetAnimationOption(option);
}


// BoneNode 를 업데이트해서 palette 를 업데이트 한다.
// 애니메이션 하지 않고 행렬만 업데이트 된다.
void cBoneMgr::UpdatePalette()
{
	BOOST_FOREACH (auto &bone, m_bones)
		bone->UpdateAccTM();
}
