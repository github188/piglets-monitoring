
// MainProgram_GUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
//#include "otlv4.h"

// CMainProgram_GUIApp:
// �йش����ʵ�֣������ MainProgram_GUI.cpp
//

class CMainProgram_GUIApp : public CWinApp
{
public:
	CMainProgram_GUIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};
extern otl_connect g_db;
extern CCriticalSection g_cs_db;
extern CMainProgram_GUIApp theApp;

//////////////////////////////////////////////////////////////////////////�Զ��庯��//////
typedef struct _APPCONFIG{
	int nSeconds;
	int nMinRecord;
	double dLinger;
	double dRun;
	double dRest;
}App_Config;
extern App_Config g_app_config;
bool G_ReadAppConfigFromDB(App_Config &appconfig);//�����ݿ��ж�ȡ����������Ϣ
bool G_WriteAppConfigToDB(const App_Config &appconfig);//���ó���������Ϣ
//////////////////////////////////////////////////////////////////////////////////////////