#pragma once

namespace common
{

	struct Vector2
	{
		float x, y;

		Vector2() {}
		Vector2(float x0, float y0) : x(x0), y(y0) {}

		Vector2 operator + () const;
		Vector2 operator - () const;
		Vector2 operator + ( const Vector2& rhs ) const;
		Vector2 operator - ( const Vector2& rhs ) const;
		Vector2& operator += ( const Vector2& rhs );
		Vector2& operator -= ( const Vector2& rhs );
		Vector2& operator *= ( const Vector2& rhs );
		Vector2& operator /= ( const Vector2& rhs );

		template <class T>
		Vector2 operator * ( T t ) const {
			return Vector2(x*t, y*t);
		}

		template <class T>
		Vector2 operator / ( T t ) const {
			return Vector2(x/t, y/t);
		}

		template <class T>
		Vector2& operator *= ( T t ) {
			*this = Vector2(x*t, y*t);
			return *this;
		}

		template <class T>
		Vector2& operator /= ( T t ) {
			*this = Vector2(x/t, y/t);
			return *this;
		}

	};

}
