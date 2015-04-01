
#include "stdafx.h"
#include "rect.h"

using namespace common;


sRect::sRect()
{
}

sRect::sRect(int left0, int top0, int right0, int bottom0)
{
	left = left0;
	top = top0;
	right = right0;
	bottom = bottom0;
}

int sRect::Width() const
{
	return abs(right-left);
}

int sRect::Height() const
{
	return abs(bottom-top);
}

sRect sRect::Rect(int x, int y, int width, int height)
{
	return sRect(x, y, x+width, y+height);
}

void sRect::SetX(const int x)
{
	*this = sRect(x, top, x+Width(), bottom);
}

void sRect::SetY(const int y)
{
	*this = sRect(left, y, right, y+Height());
}

void sRect::SetWidth(const int width)
{
	*this = Rect(left, top, width, Height());
}

void sRect::SetHeight(const int height)
{
	*this = Rect(left, top, right, height);
}
