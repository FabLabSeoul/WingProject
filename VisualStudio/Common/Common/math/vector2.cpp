
#include "stdafx.h"
#include "vector2.h"

using namespace common;


Vector2 Vector2::operator + () const
{
	return *this;
}


Vector2 Vector2::operator - () const
{
	return Vector2(-x, -y);
}


Vector2 Vector2::operator + ( const Vector2& rhs ) const
{
	return Vector2(x+rhs.x, y+rhs.y);
}


Vector2 Vector2::operator - ( const Vector2& rhs ) const
{
	return Vector2(x-rhs.x, y-rhs.y);
}


Vector2& Vector2::operator += ( const Vector2& rhs )
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}


Vector2& Vector2::operator -= ( const Vector2& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}


Vector2& Vector2::operator *= ( const Vector2& rhs )
{
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}


Vector2& Vector2::operator /= ( const Vector2& rhs )
{
	x /= rhs.x;
	y /= rhs.y;
	return *this;
}
