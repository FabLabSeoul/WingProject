
// PID3D2View.h : interface of the CPID3D2View class
//

#pragma once


class CPID3D2View : public CView
{
protected: // create from serialization only
	CPID3D2View();
	DECLARE_DYNCREATE(CPID3D2View)

// Attributes
public:
	CPID3D2Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPID3D2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PID3D2View.cpp
inline CPID3D2Doc* CPID3D2View::GetDocument() const
   { return reinterpret_cast<CPID3D2Doc*>(m_pDocument); }
#endif

