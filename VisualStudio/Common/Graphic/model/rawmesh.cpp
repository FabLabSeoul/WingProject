#include "stdafx.h"
#include "rawmesh.h"

using namespace graphic;


sMaterial::sMaterial(const sMaterial &rhs)
{
	operator=(rhs);
}


sMaterial& sMaterial::operator=(const sMaterial &rhs)
{
	if (this != &rhs)
	{
		ambient = rhs.ambient;
		diffuse = rhs.diffuse;
		specular = rhs.specular;
		emissive = rhs.emissive;
		power = rhs.power;
		directoryPath = rhs.directoryPath;
		texture = rhs.texture;
		bumpMap = rhs.bumpMap;
		specularMap = rhs.specularMap;
		selfIllumMap = rhs.selfIllumMap;
	}
	return *this;
}

