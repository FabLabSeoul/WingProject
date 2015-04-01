// �ִϸ��̼� ������ �����ϴ� Ŭ������.
// �ִϸ��̼� ���߿� �� �ִϸ��̼��� �÷����ؾ� �� �� 
// ���� ó���� �ϰԵȴ�.
#pragma once


namespace graphic
{

	class cBlendTrack
	{
	public:
		cBlendTrack();
		virtual ~cBlendTrack();

		void PlayAnimation( const Matrix44 &localTm, const sRawAni &rawAni, const int totalAniFrame, 
			const bool isLoop=false, const bool isBlend=true );

		bool Move( const float elapseTime, const Matrix44 &tm, 
			OUT Matrix44 &aniOut, OUT Matrix44 &accOut );

		void SetCurrentFrame( const int curFrame, const Matrix44 &tm, 
			OUT Matrix44 &aniOut, OUT Matrix44 &accOut  );
		int GetCurrentFrame() const;
		bool IsAnimation() const;
		void SetAnimationOption(DWORD option);
		void UpdateLocalTM(const Matrix44 &tm);

		void GetCalculateTM(const Matrix44 &localTm, 
			const Matrix44 &aniTm, const Matrix44 &tm, OUT Matrix44 &out) const;


	protected:
		bool CheckAnimationFrame( const float elapseTime );
		void InitBlend();
		bool MakeBlendAni( const int curFrame, const Matrix44 &nextLocalTm, 
			const sRawAni &nextRawAni, const int blendTime, sRawAni &out );


	private:
		const sRawAni *m_curAni;
		const sRawAni *m_nextAni;
		sRawAni m_blendAni; // �� ���ϸ��̼ǰ� ������ ���� �߰� �ִϸ��̼� ����.
		bool m_isBlend;	//�ִϸ��̼� ������ ���� ���̶�� true �� �ȴ�.

		cTrack *m_track;
		Matrix44 m_localTm;	// Track�� �������. 
											// �ִϸ��̼� ��� ����� m_localTm �� ������ �� �����Ѵ�.
		Matrix44 m_tempLocalTm; // ������ ���� �� ��ȯ �� �������.

		int m_aniStart; // ������ ���۽ð� (������)
		int m_aniEnd; // ������ ����ð� (������)
		int m_totalPlayFrame; // �� ���ϸ��̼� �� ������

		int m_curPlayFrame; // ���� ���ϸ��̼� ������ (AniEnd�� ������ 0���� �ʱ�ȭ�ȴ�.)
		int m_incPlayFrame; // ���ϸ��̼� ���� �� ������
		float m_curPlayTime; // ���� �ִϸ��̼� �ð� (m_aniEnd �� �����ϸ� 0 �� �ȴ�.)
		float m_incPlayTime; // ���� �ִϸ��̼� �ð� (�� �ð�)

		bool m_isAni; // TRUE�ϰ�츸 ���ϸ��̼��� �ȴ�.
		bool m_isLoop; // ���ϸ��̼� �ݺ� ����
		DWORD m_option; // 0x01 = �ִϸ��̼ǿ��� �̵� ������ �����Ѵ�.
	};


	inline int cBlendTrack::GetCurrentFrame() const { return m_curPlayFrame; }
	inline bool cBlendTrack::IsAnimation() const { return m_isAni; }
	inline void cBlendTrack::SetAnimationOption(DWORD option) { m_option = option; }
	inline void cBlendTrack::UpdateLocalTM(const Matrix44 &tm) { m_localTm = tm; m_tempLocalTm = tm; }
}
