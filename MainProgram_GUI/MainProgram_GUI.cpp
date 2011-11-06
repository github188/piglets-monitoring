
// MainProgram_GUI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MainProgram_GUI.h"
#include "MainProgram_GUIDlg.h"
#include "../MainProgram_GUI/cxlog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool G_ReadAppConfigFromDB( App_Config &appconfig )
{
	try{
		g_cs_db.Lock();
		otl_stream dbcon;//数据库连接
		string strSql;
		strSql="select top 1 Seconds,MinRecord,dLinger,dRun,dRest from App_Config";
		dbcon.open(50,strSql.c_str(),g_db);
		if(!dbcon.eof()){
			dbcon>>appconfig.nSeconds>>appconfig.nMinRecord;
			dbcon>>appconfig.dLinger>>appconfig.dRun>>appconfig.dRest;
		}else{
			strSql=("insert into App_Config(Seconds,minRecord,dLinger,dRun,dRest) values(10,5,20.,50.,4.)");
			otl_cursor::direct_exec(g_db,strSql.c_str());  // create table
		}
		g_cs_db.Unlock();
		return true;
	}
	catch (otl_exception& p)
	{
		CString strErr,strStmText,strVarInfo;
		strStmText=p.stm_text;strVarInfo=p.var_info;
		strErr.Format(_T("%s\r\n%s\r\n%s\r\n%s"),p.msg,strStmText.GetBuffer(),p.sqlstate,strVarInfo.GetBuffer());
		MessageBox(NULL,strErr,_T("从数据库获取程序配置信息出错"),MB_OK);
		g_cs_db.Unlock();
		return false;
	}
}

bool G_WriteAppConfigToDB( const App_Config &appconfig )
{
	//写入数据库
	g_cs_db.Lock();
	otl_stream conInsert;//获取连接
	try{
		otl_stream conSelect;//获取连接
		string strSql="Select * from App_Config";
		conSelect.open(50,strSql.c_str(),g_db);
		if (!conSelect.eof()){
			strSql=("update App_Config set Seconds=:1<int>,minRecord=:2<int>,dLinger=:3<double>, dRun=:4<double>, dRest=:5<double>");
			conInsert.open(50,strSql.c_str(),g_db);
			conInsert<<appconfig.nSeconds<<appconfig.nMinRecord;
			conInsert<<appconfig.dLinger<<appconfig.dRun<<appconfig.dRest;
		}else{
			strSql=("insert into App_Config(Seconds,minRecord,dLinger,dRun,dRest) ");
			strSql+=(" values(:1<int>,:2<int>,:3<double>,:4<double>,:5<double>)");
			conInsert.open(50,strSql.c_str(),g_db);
			conInsert<<appconfig.nSeconds<<appconfig.nMinRecord;
			conInsert<<appconfig.dLinger<<appconfig.dRun<<appconfig.dRest;
		}
		g_cs_db.Unlock();
		return true;
	}
	catch (otl_exception& p)
	{
		CString strErr,strStmText,strVarInfo;
		strStmText=p.stm_text;strVarInfo=p.var_info;
		strErr.Format(_T("%s\r\n%s\r\n%s\r\n%s"),p.msg,strStmText.GetBuffer(),p.sqlstate,strVarInfo.GetBuffer());
		MessageBox(NULL,strErr,_T("写入数据库出错:程序配置信息"),MB_OK);
		g_cs_db.Unlock();
		return false;
	}
}

// CMainProgram_GUIApp

BEGIN_MESSAGE_MAP(CMainProgram_GUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMainProgram_GUIApp 构造

CMainProgram_GUIApp::CMainProgram_GUIApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMainProgram_GUIApp 对象

CMainProgram_GUIApp theApp;
otl_connect g_db; // connect object
CCriticalSection g_cs_db;
App_Config g_app_config;//程序配置信息
// CMainProgram_GUIApp 初始化
//C语言复制文件函数
bool filecopy(TCHAR *srcFile,TCHAR *dstFile) 
{
	char ch;   
	FILE *fin;
	FILE *fout;   

	if((fin = _tfopen(srcFile,_T("rb+"))) == NULL)   //也可以以"rt"文本方式打开
	{   
		return false;
	}   
	if((fout = _tfopen(dstFile,_T("wb"))) == NULL)   //也可以以"rt"文本方式打开
	{   
		return false;
	}
	ch=fgetc(fin);
	while(!feof(fin))//当feof(FILE   *)读到EOF标志并不认为文件结束了，依旧返回0，
		             //直到读到EOF的下一个字符才返回1，这时才认为是文件结束。 
	{   
		fputc(ch,fout);
		ch=fgetc(fin);
	}
	fclose(fin);   
	fclose(fout);    
	return true; 
}

BOOL CMainProgram_GUIApp::InitInstance()
{
	LOG(_T("启动猪舍监控系统\r\n"));
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("陈吉宏的哺乳仔猪舍监控系统"));
	//数据库连接的初始化
	otl_connect::otl_initialize(); // initialize ODBC environment
	try{
		g_cs_db.Lock();
		char* constr="Driver={Microsoft Access Driver (*.mdb)};Dbq=.\\Feature.mdb;Exclusive=1;Uid=;Pwd=;" ;
		g_db.rlogon(constr); // connect to ODBC
		g_cs_db.Unlock();
	}
	catch(otl_exception& p){ // intercept OTL exceptions
		//CStringA strErr;
		//strErr.Format("%s\r\n%s\r\n%s\r\n%s",p.msg,p.stm_text,p.sqlstate,p.var_info);
		//MessageBoxA(this->GetSafeHwnd(),strErr+"连接数据库出错，请检测数据库是否存在","连接特征库出错",MB_OK);
		CString strErr,strSqlState;
		strSqlState.Format(_T("%s"),p.sqlstate);
		if (p.code==-1024 && strSqlState.CompareNoCase(_T("HY000"))==0)
		{
			strErr=_T("数据库已经被打开,请关闭数据库再运行此程序");
			AfxMessageBox(strErr,MB_OK);
			g_cs_db.Unlock();
			LOG(_T("数据库已经被打开,请关闭数据库再运行此程序\r\n"));
			return false;
		}else if (p.code==-1811 && strSqlState.CompareNoCase(_T("HY000"))==0)
		{
			strErr=_T("数据库不存在,是否加载空数据库?点击\"是\"程序将加载空的数据库，点击\"否\"程序将退出，")
				_T("请手动将先前备份的数据库（若有的话）复制到程序安装目录下，并重命名为\"Feature.mdb\"，然后再启动程序");
			LOG(_T("数据库不存在\r\n"));
		}else{
			strErr=_T("连接数据库出错,是否加载空数据库?点击\"是\"程序将加载空的数据库，点击\"否\"程序将退出，")
				_T("请手动将先前备份的数据库（若有的话）复制到程序安装目录下，并重命名为\"Feature.mdb\"，然后再启动程序");
			LOG(_T("连接数据库出错\r\n"));
		}
		if(IDYES==AfxMessageBox(strErr,MB_YESNO)){//加载空数据库
			LOG(_T("开始加载空数据库。\r\n"));
			//获取当前路径
			TCHAR szFull[_MAX_PATH];
			TCHAR szDrive[_MAX_DRIVE];
			TCHAR szDir[_MAX_DIR];
			::GetModuleFileName(NULL, szFull, sizeof(szFull)/sizeof(TCHAR));
			_tsplitpath(szFull, szDrive, szDir, NULL, NULL);
			_tmakepath(szFull,szDrive,szDir,_T("backup"),NULL);//空的数据库路径
			if(_taccess(szFull,0)==0){//检测文件是否存在
				TCHAR szDst[_MAX_PATH];
				_tmakepath(szDst,szDrive,szDir,_T("Feature"),_T("mdb"));//特征库路径
				if(!filecopy(szFull,szDst)){
					AfxMessageBox(_T("无访问权限，请重启电脑后再试。"),MB_OK);
					g_cs_db.Unlock();
					LOG(_T("无访问权限，请重启电脑后再试。\r\n"));
					return false;
				}else{
					g_cs_db.Unlock();
					g_cs_db.Lock();
					char* constr="Driver={Microsoft Access Driver (*.mdb)};Dbq=.\\Feature.mdb;Exclusive=1;Uid=;Pwd=;" ;
					g_db.rlogon(constr); // connect to ODBC
					g_cs_db.Unlock();
					LOG(_T("完成加载空数据库。\r\n"));
				}
			}else{//找不到空数据库
				AfxMessageBox(_T("程序已损坏，请重新安装应用程序。"),MB_OK);
				LOG(_T("程序已损坏，请重新安装应用程序。\r\n"));
				g_cs_db.Unlock();
				return false;
			}
		}else{
			g_cs_db.Unlock();
			return false;
		}
	}
	////读取程序配置信息
	if(G_ReadAppConfigFromDB(g_app_config)==false){
		return false;
	}
	CMainProgram_GUIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CMainProgram_GUIApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	g_cs_db.Lock();
	g_db.logoff();//断开连接
	g_cs_db.Unlock();
	LOG(_T("退出猪舍监控系统\r\n**************************************************************\r\n\r\n"));
	return CWinApp::ExitInstance();
}
