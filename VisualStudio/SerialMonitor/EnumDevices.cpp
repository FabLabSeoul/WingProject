// Enumerate serial ports using the SetupAPI.
//
// Windows 95 and NT4 notes:
// The app may be linked to SetupAPI.lib even if Win 95 or NT4 must be supported when:
// - IE or specific service packs has been installed on the user's machine (SetupAPI.dll is present)
// - The app is build with delay loading for SetupAPI.dll and the OS version is checked
//   before calling any setup functions. No Setup functions are called when Init() didn't find the DLL. 
//
// Windows 9x notes:
// - Must use ANSI versions (Unicode versions return ERROR_CALL_NOT_IMPLEMENTED)
// - Must use port class rather than serial interface class with SetupDiGetClassDevs.
// 
// Tested with Windows 2000 SP4, XP SP2, 7, 98SE (Virtual PC 2007)
//
// SetupAPI does not work with NT4!
// The SetupAPI DLL may be present, but the functions used here can't be used to enumerate the
//  serial ports. Windows NT4 contains a registry key with the ports GUID, but this key only
//  contains the value names "Class" and "Install32" and no subkeys with device information.
// Therefore, the provided function EnumPortsNT4() must be used with NT4.
//
// Author: Jochen Arndt <jochen.arndt@t-online.de>

#include "stdafx.h"
#include ".\EnumDevices.h"

//#include <setupapi.h>								// included by EnumSerial.h
#include <WinIoCtl.h>								// Pre-defined GUIDs

// Size for property buffer in bytes.
// NOTE: Max. string length is half size with Unicode! 
#define PROP_BUF_SIZE	1024

#if _SETUPAPI_VER > 0x0400
#pragma comment(lib, "setupapi")					// link setupapi of Win 95 and NT4 must be not supported
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEnumDevInfo::CEnumDevInfo()
{
	m_nStatus = 0;
	m_nPortNum = -1;
}

CEnumDevices::CEnumDevices()
{
	m_bIsNT = TRUE;									// assume Windows NT4 or later
#if defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0500	// NT4 must be supported
	m_bIsNT4 = FALSE;								// assume not NT4
#endif

#if _SETUPAPI_VER < 0x0501							// late binding; get pointers to used setupapi functions
	m_hinstSetup = NULL;							// setupapi.dll instance handle
	m_GetCustomDeviceProperty = NULL;
#if _SETUPAPI_VER <= 0x0400							// Windows 95 or NT4 must be supported
	m_GetClassDevsA = NULL;
	m_DestroyDeviceInfoList = NULL;
	m_EnumDeviceInfo = NULL;
	m_GetDeviceRegistryProperty = NULL;
#endif // _SETUPAPI_VER <= 0x0400
#endif // _SETUPAPI_VER < 0x0501
}

CEnumDevices::~CEnumDevices()
{
#if _SETUPAPI_VER < 0x0501
	if (NULL != m_hinstSetup)
		::FreeLibrary(m_hinstSetup);
#endif
}

// Get COM port file name from port number.
/*static*/ bool CEnumDevices::GetFileName(int nPortNum, CString& str)
{
	if (nPortNum > 0)
		str.Format(nPortNum >= 10 ? _T("\\\\.\\COM%d") : _T("COM%d"), nPortNum);
	else
		str.Empty();
	return nPortNum >= 0 ? true : false;
}

// Get port number form COM port file name.
// Return value:
// -1 : Invalid name
//  0 : Empty name treated as "no port".
// >0 : Port number
/*static*/ int CEnumDevices::GetPortFromName(LPCTSTR lpszPort)
{
	ASSERT(lpszPort != NULL);

	int nPortNum = -1;
	if (lpszPort != NULL)
	{
		if (*lpszPort == _T('\0'))
			nPortNum = 0;
		else
		{
			LPCTSTR s = lpszPort;
			if (_tcsncmp(s, _T("\\\\.\\"), 4) == 0)
				s += 4;
			if (_tcsnicmp(s, _T("COM"), 3) == 0)
				nPortNum = _tstoi(s + 3);
			if (nPortNum <= 0)
			{
				TRACE1("Invalid COM port file name %s\n", lpszPort);
				nPortNum = -1;
			}
		}
	}
	return nPortNum;
}

BOOL CEnumDevices::InitSetupApi()
{
	BOOL bResult = TRUE;

#if WINVER < 0x0500
	OSVERSIONINFO VerInfo;							// Get Windows version information.
	VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&VerInfo);
	m_bIsNT = (VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);
#if defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0500	// NT4 must be supported
	m_bIsNT4 = (VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (VerInfo.dwMajorVersion == 4); 
	if (m_bIsNT4)									// Setup API not used with NT4
		return TRUE;
#endif
#endif

#if _SETUPAPI_VER < 0x0501							// late binding; get pointers to used setupapi functions
	if (NULL == m_hinstSetup)
	{
		m_hinstSetup = ::LoadLibrary(_T("setupapi"));
		if (m_hinstSetup == NULL)
		{
			bResult = FALSE;
			TRACE0("SetupApi.dll not found\n");
		}
		else
		{

			m_GetCustomDeviceProperty =				// requires setupapi >= 0x0501
#ifdef _UNICODE
				(LP_GetCustomDeviceProperty)::GetProcAddress(m_hinstSetup, "SetupDiGetCustomDevicePropertyW");
#else
				(LP_GetCustomDeviceProperty)::GetProcAddress(m_hinstSetup, "SetupDiGetCustomDevicePropertyA");
#endif
#if _SETUPAPI_VER <= 0x0400							// Windows 95 or NT4 must be supported
			m_GetClassDevsA =
				(LP_GetClassDevsA)::GetProcAddress(m_hinstSetup, "SetupDiGetClassDevsA");
			m_EnumDeviceInfo =
				(LP_EnumDeviceInfo)::GetProcAddress(m_hinstSetup, "SetupDiEnumDeviceInfo");
			m_DestroyDeviceInfoList =
				(LP_DestroyDeviceInfoList)::GetProcAddress(m_hinstSetup, "SetupDiDestroyDeviceInfoList");
#ifdef _UNICODE
			m_GetDeviceRegistryProperty = m_bIsNT ?
				(LP_GetDeviceRegistryProperty)::GetProcAddress(m_hinstSetup, "SetupDiGetDeviceRegistryPropertyW") :
				(LP_GetDeviceRegistryProperty)::GetProcAddress(m_hinstSetup, "SetupDiGetDeviceRegistryPropertyA");
#else
			m_GetDeviceRegistryProperty = 
				(LP_GetDeviceRegistryProperty)::GetProcAddress(m_hinstSetup, "SetupDiGetDeviceRegistryPropertyA");
#endif
			if (m_GetClassDevsA == NULL ||
				m_EnumDeviceInfo == NULL ||
				m_DestroyDeviceInfoList == NULL ||
				m_GetDeviceRegistryProperty == NULL)
			{
				bResult = FALSE;					// required function not found
				TRACE("Failed to locate required setupapi.dll functions\n");
			}
#endif // _SETUPAPI_VER <= 0x0400
		}
	}
#endif // _SETUPAPI_VER < 0x0501
	return bResult;
}

// Generate error message for SetupAPI errors.
// NOTE: Must use special conversion macro HRESULT_FROM_SETUPAPI.
void CEnumDevices::Error(LPCTSTR lpszFunction, bool bMsg /*= false*/) const
{
	ASSERT(lpszFunction != NULL);

	DWORD dwErr = ::GetLastError();
	if (dwErr)
	{
		LPVOID lpMsgBuf;
		if (::FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			HRESULT_FROM_SETUPAPI(dwErr),			// special conversion for SetupAPI errors
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default user language
			reinterpret_cast<LPTSTR>(&lpMsgBuf),
			0,
			NULL))
		{
			TRACE("SetupAPI error %d (%#X) in %s: %s\n", 
				dwErr, dwErr, lpszFunction, 
				reinterpret_cast<LPCTSTR>(lpMsgBuf));
			if (bMsg && dwErr != ERROR_CALL_NOT_IMPLEMENTED)
			{
				CString strMsg;
				strMsg.Format(_T("SetupAPI error in %s:\n%s"), 
					lpszFunction, 
					reinterpret_cast<LPCTSTR>(lpMsgBuf));
				AfxMessageBox(static_cast<LPCTSTR>(strMsg));
			}
			::LocalFree(lpMsgBuf);
		}
	}
}

// With Windows 2K/XP and later we can use GUID_DEVINTERFACE_COMPORT with DIGCF_DEVICEINTERFACE.
// This does not work with Windows 9x where we must use
//  GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR which returns also LPT ports.
// We can always use the ANSI version here, because the PnP enum/name parameter isn't used.
HDEVINFO CEnumDevices::GetClassDevs(const GUID* lpGUID, BOOL bPresent /*= TRUE*/)
{
	DWORD dwFlags = bPresent ? DIGCF_PRESENT : 0;	// only present devices or all that has been seen
	if (lpGUID == &GUID_DEVINTERFACE_COMPORT)		// Fixed GUID for COM ports (NT only) 
		dwFlags |= DIGCF_DEVICEINTERFACE;
	else if (lpGUID == NULL)						// No specific GUID (all classes)
		dwFlags |= DIGCF_ALLCLASSES;
#if _SETUPAPI_VER <= 0x0400							// Windows 95 or NT4
	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
	if (m_GetClassDevsA == NULL)
		::SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	else
		hDevInfo = m_GetClassDevsA(lpGUID, NULL, NULL, dwFlags); 
#else
	HDEVINFO hDevInfo = ::SetupDiGetClassDevsA(lpGUID, NULL, NULL, dwFlags); 
#endif // _SETUPAPI_VER <= 0x0400
	if (hDevInfo == INVALID_HANDLE_VALUE)
		Error(_T("SetupDiGetClassDevs"));
	return hDevInfo;
}

BOOL CEnumDevices::GetPropertyString(HDEVINFO hDevInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty, CString& str)
{
	ASSERT(hDevInfo != INVALID_HANDLE_VALUE);
	ASSERT(pDevInfoData != NULL);
	ASSERT(dwProperty < SPDRP_MAXIMUM_PROPERTY);

	BOOL bResult = FALSE;
	DWORD dwDataType = 0;							// registry data type (e.g. REG_SZ)
	DWORD dwReqSize = 0;							// retrieve required buffer size
	BYTE lpszBuf[PROP_BUF_SIZE];
	
#if _SETUPAPI_VER <= 0x0400							// Windows 95 or NT4
	if (m_GetDeviceRegistryProperty == NULL)
		::SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	else
	{
		bResult = m_GetDeviceRegistryProperty(
			hDevInfo,								// <- device info set handler
			pDevInfoData,							// <- enumerated device info data
			dwProperty,								// <- Description and name as displayed in the hardware config
			&dwDataType,							// -> registry data type (e.g. REG_SZ), optional
			lpszBuf,								// -> output buffer filled with registry data
			PROP_BUF_SIZE,							// <- output buffer size
			&dwReqSize);							// -> required size, optional
		if (bResult && dwDataType == REG_SZ)		// just to be sure ...
		{
#ifdef _UNICODE
			if (!m_bIsNT)							// ANSI version has been called
				str = reinterpret_cast<LPCSTR>(lpszBuf);
			else									// Unicode version has been called
				str = reinterpret_cast<LPCTSTR>(lpszBuf);
#else
			str = reinterpret_cast<LPCSTR>(lpszBuf);
#endif
		}
	}
#else
#ifdef _UNICODE
	if (m_bIsNT)									// call the Unicode version
	{
		bResult = ::SetupDiGetDeviceRegistryProperty(
			hDevInfo,								// <- device info set handler
			pDevInfoData,							// <- enumerated device info data
			dwProperty,								// <- Description and name as displayed in the hardware config
			&dwDataType,							// -> registry data type (e.g. REG_SZ), optional
			lpszBuf,								// -> output buffer filled with registry data
			PROP_BUF_SIZE,							// <- output buffer size
			&dwReqSize);							// -> required size, optional
		if (bResult && dwDataType == REG_SZ)		// just to be sure ...
			str = reinterpret_cast<LPCTSTR>(lpszBuf);
	}
	else											// call the ANSI version with Windows 9x
#endif
	{
		bResult = ::SetupDiGetDeviceRegistryPropertyA(
			hDevInfo,								// <- device info set handler
			pDevInfoData,							// <- enumerated device info data
			dwProperty,								// <- Description and name as displayed in the hardware config
			&dwDataType,							// -> registry data type (e.g. REG_SZ), optional
			lpszBuf,								// -> output buffer filled with registry data
			PROP_BUF_SIZE,							// <- output buffer size
			&dwReqSize);							// -> required size, optional
		if (bResult && dwDataType == REG_SZ)		// just to be sure ...
			str = reinterpret_cast<LPCSTR>(lpszBuf);
	}
#endif // _SETUPAPI_VER <= 0x0400
	if (!bResult)
	{
		CString s;
		s.Format(_T("SetupDiGetDeviceRegistryProperty(%d)"), dwProperty);
		Error(s);
	}
	else
	{
		ASSERT(dwDataType == REG_SZ);				// not a string property
		TRACE2(" Property %#X = %s\n", dwProperty, str);
	}
	return bResult;
}

// Get value from 'Device Parameters' key.
// Requires Windows XP or later (API version >= 5.01).
BOOL CEnumDevices::GetCustomPropertyString(HDEVINFO hDevInfo, PSP_DEVINFO_DATA pDevInfoData, LPCTSTR lpszEntry, CString& str)
{
	ASSERT(hDevInfo != INVALID_HANDLE_VALUE);
	ASSERT(pDevInfoData != NULL);
	ASSERT(lpszEntry != NULL && *lpszEntry != _T('\0'));

	DWORD dwDataType = 0;							// registry data type (e.g. REG_SZ)
	DWORD dwReqSize = 0;							// retrieve required buffer size
	BYTE lpszBuf[PROP_BUF_SIZE];
	
#if _SETUPAPI_VER < 0x0501							// < Windows XP
	BOOL bResult = FALSE;
	if (m_GetCustomDeviceProperty == NULL)
	{
		::SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	}
	else
	{
		bResult = m_GetCustomDeviceProperty(		// get value from 'Device Parameters' key
			hDevInfo, pDevInfoData,
			lpszEntry,								// get value from entry
			0, &dwDataType,
			lpszBuf, PROP_BUF_SIZE, &dwReqSize);
	}
#else												// Windows XP or later
	BOOL bResult = ::SetupDiGetCustomDeviceProperty(
		hDevInfo, pDevInfoData,
		lpszEntry, 0, &dwDataType,
		lpszBuf, PROP_BUF_SIZE, &dwReqSize);
#endif
	if (!bResult)
	{
		CString s;
		s.Format(_T("SetupDiGetCustomDeviceProperty(%s)"), lpszEntry);
		Error(s);
	}
	else
	{
		ASSERT(dwDataType == REG_SZ);				// not a string property
		if (dwDataType == REG_SZ)
		{
			str = reinterpret_cast<LPCTSTR>(lpszBuf);
			TRACE2(" Custom property %s = %s\n", lpszEntry, str);
		}
	}
	return bResult;
}

// Enumerate serial ports and pass data to ListBox/ComboBox using call back function.
BOOL CEnumDevices::EnumSerialPorts(CObject* pList, EnumCallBack pCallBack, BOOL bPresent /*= TRUE*/)
{
#if defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0500	// NT4 must be supported
	if (m_bIsNT4)									// must use special function with NT4
		return EnumSerialPortsNT4(pList, pCallBack);
#endif
	return EnumDevices(
		bPresent ? INFO_PORTS : INFO_PORTS | INFO_NON_PRESENT, 
		pList, 
		pCallBack, 
#if defined(_WIN32_WINDOWS) && _WIN32_WINDOWS < 0x0500 // Windows 9x must be supported
		!m_bIsNT ?									// GUID_DEVINTERFACE_COMPORT not supported with Win 9x
		&GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR :
#endif
		&GUID_DEVINTERFACE_COMPORT);
}

// Enumerate devices and pass data using call back function.
BOOL CEnumDevices::EnumDevices(unsigned nInfo, CObject* pList, EnumCallBack pCallBack, const GUID* lpGUID)
{
//	ASSERT(pList != NULL);							// may be used by any enumeration
	ASSERT(pCallBack != NULL);

	DWORD nNumDev = 0;								// count interfaces
	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;		// device infoset handle
	SP_DEVINFO_DATA	DevInfoData;					// device info data for enumerated device

	::ZeroMemory(&DevInfoData, sizeof(SP_DEVINFO_DATA));
	DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	TRACE1("EnumDevices(%#X)\n", nInfo);
	BOOL bResult = InitSetupApi();
	if (bResult)
	{
		hDevInfo = GetClassDevs(lpGUID, !(nInfo & INFO_NON_PRESENT));
		if (hDevInfo == INVALID_HANDLE_VALUE)
			bResult = FALSE;
	}

	try
	{
		while (bResult)								// enumerate devices
		{
#if _SETUPAPI_VER <= 0x0400							// Windows 95 or NT4
			bResult = m_EnumDeviceInfo(				// get device info
				hDevInfo, nNumDev++, &DevInfoData);
#else
			bResult = ::SetupDiEnumDeviceInfo(		// get device info
				hDevInfo, nNumDev++, &DevInfoData);
#endif
			if (!bResult)
			{
				if (::GetLastError() == ERROR_NO_MORE_ITEMS)
					bResult = TRUE;					// no more devices
				else
					Error(_T("SetupDiEnumDeviceInfo()"));
				break;
			}

			CEnumDevInfo Info;

			if ((nInfo & INFO_NAME) &&
				GetPropertyString(hDevInfo, &DevInfoData, SPDRP_FRIENDLYNAME, Info.m_strName))
				Info.m_nStatus |= INFO_NAME;
			if ((nInfo & INFO_SERVICE) &&
				GetPropertyString(hDevInfo, &DevInfoData, SPDRP_SERVICE, Info.m_strService))
				Info.m_nStatus |= INFO_SERVICE;
			if ((nInfo & INFO_MFG) &&
				GetPropertyString(hDevInfo, &DevInfoData, SPDRP_MFG, Info.m_strManufacturer))
				Info.m_nStatus |= INFO_MFG;
			if ((nInfo & INFO_DESCR) &&
				GetPropertyString(hDevInfo, &DevInfoData, SPDRP_DEVICEDESC, Info.m_strDescription))
				Info.m_nStatus |= INFO_DESCR;
			if ((nInfo & INFO_CLASS_NAME) &&
				GetPropertyString(hDevInfo, &DevInfoData, SPDRP_CLASS, Info.m_strClassName))
				Info.m_nStatus |= INFO_CLASS_NAME;
			if ((nInfo & INFO_CLASS_GUID) &&
				GetPropertyString(hDevInfo, &DevInfoData, SPDRP_CLASSGUID, Info.m_strClassName))
				Info.m_nStatus |= INFO_CLASS_GUID;
			if ((nInfo & INFO_PNP_NAME) &&
				GetPropertyString(hDevInfo, &DevInfoData, SPDRP_ENUMERATOR_NAME, Info.m_strPnPName))
				Info.m_nStatus |= INFO_PNP_NAME;
			if ((nInfo & INFO_PORT_NAME) &&
				GetCustomPropertyString(hDevInfo, &DevInfoData, _T("PortName"), Info.m_strPort))
				Info.m_nStatus |= INFO_PORT_NAME;
			// Serial port specific handling:
			//  Get the port name 'COMx' and the user friendly name.
			//  The used function for the port name requires a API version >= 5.01 (Windows XP).
			//  With older versions, we will extract the port name from the user friendly name.
			//  Extraction assumes that the port name is provided in parentheses as '(COMx)'.
			//  Looking for 'COM' only does not always work (e.g. with German Win 98 where
			//  the user friendly name is 'COM Anschlu?(COM1)')!
			if ((Info.m_nStatus & INFO_NAME) &&
				!(Info.m_nStatus & INFO_PORT_NAME))
			{
				int nStartPos = Info.m_strName.Find(_T("(COM"));
				if (nStartPos >= 0)
				{
					int nEndPos = Info.m_strName.Find(_T(')'), ++nStartPos);
					if (nEndPos > 0)
					{
						Info.m_strPort = Info.m_strName.Mid(nStartPos, nEndPos - nStartPos);
						Info.m_nStatus |= INFO_PORT_NAME;
					}
				}
			}
			if ((Info.m_nStatus & INFO_PORT_NAME) &&	// found a serial port
				Info.m_strPort.Left(3) == _T("COM"))
//				&& _istdigit(strPort.GetAt(4)))
			{
				Info.m_nPortNum = _tstoi(Info.m_strPort.GetString() + 3);
				// Getting the service type fails with Windows 9x.
				// If the service type is not "Serial", it is probably a virtual
				//  COM port (e.g. USB to serial converter or internal modem card).
				// This is indicated by adding a special bit to the port number.
				if (Info.m_nStatus & INFO_SERVICE)
				{
					if (Info.m_nPortNum &&
						Info.m_strService.CompareNoCase(_T("Serial")))
						Info.m_nPortNum |= DATA_VIRTUAL_MASK;
				}
				else
					Info.m_strService = _T("Serial");
			}
			pCallBack(pList, &Info);
		} // while (bResult)
	}
	catch (CMemoryException *e)
	{
		e->Delete();
		bResult = FALSE;
	}
	if (hDevInfo != INVALID_HANDLE_VALUE)
	{
#if _SETUPAPI_VER <= 0x0400
		m_DestroyDeviceInfoList(hDevInfo);
#else
		::SetupDiDestroyDeviceInfoList(hDevInfo);
#endif
	}
	TRACE1(" %d devices found\n", --nNumDev);
	return bResult;
}

#if defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0500
// Enumerate serial ports by searching in the registry key "HKLM\Hardware\DeviceMap\SerialComm".
// Check for NT4 before calling this function!
BOOL CEnumDevices::EnumSerialPortsNT4(CObject* pList, EnumCallBack pCallBack)
{
//	ASSERT(pList != NULL);							// may be used by any enumeration
	ASSERT(pCallBack != NULL);

	int nPorts = 0;
	HKEY hKey;
	TCHAR lpszDevName[40], lpszPortName[20];
	DWORD dwType;

	TRACE0("EnumSerialPortsNT4\n");
	LONG lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("Hardware\\DeviceMap\\SerialComm"), 
		0, KEY_READ, &hKey);
	if (lResult != ERROR_SUCCESS) 
	{
		TRACE1(" Failed. RegOpenKeyEx returns %#X\n", lResult);
	}
	while (lResult == ERROR_SUCCESS) 
	{
		DWORD dwDevNameSize = sizeof(lpszDevName);
		DWORD dwPortNameSize = sizeof(lpszPortName);
		lResult = ::RegEnumValue(hKey, nPorts, 
			lpszDevName, &dwDevNameSize,			// value name (Serial<n>)
			NULL, &dwType, 
			reinterpret_cast<LPBYTE>(lpszPortName),	// value data (COMx)
			&dwPortNameSize);
		if (lResult != ERROR_SUCCESS)
		{
			VERIFY(::RegCloseKey(hKey) == ERROR_SUCCESS);
			if (lResult != ERROR_NO_MORE_ITEMS)
				TRACE1(" Failed. RegEnumValue returns %#X\n", lResult);
		}
		else
		{
			++nPorts;
			ASSERT(dwType == REG_SZ);
			TRACE2(" %s, device %s\n", lpszPortName, lpszDevName);
			CEnumDevInfo Info;
			Info.m_nStatus = INFO_NAME | INFO_PORT_NAME;
			Info.m_nPortNum = _tstoi(lpszPortName+3);
			Info.m_strPort = Info.m_strName = lpszPortName;
			Info.m_strService = _T("Serial");
			pCallBack(pList, &Info);
		}
	}
	TRACE1(" %d serial ports found\n", nPorts);
	return (lResult == ERROR_SUCCESS || lResult == ERROR_NO_MORE_ITEMS);
}
#endif

#if 0
// Enumerate serial ports using GetDefaultCommConfig.
// Just for reference.
void CEnumDevices::EnumSerialPortsOld(CObject* pList, EnumCallBack pCallBack, int nMaxPort /*= 10*/)
{
//	ASSERT(pList != NULL);							// may be used by any enumeration
	ASSERT(pCallBack != NULL);
		
	int nPorts = 0;
	COMMCONFIG CC;
	CString strPortName;
	for (int i = 1 ; i <= nMaxPort; i++)
	{
		bool bPortExist = false;
		strPortName.Format(_T("COM%d"), i);			// name of port
		// GetDefaultCommConfig() accepts only COM1 to COM9 without extended syntax!
		// Requires about 10 ms per port.
		// Works also for opened ports.
		if (i < 10)
		{
			DWORD dwSize = sizeof(COMMCONFIG);		// must be re-initialized inside loop
			if (::GetDefaultCommConfig(static_cast<LPCTSTR>(strPortName), &CC, &dwSize))
			{
				if (CC.dwProviderSubType == PST_RS232)
					bPortExist = true;				// add port to list
			}
		}
		else										// try to open the port
		{
			CString strPortFile;
			strPortFile.Format(_T("\\\\.\\COM%d"), i);	// file name of port
			HANDLE hPort = ::CreateFile(
				strPortFile, 
				GENERIC_READ | GENERIC_WRITE, 
				0, NULL, OPEN_EXISTING, 0, NULL);
			if (hPort != INVALID_HANDLE_VALUE)
			{
				bPortExist = true;
				VERIFY(::CloseHandle(hPort));
			}
		}
		if (bPortExist)
		{
			CEnumDevInfo Info;
			Info.m_nStatus = INFO_NAME | INFO_PORT_NAME;
			Info.m_nPortNum = i;
			Info.m_strPort = Info.m_strName = strPortName;
			Info.m_strService = _T("Serial");
			pCallBack(pList, &Info);
			++nPorts;
		}
	}
}
#endif
