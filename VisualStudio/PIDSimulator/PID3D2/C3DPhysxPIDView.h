// 3차원 물리엔진 출력 오브젝트
// 
// PhysX 정보를 관리한다.
// 
//
#pragma once


#include "C3DPhysxView.h"
#include "Machine.h"

// C3DDialog dialog
class C3DPhysxPIDView : public C3DPhysxView
{
public:
	C3DPhysxPIDView(CWnd* pParent = NULL);   // standard constructor
	virtual ~C3DPhysxPIDView();
	// Dialog Data
	enum { IDD = IDD_DIALOG_3D_2 };

	virtual void UpdateBefore(const float deltaSeconds) override;
	virtual void RenderChild() override;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	cMachine m_machine;	
};
