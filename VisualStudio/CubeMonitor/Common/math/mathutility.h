#pragma once


namespace common
{

	//-----------------------------------------------------------------------------
	// Name: getRandomMinMax()
	// Desc: Gets a random number between min/max boundaries
	// http://www.codesampler.com/dx9src/dx9src_7.htm
	//-----------------------------------------------------------------------------
	inline float GetRandomMinMax( float fMin, float fMax )
	{
		float fRandNum = (float)rand () / RAND_MAX;
		return fMin + (fMax - fMin) * fRandNum;
	}

	//-----------------------------------------------------------------------------
	// Name: getRandomVector()
	// Desc: Generates a random vector where X,Y, and Z components are between
	//       -1.0 and 1.0
	// http://www.codesampler.com/dx9src/dx9src_7.htm
	//-----------------------------------------------------------------------------
	inline Vector3 GetRandomVector( void )
	{
		Vector3 vVector;

		// Pick a random Z between -1.0f and 1.0f.
		vVector.z = GetRandomMinMax( -1.0f, 1.0f );

		// Get radius of this circle
		float radius = (float)sqrt(1 - vVector.z * vVector.z);

		// Pick a random point on a circle.
		float t = GetRandomMinMax( -MATH_PI, MATH_PI);

		// Compute matching X and Y for our Z.
		vVector.x = (float)cosf(t) * radius;
		vVector.y = (float)sinf(t) * radius;

		return vVector;
	}


}
