// 3차원 물리엔진 출력 오브젝트
//
//
#pragma once


#include "C3DView.h"
#include "DockablePaneChildView.h"
#include "PxPhysics.h"
#include "foundation/PxFoundation.h"
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"

using namespace physx;


struct sPhysxBox
{
	physx::PxRigidDynamic *rigid; // 물리 연산 오브젝트 
	graphic::cCube2 cube; // 3D 오브젝트
};



// C3DDialog dialog
class C3DPhysxView : public C3DView
{
public:
	C3DPhysxView(CWnd* pParent = NULL);   // standard constructor
	virtual ~C3DPhysxView();
	enum { IDD = IDD_DIALOG_3D_2 };

	virtual void UpdateBefore(const float deltaSeconds) override;
	virtual void UpdateAfter(const float deltaSeconds) override;
	virtual void Render() override;

	physx::PxPhysics* GetPhysx();

	physx::PxRigidActor* CreateGrid();
	physx::PxRigidDynamic* CreateBox(const physx::PxVec3& pos, const PxVec3& dims, const PxVec3* linVel,
		PxReal density);


protected:
	bool InitPhysX();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);



protected:
	// PhysX
	physx::PxFoundation *m_Foundation;
	physx::PxPhysics *m_physics;
	physx::PxDefaultAllocator m_pxAllocator;
	physx::PxDefaultCpuDispatcher *m_CpuDispatcher;
	physx::PxScene *m_Scene;
	physx::PxMaterial *m_Material;

	static const PxU32 SCRATCH_BLOCK_SIZE = 1024 * 128;
	void *m_ScratchBlock;

	vector<sPhysxBox*> m_cubes;
};


inline physx::PxPhysics* C3DPhysxView::GetPhysx() { return m_physics;  }
