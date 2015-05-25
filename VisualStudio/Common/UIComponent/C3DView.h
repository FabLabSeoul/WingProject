#pragma once

//#include "foundation/PxFoundation.h"
//#include "PxPhysicsAPI.h"
//#include "extensions/PxExtensionsAPI.h"
//using namespace physx;


#define IDD_DIALOG_3D_2	123456


// C3DDialog dialog
class C3DView : public CDockablePaneChildView
{
public:
	C3DView(CWnd* pParent = NULL);   // standard constructor
	virtual ~C3DView();

// Dialog Data
	enum { IDD = IDD_DIALOG_3D_2 };

	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateBefore(const float deltaSeconds) {}
	virtual void UpdateAfter(const float deltaSeconds) {}
	virtual void Render();
	virtual void RenderChild() {}

	void SetFixedFrame(const bool isFixedFrame);


protected:
	bool m_isInitDx = false;
	bool m_isFixedFrame = true; // 고정 프레임모드시 true, (100 frame)
	graphic::Box m_box;
	graphic::cSphere m_sphere;
	graphic::cCube m_cube;

	Matrix44 m_cubeTm;
	Matrix44 m_rotateTm;
	bool m_LButtonDown = false;
	bool m_RButtonDown = false;
	bool m_MButtonDown = false;
	CPoint m_curPos;

	float m_IncSeconds = 0;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


inline void C3DView::SetFixedFrame(const bool isFixedFrame) { m_isFixedFrame = isFixedFrame;  }
