// 애니메이션 블렌딩을 관리하는 클래스다.
// 애니메이션 도중에 새 애니메이션을 플레이해야 할 때 
// 블렌딩 처리를 하게된다.
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
		sRawAni m_blendAni; // 새 에니메이션과 블렌딩을 위한 중간 애니메이션 정보.
		bool m_isBlend;	//애니메이션 블렌딩이 진행 중이라면 true 가 된다.

		cTrack *m_track;
		Matrix44 m_localTm;	// Track의 로컬행렬. 
											// 애니메이션 결과 행렬을 m_localTm 과 연산한 후 리턴한다.
		Matrix44 m_tempLocalTm; // 블렌딩이 끝난 후 전환 될 로컬행렬.

		int m_aniStart; // 프래임 시작시간 (프레임)
		int m_aniEnd; // 프래임 종료시간 (프레임)
		int m_totalPlayFrame; // 총 에니메이션 될 프레임

		int m_curPlayFrame; // 현재 에니메이션 프래임 (AniEnd를 지나면 0으로 초기화된다.)
		int m_incPlayFrame; // 에니메이션 중인 총 프레임
		float m_curPlayTime; // 현재 애니메이션 시간 (m_aniEnd 에 도달하면 0 이 된다.)
		float m_incPlayTime; // 현재 애니메이션 시간 (총 시간)

		bool m_isAni; // TRUE일경우만 에니메이션이 된다.
		bool m_isLoop; // 에니메이션 반복 여부
		DWORD m_option; // 0x01 = 애니메이션에서 이동 정보는 무시한다.
	};


	inline int cBlendTrack::GetCurrentFrame() const { return m_curPlayFrame; }
	inline bool cBlendTrack::IsAnimation() const { return m_isAni; }
	inline void cBlendTrack::SetAnimationOption(DWORD option) { m_option = option; }
	inline void cBlendTrack::UpdateLocalTM(const Matrix44 &tm) { m_localTm = tm; m_tempLocalTm = tm; }
}
