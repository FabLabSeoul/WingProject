#pragma once

#include "EnumDevices.h"
#define CENUMERATESERIAL_USE_STL

// CComPortCombo
// http://www.codeproject.com/Articles/293273/Ready-to-use-serial-port-enumeration-list-box 

class CComPortCombo : public CComboBox
{
	DECLARE_DYNAMIC(CComPortCombo)

public:
	CComPortCombo();
	virtual ~CComPortCombo();

	BOOL InitList(int nDefPort = -1);

	inline BOOL InitList(LPCTSTR lpszPort) 
	{ return InitList(CEnumDevices::GetPortFromName(lpszPort)); }
	inline BOOL IsPhysicalPort() const  
	{ return GetCurData() && !(GetCurData() & DATA_VIRTUAL_MASK); }
	inline BOOL IsVirtualPort() const  
	{ return (GetCurData() & DATA_VIRTUAL_MASK); }
	inline int GetPortNum() const 
	{ return GetCurData() & DATA_PORT_MASK; }
	inline bool GetFileName(CString& str) const
	{ return CEnumDevices::GetFileName(GetPortNum(), str); }

	inline void SetNoneItem(BOOL bSet = 1) { m_bNoneItem = bSet; }
	inline void SetOnlyPhysical(BOOL bSet = 1) { m_bOnlyPhysical = bSet; }
	inline void SetOnlyPresent(BOOL bSet = 1) { m_bOnlyPresent = bSet; }
	inline void SetNoneStr(LPCTSTR s) { ASSERT(s && *s); m_strNone = s; }

protected:
	static void CALLBACK CallbackWrapper(CObject* pObject, const CEnumDevInfo* pInfo);
	void AddItem(const CEnumDevInfo* pInfo);
	int GetCurData() const;

	BOOL			m_bNoneItem;
	BOOL			m_bOnlyPhysical;
	BOOL			m_bOnlyPresent;
	int             m_nDefPort;
	CString			m_strNone;

public:
	DECLARE_MESSAGE_MAP()
};


