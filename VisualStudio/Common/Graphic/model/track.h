#pragma once


namespace graphic
{

	class cTrack 
	{
	public:
		cTrack(const sRawAni *rawAni);
		virtual ~cTrack();

		bool Load( const sRawAni *rawAni );

		void InitAnimation( const int curFrame = 0 );
		void Move( const int curFrame, OUT Matrix44 &out );
		int GetCurrentFrame() const;
		void SetCurrentFramePos( const int curFrame );

		bool GetPosKey( const int curFrame, OUT Vector3 &out );
		bool GetRotKey( const int curFrame, OUT Quaternion &out );
		bool GetScaleKey( const int curFrame, OUT Vector3 &out );


	protected:
		inline float GetAlpha( float f1, float f2, float frame ) const;


	private:
		const sRawAni *m_rawAni; // reference
		int m_curFrame;

		// ���ϸ��̼��� ���� Key index�� ��Ÿ����.
		int m_keyPosIdx;
		int m_keyRotIdx;
		int m_keyScaleIdx;

		// ���ϸ��̼ǿ��� ���� Key�� ���� Key�� �����Ѵ�.
		sKeyPos *m_curKeyPos[ 2];
		sKeyRot *m_curKeyRot[ 2];
		sKeyScale *m_curKeyScale[ 2];
	};


	inline int cTrack::GetCurrentFrame() const { return m_curFrame; }
}
