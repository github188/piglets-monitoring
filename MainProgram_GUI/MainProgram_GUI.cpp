
// MainProgram_GUI.cpp : ����Ӧ�ó��������Ϊ��
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
		otl_stream dbcon;//���ݿ�����
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
		MessageBox(NULL,strErr,_T("�����ݿ��ȡ����������Ϣ����"),MB_OK);
		g_cs_db.Unlock();
		return false;
	}
}

bool G_WriteAppConfigToDB( const App_Config &appconfig )
{
	//д�����ݿ�
	g_cs_db.Lock();
	otl_stream conInsert;//��ȡ����
	try{
		otl_stream conSelect;//��ȡ����
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
		MessageBox(NULL,strErr,_T("д�����ݿ����:����������Ϣ"),MB_OK);
		g_cs_db.Unlock();
		return false;
	}
}

// CMainProgram_GUIApp

BEGIN_MESSAGE_MAP(CMainProgram_GUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMainProgram_GUIApp ����

CMainProgram_GUIApp::CMainProgram_GUIApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMainProgram_GUIApp ����

CMainProgram_GUIApp theApp;
otl_connect g_db; // connect object
CCriticalSection g_cs_db;
App_Config g_app_config;//����������Ϣ
// CMainProgram_GUIApp ��ʼ��
//C���Ը����ļ�����
bool filecopy(TCHAR *srcFile,TCHAR *dstFile) 
{
	char ch;   
	FILE *fin;
	FILE *fout;   

	if((fin = _tfopen(srcFile,_T("rb+"))) == NULL)   //Ҳ������"rt"�ı���ʽ��
	{   
		return false;
	}   
	if((fout = _tfopen(dstFile,_T("wb"))) == NULL)   //Ҳ������"rt"�ı���ʽ��
	{   
		return false;
	}
	ch=fgetc(fin);
	while(!feof(fin))//��feof(FILE   *)����EOF��־������Ϊ�ļ������ˣ����ɷ���0��
		             //ֱ������EOF����һ���ַ��ŷ���1����ʱ����Ϊ���ļ������� 
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
	LOG(_T("����������ϵͳ\r\n"));
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("�¼���Ĳ�����������ϵͳ"));
	//���ݿ����ӵĳ�ʼ��
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
		//MessageBoxA(this->GetSafeHwnd(),strErr+"�������ݿ�����������ݿ��Ƿ����","�������������",MB_OK);
		CString strErr,strSqlState;
		strSqlState.Format(_T("%s"),p.sqlstate);
		if (p.code==-1024 && strSqlState.CompareNoCase(_T("HY000"))==0)
		{
			strErr=_T("���ݿ��Ѿ�����,��ر����ݿ������д˳���");
			AfxMessageBox(strErr,MB_OK);
			g_cs_db.Unlock();
			LOG(_T("���ݿ��Ѿ�����,��ر����ݿ������д˳���\r\n"));
			return false;
		}else if (p.code==-1811 && strSqlState.CompareNoCase(_T("HY000"))==0)
		{
			strErr=_T("���ݿⲻ����,�Ƿ���ؿ����ݿ�?���\"��\"���򽫼��ؿյ����ݿ⣬���\"��\"�����˳���")
				_T("���ֶ�����ǰ���ݵ����ݿ⣨���еĻ������Ƶ�����װĿ¼�£���������Ϊ\"Feature.mdb\"��Ȼ������������");
			LOG(_T("���ݿⲻ����\r\n"));
		}else{
			strErr=_T("�������ݿ����,�Ƿ���ؿ����ݿ�?���\"��\"���򽫼��ؿյ����ݿ⣬���\"��\"�����˳���")
				_T("���ֶ�����ǰ���ݵ����ݿ⣨���еĻ������Ƶ�����װĿ¼�£���������Ϊ\"Feature.mdb\"��Ȼ������������");
			LOG(_T("�������ݿ����\r\n"));
		}
		if(IDYES==AfxMessageBox(strErr,MB_YESNO)){//���ؿ����ݿ�
			LOG(_T("��ʼ���ؿ����ݿ⡣\r\n"));
			//��ȡ��ǰ·��
			TCHAR szFull[_MAX_PATH];
			TCHAR szDrive[_MAX_DRIVE];
			TCHAR szDir[_MAX_DIR];
			::GetModuleFileName(NULL, szFull, sizeof(szFull)/sizeof(TCHAR));
			_tsplitpath(szFull, szDrive, szDir, NULL, NULL);
			_tmakepath(szFull,szDrive,szDir,_T("backup"),NULL);//�յ����ݿ�·��
			if(_taccess(szFull,0)==0){//����ļ��Ƿ����
				TCHAR szDst[_MAX_PATH];
				_tmakepath(szDst,szDrive,szDir,_T("Feature"),_T("mdb"));//������·��
				if(!filecopy(szFull,szDst)){
					AfxMessageBox(_T("�޷���Ȩ�ޣ����������Ժ����ԡ�"),MB_OK);
					g_cs_db.Unlock();
					LOG(_T("�޷���Ȩ�ޣ����������Ժ����ԡ�\r\n"));
					return false;
				}else{
					g_cs_db.Unlock();
					g_cs_db.Lock();
					char* constr="Driver={Microsoft Access Driver (*.mdb)};Dbq=.\\Feature.mdb;Exclusive=1;Uid=;Pwd=;" ;
					g_db.rlogon(constr); // connect to ODBC
					g_cs_db.Unlock();
					LOG(_T("��ɼ��ؿ����ݿ⡣\r\n"));
				}
			}else{//�Ҳ��������ݿ�
				AfxMessageBox(_T("�������𻵣������°�װӦ�ó���"),MB_OK);
				LOG(_T("�������𻵣������°�װӦ�ó���\r\n"));
				g_cs_db.Unlock();
				return false;
			}
		}else{
			g_cs_db.Unlock();
			return false;
		}
	}
	////��ȡ����������Ϣ
	if(G_ReadAppConfigFromDB(g_app_config)==false){
		return false;
	}
	CMainProgram_GUIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CMainProgram_GUIApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	g_cs_db.Lock();
	g_db.logoff();//�Ͽ�����
	g_cs_db.Unlock();
	LOG(_T("�˳�������ϵͳ\r\n**************************************************************\r\n\r\n"));
	return CWinApp::ExitInstance();
}
