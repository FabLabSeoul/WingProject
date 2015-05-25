#pragma once

#include "../action/actiondef.h"


namespace graphic
{

	class cCharacter : public cModel, 
								public ai::IArtificialIntelligence
	{
	public:
		cCharacter(const int id=-1);
		virtual ~cCharacter();

		virtual bool Move(const float elapseTime) override;
		virtual void Clear() override;

		// Action
		void SetAction(ai::cAction *action);
		void PushAction(ai::cAction *action);
		void StartAction();
		void EndAction();
		ai::cAction* GetAction();
		bool IsAction(ai::ACTION_TYPE::TYPE type);
		void ClearAction();

		// ai interface implement
		virtual Matrix44& aiGetTransform() override;
		virtual void aiSetAnimation(const string &animationName) override;


		void SetActionData(const vector<sActionData> &actions);
		void Action(const CHARACTER_ACTION::TYPE type, const int param1=0, const int param2=0);


	private:
		vector<sActionData> m_actions;
		CHARACTER_ACTION::TYPE m_action;
		
		ai::cAction *m_rootAction;
		string m_curAniName; // 플레이 중인 애니메이션.
	};

}
