
// MainProgram_GUI.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
//#include "otlv4.h"

// CMainProgram_GUIApp:
// 有关此类的实现，请参阅 MainProgram_GUI.cpp
//

class CMainProgram_GUIApp : public CWinApp
{
public:
	CMainProgram_GUIApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};
extern otl_connect g_db;
extern CCriticalSection g_cs_db;
extern CMainProgram_GUIApp theApp;

//////////////////////////////////////////////////////////////////////////自定义函数//////
typedef struct _APPCONFIG{
	int nSeconds;
	int nMinRecord;
	double dLinger;
	double dRun;
	double dRest;
}App_Config;
extern App_Config g_app_config;
bool G_ReadAppConfigFromDB(App_Config &appconfig);//从数据库中读取程序配置信息
bool G_WriteAppConfigToDB(const App_Config &appconfig);//设置程序配置信息
//////////////////////////////////////////////////////////////////////////////////////////