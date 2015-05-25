
#include "stdafx.h"
#include "C3DPhysxPIDView.h"


C3DPhysxPIDView *g_physxView;


BEGIN_MESSAGE_MAP(C3DPhysxPIDView, C3DPhysxView)
END_MESSAGE_MAP()


C3DPhysxPIDView::C3DPhysxPIDView(CWnd* pParent)
{
	g_physxView = this; // 전역변수 업데이트
}

C3DPhysxPIDView::~C3DPhysxPIDView()
{

}


BOOL C3DPhysxPIDView::OnInitDialog()
{
	if (!C3DPhysxView::OnInitDialog())
		return FALSE;

	// 카메라 이동
	graphic::GetMainCamera()->SetEyePos(Vector3(1.2f, 2.f, -1.f));
	SetFixedFrame(false);

	m_machine.Init();


	return TRUE;
}


void C3DPhysxPIDView::UpdateBefore(const float deltaSeconds)
{
	m_machine.UpdatePhysX(deltaSeconds);
	C3DPhysxView::UpdateBefore(deltaSeconds);
	m_machine.UpdateAfterPhysX(deltaSeconds);

}


void C3DPhysxPIDView::RenderChild()
{
	m_machine.m_box.cube.Render(Matrix44::Identity);

	m_machine.m_motor[0]->m_cube.Render(m_machine.m_box.cube.GetTransform());
	m_machine.m_motor[0]->m_line.Render();

	m_machine.m_motor[1]->m_cube.Render(m_machine.m_box.cube.GetTransform());
	m_machine.m_motor[1]->m_line.Render();
}
