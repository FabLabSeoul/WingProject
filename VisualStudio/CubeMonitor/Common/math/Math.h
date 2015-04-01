#pragma once


const float MATH_PI = 3.141592654f;
const float MATH_EPSILON = 0.0005f;//1.0e-5f;
#define SQR(x)		( (x) * (x) )
#define LIMIT_RANGE(low, value, high)	{	if (value < low)	value = low;	else if(value > high)	value = high;	}
#define FLOAT_EQ(x,v)	(fabs((x)-(v)) < (MATH_EPSILON))
#define ABS( x )	( (x) >= 0 ? (x) : -(x) )
#define ANGLE2RAD(angle) ((MATH_PI*(angle)) * (0.00555555f))
#define RAD2ANGLE(radian) ((radian) * (180.0f/MATH_PI))


#include <windows.h>
#include <math.h>
#include <float.h>
#include <vector>
using std::vector;

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix44.h"
#include "quaternion.h"
#include "triangle.h"
#include "plane.h"
#include "ray.h"

#include "mathutility.h"
