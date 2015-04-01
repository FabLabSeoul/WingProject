#pragma once


namespace graphic
{

	class cSprite : public cNode
	{
	public:
		cSprite(LPD3DXSPRITE sprite, const int id, const string &name="sprite");
		virtual ~cSprite();

		void SetTexture(const string &fileName);
		virtual void Render(const Matrix44 &parentTm) override;
		void SetCenter(const Vector3 &center); // x,y = 0~1
		void SetPos(const Vector3 &pos);
		const Vector3& GetPos() const;
		void SetScale(const Vector3 &scale);
		const Vector3& GetScale() const;
		void SetRect(const sRect &rect);
		const sRect& GetRect() const;

		bool IsContain(const Vector2 &pos);		

		void Clear();


	private:
		LPD3DXSPRITE m_sprite; // reference
		cTexture *m_texture; // reference
		Vector3 m_center;	// x,y = 0~1, default : 0,0
		Vector3 m_pos; // pixel coordinate
		Vector3 m_scale;
		sRect m_rect; // pixel coordinate
		DWORD m_color;
		Matrix44 m_accTM;
	};


	inline void cSprite::SetPos(const Vector3 &pos) { m_pos = pos; } 
	inline const Vector3& cSprite::GetPos() const { return m_pos; }
	inline void cSprite::SetScale(const Vector3 &scale) { m_scale = scale; }
	inline const Vector3& cSprite::GetScale() const { return m_scale; }
	inline void cSprite::SetRect(const sRect &rect) { m_rect = rect; }
	inline const sRect& cSprite::GetRect() const { return m_rect; }
}
