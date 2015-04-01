 // 테라 게임 캐릭터 클래스.
//
#pragma once

#include "character.h"


namespace graphic
{

	namespace TERA_MODEL
	{
		enum TYPE
		{
			FACE, 
			HAIR,
			BODY,
			HAND,
			LEG,
			TAIL,
			MAX_TYPE
		};
	}


	class cTeraCharacter : public cCharacter
	{
	public:
		cTeraCharacter();
		virtual ~cTeraCharacter();

		bool Create( 
			const string &faceModelFileName,
			const string &hairModelFileName,
			const string &bodyModelFileName,
			const string &handModelFileName,
			const string &legModelFileName,
			const string &tailModelFileName );

		virtual bool SetAnimation( const string &aniFileName, const int nAniFrame=0,  
			const bool isLoop=false, const bool isBlend=true ) override;
		void SetBodyModel( const string &fileName );
		void SetHandModel( const string &fileName );
		void SetLegModel( const string &fileName );
		void SetFaceModel( const string &fileName );
		void SetHairModel( const string &fileName );
		void SetTailModel( const string &fileName );

		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &tm) override;


	private:
		cModel *m_models[ TERA_MODEL::MAX_TYPE];

		cBoneNode *m_bodyNeckNode; // reference
		cBoneNode *m_bodyFaceNode; // reference
		cBoneNode *m_faceFaceNode; // reference

		cBoneNode *m_faceNeckNode; // reference
		cBoneNode *m_hairHairNode; // reference

	};

}
