// 3���� �������� ��� ������Ʈ
// 
// PhysX ������ �����Ѵ�.
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
