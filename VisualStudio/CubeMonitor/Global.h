#pragma once


int AppendToLogAndScroll(CRichEditCtrl* pCtrl, CString str, COLORREF color);
int GetNumVisibleLines(CRichEditCtrl* pCtrl);

void SendCommand(CSerial &serial, const unsigned char cmd);
int RecvCommand(CSerial &serial, const unsigned char cmd, OUT unsigned char buffer[], const int maxLen);
