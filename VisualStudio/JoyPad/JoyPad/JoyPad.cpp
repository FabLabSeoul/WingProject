///////////////////////////////////////////////////////////////////////////////
//
// Raw Input API sample showing joystick support
//
// Author: Alexander B?ken
// Date:   04/22/2011
//
// Copyright 2011 Alexander B?ken
//
///////////////////////////////////////////////////////////////////////////////
// 
// 원본 소스는 다음 사이트에서 참조함.
// http://www.codeproject.com/Articles/185522/Using-the-Raw-Input-API-to-Process-Joystick-Input
//
// 코드정리.
//


#include "stdafx.h"
#include <stdio.h>
#include "JoyPad.h"

#include <Windows.h>
#include <tchar.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <hidsdi.h>
#include "Serial.h"


#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#define WC_MAINFRAME	TEXT("MainFrame")
#define MAX_BUTTONS		128
#define CHECK(exp)		{ if(!(exp)) goto Error; }
#define SAFE_FREE(p)	{ if(p) { HeapFree(hHeap, 0, p); (p) = NULL; } }

//
// Global variables
//

BOOL bButtonStates[MAX_BUTTONS];
LONG lAxisX;
LONG lAxisY;
LONG lAxisZ;
LONG lAxisRz;
LONG lHat;
INT  g_NumberOfButtons;
CSerial g_Serial;


void ParseRawInput(PRAWINPUT pRawInput)
{
	PHIDP_PREPARSED_DATA pPreparsedData;
	HIDP_CAPS            Caps;
	PHIDP_BUTTON_CAPS    pButtonCaps;
	PHIDP_VALUE_CAPS     pValueCaps;
	USHORT               capsLength;
	UINT                 bufferSize;
	HANDLE               hHeap;
	USAGE                usage[MAX_BUTTONS];
	ULONG                i, usageLength, value;

	pPreparsedData = NULL;
	pButtonCaps = NULL;
	pValueCaps = NULL;
	hHeap = GetProcessHeap();

	//
	// Get the preparsed data block
	//

	CHECK(GetRawInputDeviceInfo(pRawInput->header.hDevice, RIDI_PREPARSEDDATA, NULL, &bufferSize) == 0);
	CHECK(pPreparsedData = (PHIDP_PREPARSED_DATA)HeapAlloc(hHeap, 0, bufferSize));
	CHECK((int)GetRawInputDeviceInfo(pRawInput->header.hDevice, RIDI_PREPARSEDDATA, pPreparsedData, &bufferSize) >= 0);

	//
	// Get the joystick's capabilities
	//

	// Button caps
	CHECK(HidP_GetCaps(pPreparsedData, &Caps) == HIDP_STATUS_SUCCESS)
		CHECK(pButtonCaps = (PHIDP_BUTTON_CAPS)HeapAlloc(hHeap, 0, sizeof(HIDP_BUTTON_CAPS) * Caps.NumberInputButtonCaps));

	capsLength = Caps.NumberInputButtonCaps;
	CHECK(HidP_GetButtonCaps(HidP_Input, pButtonCaps, &capsLength, pPreparsedData) == HIDP_STATUS_SUCCESS)
		g_NumberOfButtons = pButtonCaps->Range.UsageMax - pButtonCaps->Range.UsageMin + 1;

	// Value caps
	CHECK(pValueCaps = (PHIDP_VALUE_CAPS)HeapAlloc(hHeap, 0, sizeof(HIDP_VALUE_CAPS) * Caps.NumberInputValueCaps));
	capsLength = Caps.NumberInputValueCaps;
	CHECK(HidP_GetValueCaps(HidP_Input, pValueCaps, &capsLength, pPreparsedData) == HIDP_STATUS_SUCCESS)

		//
		// Get the pressed buttons
		//
		usageLength = g_NumberOfButtons;
	CHECK(
		HidP_GetUsages(
		HidP_Input, pButtonCaps->UsagePage, 0, usage, &usageLength, pPreparsedData,
		(PCHAR)pRawInput->data.hid.bRawData, pRawInput->data.hid.dwSizeHid
		) == HIDP_STATUS_SUCCESS);

	ZeroMemory(bButtonStates, sizeof(bButtonStates));
	for (i = 0; i < usageLength; i++)
		bButtonStates[usage[i] - pButtonCaps->Range.UsageMin] = TRUE;

	//
	// Get the state of discrete-valued-controls
	//

	for (i = 0; i < Caps.NumberInputValueCaps; i++)
	{
		CHECK(
			HidP_GetUsageValue(
			HidP_Input, pValueCaps[i].UsagePage, 0, pValueCaps[i].Range.UsageMin, &value, pPreparsedData,
			(PCHAR)pRawInput->data.hid.bRawData, pRawInput->data.hid.dwSizeHid
			) == HIDP_STATUS_SUCCESS);

		switch (pValueCaps[i].Range.UsageMin)
		{
		case 0x30:	// X-axis
			lAxisX = (LONG)value - 128;
			break;

		case 0x31:	// Y-axis
			lAxisY = (LONG)value - 128;
			break;

		case 0x32: // Z-axis
			lAxisZ = (LONG)value - 128;
			break;

		case 0x35: // Rotate-Z
			lAxisRz = (LONG)value - 128;
			break;

		case 0x39:	// Hat Switch
			lHat = value;
			break;
		}
	}

	//
	// Clean up
	//

Error:
	SAFE_FREE(pPreparsedData);
	SAFE_FREE(pButtonCaps);
	SAFE_FREE(pValueCaps);
}


void DrawButton(HDC hDC, int i, int x, int y, BOOL bPressed)
{
	HBRUSH hOldBrush, hBr;
	TCHAR  sz[4];
	RECT   rc;

	if (bPressed)
	{
		hBr = CreateSolidBrush(RGB(192, 0, 0));
		hOldBrush = (HBRUSH)SelectObject(hDC, hBr);
	}

	rc.left = x;
	rc.top = y;
	rc.right = x + 30;
	rc.bottom = y + 30;
	Ellipse(hDC, rc.left, rc.top, rc.right, rc.bottom);
	//_stprintf(sz, ARRAY_SIZE(sz), TEXT("%d"), i);
	sprintf_s(sz, TEXT("%d"), i);
	DrawText(hDC, sz, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (bPressed)
	{
		SelectObject(hDC, hOldBrush);
		DeleteObject(hBr);
	}
}


void DrawCrosshair(HDC hDC, int x, int y, LONG xVal, LONG yVal)
{
	Rectangle(hDC, x, y, x + 256, y + 256);
	MoveToEx(hDC, x + xVal - 5 + 128, y + yVal + 128, NULL);
	LineTo(hDC, x + xVal + 5 + 128, y + yVal + 128);
	MoveToEx(hDC, x + xVal + 128, y + yVal - 5 + 128, NULL);
	LineTo(hDC, x + xVal + 128, y + yVal + 5 + 128);
}


void DrawDPad(HDC hDC, int x, int y, LONG value)
{
	LONG i;

	for (i = 0; i < 8; i++)
	{
		HBRUSH hOldBrush = NULL;
		HBRUSH hBr = NULL;
		int xPos = (int)(sin(-2 * M_PI * i / 8 + M_PI) * 80.0) + 80;
		int yPos = (int)(cos(2 * M_PI * i / 8 + M_PI) * 80.0) + 80;

		if (value == i)
		{
			hBr = CreateSolidBrush(RGB(192, 0, 0));
			hOldBrush = (HBRUSH)SelectObject(hDC, hBr);
		}

		Ellipse(hDC, x + xPos, y + yPos, x + xPos + 20, y + yPos + 20);

		if (value == i)
		{
			SelectObject(hDC, hOldBrush);
			DeleteObject(hBr);
		}
	}
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		//
		// Register for joystick devices
		//

		RAWINPUTDEVICE rid;

		rid.usUsagePage = 1;
		rid.usUsage = 4;	// Joystick
		rid.dwFlags = 0;
		rid.hwndTarget = hWnd;

		if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
			return -1;

		g_Serial.Open(2, 9600);

	}
	return 0;

	case WM_INPUT:
	{
		//
		// Get the pointer to the raw device data, process it and update the window
		//

		RAWINPUT RawInput;
		ZeroMemory(&RawInput, sizeof(RAWINPUT));
		UINT      bufferSize = sizeof(RawInput);

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &RawInput, &bufferSize, sizeof(RAWINPUTHEADER));
		ParseRawInput(&RawInput);


		// 시리얼통신을 주기적으로 보낸다.
		static int oldT = 0;
		int curT = GetTickCount();
		if (curT - oldT > 300)
		{
			//char buff[5] = { 'S', (char)lAxisX, (char)lAxisY, (char)lAxisZ, (char)lAxisRz };
			char buff[14] = {
				//0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
				0x0, 0x0, 0x0, 0x0, 0x0,
				//0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
				(char)lAxisX, (char)lAxisY, (char)lAxisZ, (char)lAxisRz,
				//0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
				0x0, 0x0, 0x0, 0x0, 0x0,
				//0xaa
			};
//			for (int i = 5; i < 20; ++i)
//				buff[i] = NULL;

			g_Serial.SendData(buff, sizeof(buff));

			oldT = curT;
		}
		//

		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}
	return 0;

	case WM_PAINT:
	{
		//
		// Draw the buttons and axis-values
		//

		PAINTSTRUCT ps;
		HDC         hDC;
		int         i;

		hDC = BeginPaint(hWnd, &ps);
		SetBkMode(hDC, TRANSPARENT);

		for (i = 0; i < g_NumberOfButtons; i++)
			DrawButton(hDC, i + 1, 20 + i * 40, 20, bButtonStates[i]);

		Rectangle(hDC, 10, 90, 10 + 552, 90 + 276);
		DrawCrosshair(hDC, 20, 100, lAxisX, lAxisY);
		DrawCrosshair(hDC, 296, 100, lAxisZ, lAxisRz);
		DrawDPad(hDC, 600, 140, lHat);

		EndPaint(hWnd, &ps);
	}
	return 0;

	case WM_ERASEBKGND:
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wcex;

	//
	// Register window class
	//

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WindowProc;
	wcex.lpszClassName = WC_MAINFRAME;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wcex))
		return -1;

	//
	// Create window
	//

	hWnd = CreateWindow(WC_MAINFRAME, TEXT("Joystick using Raw Input API"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 500,//CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	//
	// Message loop
	//

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

