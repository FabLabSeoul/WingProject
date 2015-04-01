#pragma once


// C3DView view

class C3DView : public CView
{
public:
	C3DView();           // protected constructor used by dynamic creation
	virtual ~C3DView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


