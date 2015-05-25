
#include "stdafx.h"
#include "maincamera.h"

using namespace graphic;


cMainCamera::cMainCamera()
{
	SetCamera(Vector3(100,100,-500), Vector3(0,0,0), Vector3(0,1,0));
	SetProjection( D3DX_PI / 4.f, 1024.f/768.f, 1.f, 10000.0f);

}

cMainCamera::~cMainCamera()
{

}

