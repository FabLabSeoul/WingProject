// 인공지능 객체
// 게임모델과 연동되어 기능한다.
// 트리 구조이며 자식으로 cObject 를 가질 수 있다.
// cAction 을 관리하며, 행동처리는 cAction이 하지만, 게임과
// 연동되는 부분은 cObject 가 담당한다.
#pragma once


namespace ai
{
	class cAction;

	class cObject
	{
	public:
		cObject(cObject *parent, const int id, const string &name);
		virtual ~cObject();

		void AddChild(cObject *obj);
		bool RemoveChild(cObject *obj);
		cObject* Find(const int objId, bool isfindRecursive=true);
		vector<cObject*>& GetChildren();
		void PostMsg(const sMsg &msg);
		void SendMsg(const sMsg &msg);
		void Clear();

		virtual bool Move(const float elapseTime);
		bool DispatchMessage(const sMsg &msg);

		int GetId() const;
		const string& GetName() const;
		cObject* GetParent();

		// Action
		void SetAction(cAction *action);
		void PushAction(cAction *action);
		void StartAction();
		void EndAction();
		cAction* GetAction();
		bool IsAction(ACTION_TYPE::TYPE type);
		void ClearAction();

		bool operator==(const cObject &rhs) const;


	protected:
		int m_id;
		string m_name;
		cObject *m_parent;
		vector<cObject*> m_children;
		
		// Action
		cAction *m_rootAction;
	};


	inline int cObject::GetId() const { return m_id; }
	inline const string& cObject::GetName() const { return m_name; }
	inline cObject* cObject::GetParent() { return m_parent; }
	inline vector<cObject*>& cObject::GetChildren() { return m_children; }
	inline bool cObject::operator==(const cObject &rhs) const { return m_id == rhs.GetId(); }
	inline cAction* cObject::GetAction() { return m_rootAction; }
}

