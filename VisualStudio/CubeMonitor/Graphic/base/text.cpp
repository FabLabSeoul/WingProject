
#include "stdafx.h"
#include "text.h"


using namespace graphic;


cText::cText()
	: m_font(NULL)
,	m_color(D3DXCOLOR(1,1,1,1))
,	m_rect(0,0,100,100)
{

}

cText::cText(const string &text, const int x, const int y, 
	const DWORD color, const int fontSize, const bool isBold, const string &fontName)
	// fontSize=18, isBold=true, fontName=����
	: m_text(text)
,	m_font(NULL)
,	m_rect(x, y, 100, 100)
,	m_color(color)
{
	Create(fontSize, isBold, fontName);
}


cText::~cText()
{
	Clear();
}


// �ؽ�Ʈ ����.
bool cText::Create(const int fontSize, const bool isBold, const string &fontName)
	// fontSize=18, isBold=true, fontName=����
{
	Clear();

	const HRESULT hr = D3DXCreateFontA(GetDevice(), fontSize, 0, 
		isBold?  FW_BOLD : FW_NORMAL, 1, FALSE, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(), 
		&m_font );

	if (FAILED(hr))
		return false;

	return true;
}


// �ؽ�Ʈ ���.
void cText::Render()
{
	RET(!m_font);

	m_font->DrawTextA( NULL, m_text.c_str(), -1, &m_rect, DT_NOCLIP, m_color);
}


void cText::Clear()
{
	SAFE_RELEASE(m_font);
}
