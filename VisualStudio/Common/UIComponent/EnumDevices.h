#pragma once

#if !defined(_WIN32_WINNT)
#error "Must have _WIN32_WINNT definition. Include stdafx.h before."
#endif

#include <setupapi.h>								// SetupAPI version and types

#define DATA_PORT_MASK		0x0000FFFF
#define DATA_VIRTUAL_MASK	0x00010000

class CEnumDevInfo;									// forward declaration for typedef
typedef void (CALLBACK* EnumCallBack)(CObject*, const CEnumDevInfo*);

// Late binding must be used when SetupAPI version is < 0x0501 (Windows XP).
// When Windows 95 or Windows NT4 must be supported, we should call all API functions
//  by function pointers.
#if _SETUPAPI_VER < 0x0501
typedef BOOL (WINAPI * LP_GetCustomDeviceProperty)(
    HDEVINFO, PSP_DEVINFO_DATA, PCTSTR, DWORD, PDWORD, PBYTE, DWORD, PDWORD);
#if _SETUPAPI_VER <= 0x0400
typedef BOOL (WINAPI * LP_ClassGuidsFromNameA)(PCSTR, LPGUID, DWORD, PDWORD);
typedef HDEVINFO (WINAPI * LP_GetClassDevsA)(const GUID *, PCSTR, HWND, DWORD);
typedef BOOL (WINAPI * LP_DestroyDeviceInfoList)(HDEVINFO);
typedef BOOL (WINAPI * LP_EnumDeviceInfo)(HDEVINFO, DWORD, PSP_DEVINFO_DATA);
typedef BOOL (WINAPI * LP_GetDeviceRegistryProperty)(
	HDEVINFO, PSP_DEVINFO_DATA, DWORD, PDWORD, PBYTE, DWORD, PDWORD);
#endif
#endif

#define INFO_PORT_NAME		0x00000001
#define INFO_NAME			0x00000002
#define INFO_SERVICE		0x00000004
#define INFO_MFG			0x00000008
#define INFO_PNP_NAME		0x00000010
#define INFO_DESCR			0x00000020
#define INFO_CLASS_NAME		0x00000040
#define INFO_CLASS_GUID		0x00000080
#define INFO_ALL			0x0FFFFFFF
#define INFO_NON_PRESENT	0x80000000

#define INFO_PORTS		(INFO_PORT_NAME | INFO_NAME | INFO_SERVICE)

class CEnumDevInfo
{
public:
	CEnumDevInfo();

	unsigned m_nStatus;
	int m_nPortNum;									// COM ports only
	CString m_strPort;								// COM and LPT ports only
	CString m_strName;								// user friendly name
	CString m_strService;
	CString m_strManufacturer;
	CString m_strPnPName;
	CString m_strDescription;
	CString m_strClassName;
	CString m_strClassGUID;
};

class CEnumDevices
{
public:
	CEnumDevices();
	~CEnumDevices();

	BOOL EnumSerialPorts(CObject* pList, EnumCallBack pCallBack, BOOL bPresent = TRUE);
	BOOL EnumDevices(unsigned nInfo, CObject* pList, EnumCallBack pCallBack, const GUID* lpGUID);
//	void EnumSerialPortsOld(CObject* pList, EnumCallBack pCallBack, int nMaxPort = 10);
	static bool GetFileName(int nPortNum, CString& str);
	static int GetPortFromName(LPCTSTR lpszPort);

	// Max. number of COM ports supported by Windows versions:
	// Windows 9x: 256
	// Windows NT: 4096 (should be limited to 2000 with NT4 due to memory usage)
	inline int GetMaxComPorts() const 
	{
#if defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0500
		return m_bIsNT4 ? 2000 : (m_bIsNT ? 4096 : 256);
#else
		return m_bIsNT ? 4096 : 256;
#endif
	}
	inline void GetComDeviceStr(CString& str, int nPort) const
	{ 
		ASSERT(nPort > 0 && nPort <= GetMaxComPorts()); 
		str.Format(nPort < 10 ? _T("COM%d") : _T("\\\\.\\COM%d"), nPort); 
	}

protected:
	BOOL InitSetupApi();
	HDEVINFO GetClassDevs(const GUID* lpGUID, BOOL bPresent = TRUE);
	BOOL GetPropertyString(HDEVINFO hDevInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty, CString& str);
	BOOL GetCustomPropertyString(HDEVINFO hDevInfo, PSP_DEVINFO_DATA pDevInfoData, LPCTSTR lpszEntry, CString& str);
	void Error(LPCTSTR lpszFunction, bool bMsg = false) const;
#if defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0500
	BOOL EnumSerialPortsNT4(CObject* pList, EnumCallBack pCallBack);
#endif

	BOOL			m_bIsNT;						// TRUE if NT detected (NT4, 2000 or later)
#if defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0500
	BOOL			m_bIsNT4;						// TRUE if NT4 detected
#endif

#if _SETUPAPI_VER < 0x0501							// late binding; get pointers to used setupapi functions
	HINSTANCE						m_hinstSetup;
	LP_GetCustomDeviceProperty		m_GetCustomDeviceProperty;
#if _SETUPAPI_VER <= 0x0400							// Windows 95 or NT4
	LP_GetClassDevsA				m_GetClassDevsA;
	LP_DestroyDeviceInfoList		m_DestroyDeviceInfoList;
	LP_EnumDeviceInfo				m_EnumDeviceInfo;
	LP_GetDeviceRegistryProperty	m_GetDeviceRegistryProperty;
#endif // _SETUPAPI_VER <= 0x0400
#endif // _SETUPAPI_VER < 0x0501
};
