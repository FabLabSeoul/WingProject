// ������ Ŭ����.
// �ذ� å ������ ��ƼŬ�� ������.
#pragma once


namespace graphic
{

	// ������ ���� ��.
	namespace BILLBOARD_TYPE {
		enum TYPE
		{
			Y_AXIS,		// Y �����θ� ȸ���Ѵ�.
			ALL_AXIS,	// ��� ���� ȸ���Ѵ�.
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
