#pragma once


// C3DDialog dialog

class C3DDialog : public CDialogEx
{
public:
	C3DDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~C3DDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_3D };

	void Update(const float deltaSeconds);
	void Render();


protected:
	bool m_isInitDx = false;
	graphic::Box m_box;
	graphic::cSphere m_sphere;
	graphic::cCube m_cube;

	Matrix44 m_cubeTm;
	Matrix44 m_rotateTm;
	bool m_LButtonDown = false;
	bool m_RButtonDown = false;
	bool m_MButtonDown = false;
	CPoint m_curPos;


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
};
