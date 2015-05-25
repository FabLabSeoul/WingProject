#pragma once


namespace graphic
{

	struct sKeyPos
	{
		float t; // frame ����
		Vector3 p;
	};


	struct sKeyScale
	{
		float t; // frame ����
		Vector3 s;
	};


	struct sKeyRot
	{
		float t; // frame ����
		Quaternion q;
	};


	struct sRawAni
	{
		string name;
		float start; // frame ����
		float end; // frame ����
		vector<sKeyPos> pos;
		vector<sKeyRot> rot;
		vector<sKeyScale> scale;
	};


	// �𵨿��� �ϳ��� �ִϸ��̼� ������ �����ϴ� �ڷᱸ��.
	struct sRawAniGroup
	{
		enum ANITYPE { MESH_ANI, BONE_ANI };

		ANITYPE type;
		string name;		
		vector<sRawAni> anies;
		vector<sRawBone> bones;
	};

}
