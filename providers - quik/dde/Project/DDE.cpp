#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <ddeml.h>

//���������� ����������
HSZ hszService;
HSZ hszTopic;
HSZ hszItem;
int g_count = 0;
DWORD idInst = 0;
LPCWSTR szTopic = (LPCWSTR)L"[�����1]������� ������� ����������"; //�������� �������

HDDEDATA EXPENTRY DdeCallback(UINT type, UINT fmt, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if(DdeInitialize(&idInst, DdeCallback, APPCLASS_STANDARD, 0)) {
		MessageBox(NULL, L"DdeInitialize() failed", L"Error", MB_SETFOREGROUND);
		return -1;
	}
	//��� �������� ������������� ������� �������������� ����� ��� �������, ������� � �������� ������
	hszService = DdeCreateStringHandle(idInst, L"EXCEL", NULL);
	hszTopic = DdeCreateStringHandle(idInst, szTopic, NULL);
	hszItem = DdeCreateStringHandle(idInst, L"MyItem", NULL);

	if( (hszService == 0) || (hszTopic == 0) || (hszItem == 0) ) {
		MessageBox(NULL, L"�������� ��������������� ����� ����������� ��������", L"Error", MB_SETFOREGROUND);
		return -2;
	}
	//����������� DDE �������
	if(!DdeNameService(idInst, hszService, NULL, DNS_REGISTER)) {
		MessageBox(NULL, L"����������� DDE ������� ��������!", L"Error", MB_SETFOREGROUND);
		return -3;
	}
	//������� ������ ��� �������� ��������� ������ ...
	SetTimer(0,0,1,0);

	//���� ��������� ���������:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))  {
		// �� WM_TIMER, �������� ��� �������, � ��������� ������� ...
		if(msg.message == WM_TIMER) {
			g_count++;
			DdePostAdvise(idInst, hszTopic, hszItem);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//������� ��������� ������ DDE
HDDEDATA EXPENTRY DdeCallback(UINT wType, UINT fmt, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2) {
	switch (wType) {
		case XTYP_CONNECT:
			// ������ �������� ������������.
			if ((!DdeCmpStringHandles(hsz1, hszTopic)) &&
				(!DdeCmpStringHandles(hsz2, hszService)))
				return (HDDEDATA)TRUE;
			else
				return FALSE;
		case XTYP_ADVSTART:
			if((!DdeCmpStringHandles(hsz1, hszTopic)) &&
				(!DdeCmpStringHandles(hsz2, hszItem)))
				return (HDDEDATA)TRUE; 
			else
				return FALSE;
		case XTYP_ADVREQ:
			if(!DdeCmpStringHandles(hsz1, hszTopic) &&
				!DdeCmpStringHandles(hsz2, hszItem)) {
					short xltableData[100];
					xltableData[0] = 0x0010; 
					xltableData[1] = 4;
					xltableData[2] = 1;
					xltableData[3] = 1;
					xltableData[4] = 0x0006;
					xltableData[5] = 2;
					xltableData[6] = (short)g_count;
					return DdeCreateDataHandle(idInst, (UCHAR*)xltableData, 2*7, 0, hszItem, fmt, 0);
			}    
		default:
			return (HDDEDATA)NULL;
	}
}
