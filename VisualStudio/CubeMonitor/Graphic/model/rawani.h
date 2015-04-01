#pragma once


namespace graphic
{

	struct sKeyPos
	{
		float t; // frame 단위
		Vector3 p;
	};


	struct sKeyScale
	{
		float t; // frame 단위
		Vector3 s;
	};


	struct sKeyRot
	{
		float t; // frame 단위
		Quaternion q;
	};


	struct sRawAni
	{
		string name;
		float start; // frame 단위
		float end; // frame 단위
		vector<sKeyPos> pos;
		vector<sKeyRot> rot;
		vector<sKeyScale> scale;
	};


	// 모델에서 하나의 애니메이션 정보를 저장하는 자료구조.
	struct sRawAniGroup
	{
		enum ANITYPE { MESH_ANI, BONE_ANI };

		ANITYPE type;
		string name;		
		vector<sRawAni> anies;
		vector<sRawBone> bones;
	};

}
