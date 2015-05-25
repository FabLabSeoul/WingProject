// Action ����
// �ൿ�� ó���ϴ� Ŭ������. cAction Ŭ������ �߻�Ŭ������ �ƹ� �ൿ��
// ���� �ʴ´�. ��ü���� �ൿ�� �� Ŭ������ ��ӹ޾Ƽ� ó���Ѵ�.
// Action Ʈ���� �����ϱ� ���� �������̽� ��
// �ൿ�� ���۰� ��, ����, �޼����� ó���ϴ� �������̽��� ���ǵȴ�.
//
// �ൿ�� ó���ϱ� ���� ���� �⺻���� ��ɰ� �������̽��� �����Ѵ�.
#pragma once


namespace ai
{

	//class cObject;
	class IArtificialIntelligence;

	class cAction
	{
	public:
		cAction(IArtificialIntelligence *obj=NULL, 
			const string &name="action", 
			const string &animationName="", 
			const ACTION_TYPE::TYPE type=ACTION_TYPE::NONE);

		virtual ~cAction();

		virtual void Start(cAction *prevAction);
		virtual void Resume(cAction *prevAction);
		virtual void End();
		virtual bool Move(const float elapseTime);
		virtual bool Update(const float elapseTime);
		virtual bool ActionUpdate(const float elapseTime);
		virtual bool MessageProccess(const sMsg &msg);
		virtual void Clear();

		void PushAction(cAction *action);
		void PopAction();
		void RemoveAction(cAction *action);
		void RemoveAction(const string &actionName);
		void ClearChildAction();
		cAction* FindAction(const string &actionName, bool isFindChild=true);
		cAction* FindAction(ACTION_TYPE::TYPE type, bool isFindChild=true);
		vector<cAction*>& GetChildren();
		bool IsCurrentAction(ACTION_TYPE::TYPE type);
		bool IsAction(ACTION_TYPE::TYPE type);
		cAction* GetLeafAction();

		ACTION_STATE::TYPE GetState() const;
		virtual int GetSubState() const;
		const string& GetName() const;
		void SetOwner(IArtificialIntelligence *owner);
		IArtificialIntelligence* GetOwner();
		cAction* GetCurrentAction();

		void SetName(const string &name);
		void SetActionState(ACTION_STATE::TYPE state);

		bool IsRun();

		bool operator== (const cAction &rhs);


	protected:
		void NextAction();
		cAction* GetNextAction();
		void SetParent(cAction *pParent);


	protected:
		ACTION_STATE::TYPE m_state;
		ACTION_TYPE::TYPE m_type;
		string m_name;
		string m_animationName;
		IArtificialIntelligence *m_owner; // reference
		cAction *m_parent;
		vector<cAction*> m_children;
		cAction *m_current;
	};


	inline ACTION_STATE::TYPE cAction::GetState() const { return m_state; }
	inline int cAction::GetSubState() const { return 0; }
	inline const string& cAction::GetName() const { return m_name; }
	inline IArtificialIntelligence* cAction::GetOwner() { return m_owner; }
	inline void cAction::SetOwner(IArtificialIntelligence *owner) { m_owner = owner; }
	inline cAction* cAction::GetCurrentAction() { return m_current; }
	inline vector<cAction*>& cAction::GetChildren() { return m_children; }
	inline bool cAction::IsCurrentAction(ACTION_TYPE::TYPE type) { return m_type == type; }
	inline void cAction::SetName(const string &name) { m_name = name; }
	inline void cAction::SetActionState(ACTION_STATE::TYPE state) { m_state = state; }
	inline bool cAction::operator== (const cAction &rhs) { return m_name == rhs.GetName(); }
	inline void cAction::SetParent(cAction *parent) { m_parent = parent; }
}
