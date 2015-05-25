#pragma once


namespace graphic
{
	class cBoneMgr;
	class cBoundingBox;

	class cModel : public cNode 
						, public ICollisionable
	{
	public:
		cModel(const int id);
		virtual ~cModel();

		virtual bool Create(const string &modelName, MODEL_TYPE::TYPE type = MODEL_TYPE::AUTO);
		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &tm) override;
		virtual void RenderShadow(const Matrix44 &viewProj, 
			const Vector3 &lightPos, const Vector3 &lightDir, const Matrix44 &parentTm) override;

		virtual void Clear() override;

		// Model Infomations
		MODEL_TYPE::TYPE GetModelType() const;
		const string& GetFileName() const;
		cMesh* GetMesh(const string &name);
		cMesh* GetMesh(const int idx);
		const sTransform& GetToolTransform() const;
		void SetToolTransform(const sTransform& tm);

		bool Pick(const Vector3 &orig, const Vector3 &dir);
		cModel* Clone() const;

		// shadow
		void UpdateShadow();
		cShadow1& GetShadow();
		const cShadow1& GetShadow() const;
		bool IsRenderShadow() const;
		void SetRenderShadow(const bool show);

		// Animation
		virtual bool SetAnimation(const string &aniFileNameconst, const int nAniFrame=0,  
			const bool isLoop=false, const bool isBlend=true);
		cBoneMgr* GetBoneMgr();
		cMesh* FindMesh(const string &meshName);
		sRawAniGroup* GetCurrentAnimation();
		bool SharePalette(vector<Matrix44> *palette);
		virtual void SetShader(cShader *shader) override;

		// ICollisionable Interface
		virtual bool IsTest( int testNum ) override;
		virtual int GetCollisionId() override;
		virtual void UpdateCollisionBox() override;
		virtual cBoundingBox* GetCollisionBox() override;
		virtual void Collision( int testNum, ICollisionable *obj ) override;

		// debug 용 함수.
		void SetRenderMesh(const bool isRender);
		void SetRenderBone(const bool isRender);
		void SetRenderBoundingBox(const bool isRender);
		void SetRenderBoneBoundingBox(const bool isRender);
		bool IsRenderMesh() const;
		bool IsRenderBone() const;
		bool IsRenderBoundingBox() const;
		bool IsRenderBoneBoundingBox() const;


	protected:
		string m_fileName; //model 파일 명.
		MODEL_TYPE::TYPE m_type;
		vector<cMesh*> m_meshes;
		cBoneMgr *m_bone;
		cBoundingBox m_boundingBox; // only rigid mesh model
		cCube m_renderBoundingBox; // only display bounding box
		sRawAniGroup *m_curAni; // reference, 애니메이션 정보.
		cShadow1 m_shadow;
		bool m_isRenderShadow; // default = false
		sTransform m_transform;
		
		// debug 용.
		bool m_isRenderMesh; // default = true
		bool m_isRenderBone; // default = false
		bool m_isRenderBoundingBox; // default = false
		bool m_isRenderBoneBoundingBox; // default = false
	};


	inline MODEL_TYPE::TYPE cModel::GetModelType() const { return m_type; }
	inline cBoneMgr* cModel::GetBoneMgr() { return m_bone; }
	inline sRawAniGroup* cModel::GetCurrentAnimation() { return m_curAni; }
	inline const string& cModel::GetFileName() const { return m_fileName; }
	inline const sTransform& cModel::GetToolTransform() const { return m_transform; }

	inline cShadow1& cModel::GetShadow() { return m_shadow; }
	inline const cShadow1& cModel::GetShadow() const { return m_shadow; }
	inline bool cModel::IsRenderShadow() const { return m_isRenderShadow; }
	inline void cModel::SetRenderShadow(const bool show) { m_isRenderShadow = show; }

	inline void cModel::SetRenderMesh(const bool isRender) { m_isRenderMesh = isRender; }
	inline void cModel::SetRenderBone(const bool isRender) { m_isRenderBone = isRender; }
	inline void cModel::SetRenderBoundingBox(const bool isRender) { m_isRenderBoundingBox = isRender; }
	inline void cModel::SetRenderBoneBoundingBox(const bool isRender) { m_isRenderBoneBoundingBox = isRender; }
	inline bool cModel::IsRenderMesh() const { return m_isRenderMesh; }
	inline bool cModel::IsRenderBone() const { return m_isRenderBone; }
	inline bool cModel::IsRenderBoundingBox() const { return m_isRenderBoundingBox; }
	inline bool cModel::IsRenderBoneBoundingBox() const { return m_isRenderBoneBoundingBox; }
}
