// 빌보드 클래스.
// 해골 책 빌보드 아티클을 참조함.
#pragma once


namespace graphic
{

	// 빌보드 형태 값.
	namespace BILLBOARD_TYPE {
		enum TYPE
		{
			Y_AXIS,		// Y 축으로만 회전한다.
			ALL_AXIS,	// 모든 축이 회전한다.
		};
	};


	class cBillboard : public cQuad
	{
	public:		
		cBillboard();
		virtual ~cBillboard();

		bool Create( const BILLBOARD_TYPE::TYPE type, const float width, const float height, 
			const Vector3 &pos, const string &textureFileName="" );
		void Render();


	private:
		BILLBOARD_TYPE::TYPE m_type;
	};

}
