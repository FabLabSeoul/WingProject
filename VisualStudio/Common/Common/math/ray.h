#pragma once


namespace common
{

	struct Ray
	{
		Ray();
		Ray(const int x, const int y, const int screenWidth, const int screenHeight,  
			const Matrix44 &matProj, const Matrix44 &matView);
		void Create( const int x, const int y, const int screenWidth, const int screenHeight,  
			const Matrix44 &matProj, const Matrix44 &matView );

		Vector3 orig;
		Vector3 dir;
	};


}
