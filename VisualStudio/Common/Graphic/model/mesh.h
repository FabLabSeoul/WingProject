// 메쉬 기본 클래스
//
// cMesh 확장
// cMesh -> cRigidMesh
//			  -> cSkinnedMesh
// 
#pragma once


#include "node.h"

namespace graphic
{

	class cMesh : public cNode
	{
	public:
		cMesh(const int id, const sRawMesh &rawMesh);
		cMesh(const int id, const sRawBone &rawBone);
		virtual ~cMesh();

		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;
		virtual void RenderShadow(const Matrix44 &viewProj, 
			const Vector3 &lightPos, const Vector3 &lightDir, const Matrix44 &parentTm) override;

		const cBoundingBox* GetBoundingBox();
		cMeshBuffer* GetMeshBuffer();
		void SetMeshBuffer(cMeshBuffer *buffer);
		

	protected:
		void CreateMaterials(const sRawMesh &rawMesh);

		virtual void RenderShader( cShader &shader, const Matrix44 &parentTm );
		virtual void RenderShadow(cShader &shader, const Matrix44 &parentTm);


	protected:
		bool m_isSkinned;
		vector<cMaterial> m_mtrls;
		vector<cTexture*>m_colorMap;  // reference
		vector<cTexture*>m_normalMap;  // reference
		vector<cTexture*>m_specularMap;  // reference
		vector<cTexture*>m_selfIllumMap;  // reference
		cMeshBuffer *m_buffers; // reference (뼈대 메쉬일 경우 예외)

		// debug 용.
		bool m_isBoneMesh;
	};


	inline cMeshBuffer* cMesh::GetMeshBuffer() { return m_buffers; }
	inline void cMesh::SetMeshBuffer(cMeshBuffer *buffer) { m_buffers = buffer; }
}
