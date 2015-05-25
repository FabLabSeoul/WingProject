#pragma once


namespace graphic
{
	class cBoneNode;

	class cBoneMgr
	{
	public:
		cBoneMgr(const int id, const sRawMeshGroup &rawMeshes);
		virtual ~cBoneMgr();

		void SetAnimation( const sRawAniGroup &rawAnies, const int nAniFrame=0,  
			const bool isLoop=false, const bool isBlend=true);

		bool Move(const float elapseTime);
		void Render(const Matrix44 &parentTm);
		//void RenderShader(cShader &shader, const Matrix44 &parentTm);

		void UpdatePalette();
		void RenderBoundingBox(const Matrix44 &parentTm);
		void Clear();
		cBoneNode* GetRoot();
		vector<cBoneNode*>& GetAllBoneNode();
		cBoneNode* FindBone(const int id);
		cBoneNode* FindBone(const string &name);
		vector<Matrix44>& GetPalette();
		vector<cCube>& GetBoundingBoxes();
		void SetAnimationOption(DWORD option);
		void SetCurrentAnimationFrame(const int curFrame);


	protected:
		void SetAnimationRec( cBoneNode *node, const sRawAniGroup &rawAni, const int nAniFrame,
			const bool isLoop, const bool isBlend);
		void CreateBoundingBox(const sRawMeshGroup &rawMeshes);
		void SetBoundingBoxIndex(cBoneNode *node, OUT map<int, int> &boneIndices, const int boneIdx=-1);


	private:
		int m_id;
		cBoneNode *m_root;
		vector<cBoneNode*> m_bones;
		vector<Matrix44> m_palette;
		vector<cCube> m_boundingBox;

		float m_deltaTime;
	};


	inline cBoneNode* cBoneMgr::GetRoot() { return m_root; }
	inline vector<Matrix44>& cBoneMgr::GetPalette() { return m_palette; }
	inline vector<cBoneNode*>& cBoneMgr::GetAllBoneNode() { return m_bones; }
	inline vector<cCube>& cBoneMgr::GetBoundingBoxes() { return m_boundingBox; }
}
