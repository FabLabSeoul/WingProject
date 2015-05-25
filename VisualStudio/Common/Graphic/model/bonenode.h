#pragma once


namespace graphic
{

	class cBoneNode : public cNode
	{
	public:
		cBoneNode(const int id, vector<Matrix44> &palette, const sRawBone &rawMesh);
		virtual ~cBoneNode();

		void SetAnimation( const sRawAni &rawAni, const int totalAniFrame, 
			const bool isLoop=false, const bool isBlend=true );

		void SetAnimation( const sRawBone &rawBone, const sRawAni &rawAni, 
			const int totalAniFrame, const bool isLoop=false, const bool isBlend=true );

		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;

		void SetAccTM(const Matrix44 &mat);
		const Matrix44& GetAccTM() const;
		const Matrix44& GetOffset() const;
		Matrix44 GetCalculateAniTM();
		void SetOffset(const Matrix44 &mat);
		void UpdateLocalTM(const Matrix44 &tm);

		int GetCurrentFrame() const;
		void SetCurrentFrame(const int curFrame);
		void UpdateAccTM();
		cMesh* GetMesh();
		cBlendTrack* GetTrack();


	private:
		vector<Matrix44> &m_palette; // 원본은 cBoneMgr에 있음.
		Matrix44 m_accTM;	// 누적된 TM
		Matrix44 m_offset;	// inverse( m_matWorld )
		cBlendTrack *m_track;

		// debug 용.
		cMesh *m_mesh;
	};


	inline void cBoneNode::SetAccTM(const Matrix44& mat) { m_accTM = mat; }
	inline const Matrix44& cBoneNode::GetAccTM() const { return m_accTM; }
	inline const Matrix44& cBoneNode::GetOffset() const { return m_offset; }
	inline void cBoneNode::SetOffset(const Matrix44 &mat) { m_offset = mat; }
	inline cMesh* cBoneNode::GetMesh() { return m_mesh; }
	inline cBlendTrack* cBoneNode::GetTrack() { return m_track; }
}
