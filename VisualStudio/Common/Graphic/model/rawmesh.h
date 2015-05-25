#pragma once


namespace graphic
{

	struct sMaterial
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector4 emissive;
		float power;
		string directoryPath; // 텍스쳐가 저장된 디렉토리 경로를 저장한다.
		string texture;
		string bumpMap;
		string specularMap;
		string selfIllumMap;

		sMaterial() {}
		sMaterial(const sMaterial &rhs);
		sMaterial& operator=(const sMaterial &rhs);
	};


	// 속성버퍼 정보를 표현함.
	struct sAttribute
	{
		int attribId;
		int faceStart;
		int faceCount;
		int vertexStart;
		int vertexCount;
	};


	struct sWeight
	{
		int bone;
		float weight;
	};


	struct sVertexWeight
	{
		int vtxIdx;
		int size;
		sWeight w[6];
	};

	
	struct sRawBone
	{
		int id;
		int parentId;
		string name;
		Matrix44 localTm;
		Matrix44 worldTm;

		// debug 용.
		vector<Vector3> vertices;
		vector<Vector3> normals; // vertex 갯수만큼 저장된다.
		vector<Vector3> tex;
		vector<int> indices;
	};


	struct sRawMesh
	{
		string name;
		vector<Vector3> vertices;
		vector<Vector3> normals; // vertex 갯수만큼 저장된다.
		vector<Vector3> tangent; // vertex 갯수만큼 저장된다.
		vector<Vector3> binormal; // vertex 갯수만큼 저장된다.
		vector<Vector3> tex;
		vector<int> indices;
		vector<sAttribute> attributes;
		vector<sVertexWeight> weights;
		sMaterial mtrl;
		int mtrlId;
		vector<sMaterial> mtrls;
		vector<int> mtrlIds;
	};


	// 모델하나의 정보를 저장하는 자료구조.
	struct sRawMeshGroup
	{
		string name;
		vector<sMaterial> mtrls;
		vector<sRawMesh> meshes;
		vector<sRawBone> bones;
	};

}
