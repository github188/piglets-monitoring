// MainProgram_GUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainProgram_GUI.h"
#include "MainProgram_GUIDlg.h"
#include "../MainProgram_GUI/cxlog.h"
#include "afxdialogex.h"
#include "DlgDVRConfig.h"
#include "DlgAppConfig.h"
#include "DlgAnalyse.h"
#include "atlconv.h"
#include <sstream>
#include <numeric>
#pragma comment(lib,"psapi.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "JHCVideo/Filevideo.h"
#include "JHCVideo/DHDvr.h"
#include "../MainProgram/MBBTrack.h"
#define AVIPLAYTIMER 1
#define DHDVRPLAYTIMER 1
#define WM_DISPLAY_PIGINFO (WM_USER+100)
#define WM_DISPLAY_EVEN    (WM_USER+101)
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogSK
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogSK(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogSK)
END_MESSAGE_MAP()


// CMainProgram_GUIDlg �Ի���



enum BTN_ID{
	BTN_ID_MIN=0,
	BTN_ID_MAX=1,
	BTN_ID_CLOSE=2,
	BTN_ID_OPENAVI=3,
	BTN_ID_OPENDVR=4,
	BTN_ID_CONFIG=5
};
CMainProgram_GUIDlg::CMainProgram_GUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogSK(CMainProgram_GUIDlg::IDD, pParent)
	,m_Sizechange_flag(false)
	,m_Multiple_width(0)
	,m_Multiple_height(0)
	,m_nDlgHeight(0)
	,m_nDlgWidth(0)
	,m_nHeight(0)
	,m_nWidth(0)
	,m_video(NULL)
	,FilePlayTimer(0)
	,m_track(NULL)
	,m_pIPThread(NULL)
	,m_pEDThread(NULL)
	,m_threadType(NULLTHREAD)
	,m_isInitFindPigParam(false)
	,m_dFreq(0),m_dSecondTime(0),m_dFirstTime(0)
	,m_AnalyDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_titlefont.CreatePointFont(105,_T("΢���ź�"));
	m_sowcenter=cvPoint2D32f(-1,-1);//��ʼ��Ϊ����
	m_track=new MBBTrack();

	LARGE_INTEGER litmp;
	if (!QueryPerformanceFrequency(&litmp)){
		LOG1(_T("���ص��Բ�֧�־�ȷ��ʱ��:%d\r\n"),litmp.QuadPart);
	}
	m_dFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ�� 
}

void CMainProgram_GUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainProgram_GUIDlg, CDialogSK)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(BTN_MIN, &CMainProgram_GUIDlg::OnBnClickedMin)
	ON_BN_CLICKED(BTN_MAX, &CMainProgram_GUIDlg::OnBnClickedMax)
	ON_BN_CLICKED(BTN_CLOSE, &CMainProgram_GUIDlg::OnBnClickedClose)
	ON_BN_CLICKED(BTN_OPENAVI, &CMainProgram_GUIDlg::OnBnClickedOpenavi)
	ON_BN_CLICKED(BTN_OPENDVR, &CMainProgram_GUIDlg::OnBnClickedOpendvr)
	ON_WM_TIMER()
	ON_MESSAGE(WM_DISPLAY_PIGINFO,&CMainProgram_GUIDlg::OnDisplayPigInfo)
	ON_MESSAGE(WM_DISPLAY_EVEN,&CMainProgram_GUIDlg::OnDisplayEven)
	ON_BN_CLICKED(BTN_CONFIG, &CMainProgram_GUIDlg::OnBnClickedConfig)
	ON_NOTIFY(NM_DBLCLK, LIST_PIGLET, &CMainProgram_GUIDlg::OnNMDblclkPiglet)
END_MESSAGE_MAP()

// CMainProgram_GUIDlg ��Ϣ�������
// �Զ��崰��
void CMainProgram_GUIDlg::InitWindowsBG()
{
	//������������ʾ�����Title��Ϣ
	CString str;
	this->GetDlgItemText(STATIC_TITLE,str);
	SetWindowText(str);
	EnableEasyMove(1);                       // �����ƶ�
	SetBitmap(_T("res/bg.bmp"));              // ���ñ���λͼ
	SetStyle(LO_STRETCH);                   // ����ͼƬ���ʺ϶Ի����С
	SetTransparentColor(RGB(0, 255, 0));     // ����͸��ɫΪ��ɫ
	CRect   temprect(0,0,972,752);			 // ���ô���ĳ�ʼ����С
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(), SWP_NOZORDER|SWP_NOMOVE);
}

void CMainProgram_GUIDlg::InitButtonCtrl()
{
	// ��С����󻯹رհ�ť�ػ�
	m_Btn[BTN_ID_MIN].AutoLoad(BTN_MIN, this); 
	m_Btn[BTN_ID_MIN].LoadBitmaps(IDB_MIN,IDB_MIN_CLICK); 
	m_Btn[BTN_ID_MIN].RedrawWindow();
	m_Btn[BTN_ID_MIN].SizeToContent();
	m_Btn[BTN_ID_MAX].AutoLoad(BTN_MAX, this); 
	m_Btn[BTN_ID_MAX].LoadBitmaps(IDB_MAX,IDB_MAX_CLICK); 
	m_Btn[BTN_ID_MAX].RedrawWindow();
	m_Btn[BTN_ID_MAX].SizeToContent();
	m_Btn[BTN_ID_CLOSE].AutoLoad(BTN_CLOSE, this); 
	m_Btn[BTN_ID_CLOSE].LoadBitmaps(IDB_CLOSE,IDB_CLOSE_CLICK); 
	m_Btn[BTN_ID_CLOSE].RedrawWindow();
	m_Btn[BTN_ID_CLOSE].SizeToContent();
	m_Btn[BTN_ID_OPENAVI].AutoLoad(BTN_OPENAVI, this); 
	m_Btn[BTN_ID_OPENAVI].LoadBitmaps(IDB_OPENAVI,IDB_OPENAVI_CLICK,IDB_OPENAVI,IDB_OPENAVI_DIS); 
	m_Btn[BTN_ID_OPENAVI].RedrawWindow();
	m_Btn[BTN_ID_OPENAVI].SizeToContent();
	m_Btn[BTN_ID_OPENDVR].AutoLoad(BTN_OPENDVR, this); 
	m_Btn[BTN_ID_OPENDVR].LoadBitmaps(IDB_OPENDVR,IDB_OPENDVR_CLICK,IDB_OPENDVR,IDB_OPENDVR_DIS); 
	m_Btn[BTN_ID_OPENDVR].RedrawWindow();
	m_Btn[BTN_ID_OPENDVR].SizeToContent();
	m_Btn[BTN_ID_CONFIG].AutoLoad(BTN_CONFIG, this); 
	m_Btn[BTN_ID_CONFIG].LoadBitmaps(IDB_CONFIG,IDB_CONFIG_CLICK,IDB_CONFIG,IDB_CONFIG_DIS); 
	m_Btn[BTN_ID_CONFIG].RedrawWindow();
	m_Btn[BTN_ID_CONFIG].SizeToContent();
}

void CMainProgram_GUIDlg::InitListCtrl()
{
	CRect listrc;
	const int nwidth=60;
	// ������ϢList�ؼ��ĳ�ʼ��
	CListCtrl *pList=(CListCtrl*)GetDlgItem(LIST_PIGLET);
	pList->GetWindowRect(listrc);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_LABELTIP); 
	pList->InsertColumn(0,_T("��ֻ���"),LVCFMT_CENTER,nwidth);
	pList->InsertColumn(1,_T("���ĵ�"),LVCFMT_CENTER,(listrc.Width()-nwidth)/6);
	pList->InsertColumn(2,_T("λ��"),LVCFMT_CENTER,(listrc.Width()-nwidth)/4);
	pList->InsertColumn(3,_T("���"),LVCFMT_CENTER,(listrc.Width()-nwidth)/6 );
	pList->InsertColumn(4,_T("�ܳ�"),LVCFMT_CENTER,(listrc.Width()-nwidth)/6 );
	pList->InsertColumn(5,_T("�ٶ�"),LVCFMT_CENTER,0 );
	pList->InsertColumn(6,_T("��ĸ�����"),LVCFMT_CENTER,(listrc.Width()-nwidth)/6 );
	// �¼���ϢList�ؼ��ĳ�ʼ��
	pList=(CListCtrl*)GetDlgItem(LIST_EVEN);
	pList->GetWindowRect(listrc);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_LABELTIP); 
	pList->InsertColumn(0,_T("ʱ��"),LVCFMT_CENTER,(listrc.Width()-nwidth)*0.4);
	pList->InsertColumn(1,_T("��ֻ���"),LVCFMT_CENTER,nwidth);
	pList->InsertColumn(2,_T("�¼�����"),LVCFMT_CENTER,0);
	pList->InsertColumn(3,_T("��ϸ����"),LVCFMT_CENTER,(listrc.Width()-nwidth)*0.6);
}
BOOL CMainProgram_GUIDlg::OnInitDialog()
{
	LOG(_T("��ʼ��������\r\n"));
	CDialogSK::OnInitDialog();
	InitWindowsBG();
	InitButtonCtrl();
	InitialResize();// ��ʼ�����塢�ؼ�����Ӧ�仯
	InitListCtrl();
	this->doStartEventDetectThread();//�����¼�����߳�
	LOG(_T("����������ʼ��\r\n"));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
bool CMainProgram_GUIDlg::InitFindPigParam(IplImage *img)
{
	/////�����ֻ
	string ImgPath=".\\res\\";
	if (ImgPath.at(ImgPath.length()-1)!='\\')
	{
		ImgPath+='\\';
	}

	string LUPName;
	LUPName=ImgPath+"ĸ��Y_CrCbSMinMaxIndex���ұ�.dat";
	int row,col;
	int *table=0;
	CreateLUT(LUPName,&table,row,col);

	CvGaussBGStatModelParams params;
	params.win_size      = CV_BGFG_MOG_WINDOW_SIZE;
	params.bg_threshold  = CV_BGFG_MOG_BACKGROUND_THRESHOLD;
	params.std_threshold = CV_BGFG_MOG_STD_THRESHOLD;
	params.weight_init   = CV_BGFG_MOG_WEIGHT_INIT;
	params.variance_init = CV_BGFG_MOG_SIGMA_INIT*CV_BGFG_MOG_SIGMA_INIT;
	params.minArea       = CV_BGFG_MOG_MINAREA*10;
	params.n_gauss       = CV_BGFG_MOG_NGAUSSIANS-2;

	CvGaussBGModel *m_bg_model = (CvGaussBGModel*)cvCreateGaussianBGModel(img,&params );
	CvMemStorage *storage=cvCreateMemStorage();
	m_userdata.bg_model=m_bg_model;
	m_userdata.storage=storage;
	m_userdata.LUPTable=table;
	m_userdata.rows=row;
	m_userdata.cols=col;
	m_userdata.modelframes=0;
	m_userdata.isRemoveShadow=true;
	m_isInitFindPigParam=true;
	return true;
}
void CMainProgram_GUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogSK::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainProgram_GUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogSK::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMainProgram_GUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CMainProgram_GUIDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogSK::OnCtlColor(pDC, pWnd, nCtlColor);

	enum STATIC_BKCOLOR
	{
		NULL_COLOR=RGB(255,255,255),
		TITLE_COLOR=RGB(16,10,245),
		STATIC_COLOR=RGB(75,101,168),
		EDIT_COLOR=RGB(207,48,159),
		LIST_COLOR=RGB(255,0,0),
		GROUPUP_COLOR=RGB(171,197,232),
		GROUPDOWN_COLOR=RGB(97,134,183),
	};

	STATIC_BKCOLOR static_BkColor = NULL_COLOR;
	HBRUSH return_hbr = hbr;

	switch (pWnd->GetDlgCtrlID())
	{
	case STATIC_TITLE:
	case STATIC_VERSION:
		static_BkColor = TITLE_COLOR;
		break;
	//case STATIC_RESULT_FRAME:
	//case STATIC_RAW_FRAME:
	case STATIC_POSITION:
	case STATIC_CENTER:
		static_BkColor = STATIC_COLOR;
		break;
		break;
	case EDIT_INFO:
	case EDIT_CENTER:
	case EDIT_POSITION:
		static_BkColor=EDIT_COLOR;
		break;
	case LIST_PIGLET:
	case LIST_EVEN:
		static_BkColor=LIST_COLOR;
		break;
	case GROUP_EVEN:
		static_BkColor=GROUPDOWN_COLOR;
		break;
	case GROUP_PIGLET:
	case GROUP_VIDEO:
		static_BkColor=GROUPUP_COLOR;
		break;
	default:
		static_BkColor = NULL_COLOR;
		break;
	}

	switch (static_BkColor)
	{
	case TITLE_COLOR:
		pDC->SetTextColor(static_BkColor);
		pDC->SelectObject(&m_titlefont);
		pDC->SetBkMode(TRANSPARENT);
		return_hbr = (HBRUSH)GetStockObject(NULL_PEN);
		break;
	case STATIC_COLOR:
		pDC->SetTextColor(static_BkColor);
		pDC->SelectObject(&m_titlefont);
		pDC->SetBkMode(TRANSPARENT);
		return_hbr = (HBRUSH)GetStockObject(NULL_PEN);
		break;
	case EDIT_COLOR:
		pDC-> SetBkMode(TRANSPARENT);
		pDC->SetTextColor(static_BkColor);
		pDC->SetBkColor(RGB(255,255,255)); 
		return_hbr =hbr;// (HBRUSH)GetStockObject(NULL_PEN);
		break;
	case LIST_COLOR:
		pDC->SetTextColor(static_BkColor);
		return_hbr = (HBRUSH)GetStockObject(NULL_PEN);
		break;
	case GROUPUP_COLOR:
		pDC->SetTextColor(RGB(89,127,255));
		pDC->SetBkColor(static_BkColor); 
		return_hbr = (HBRUSH)GetStockObject(NULL_PEN);
		break;
	case GROUPDOWN_COLOR:
		pDC->SetTextColor(RGB(173,200,255));
		pDC->SetBkColor(static_BkColor); 
		return_hbr = (HBRUSH)GetStockObject(NULL_PEN);
		break;
	case NULL_COLOR:
		return_hbr = hbr;
		break;
	default:
		return_hbr = hbr;
	}
	return return_hbr;
}
// ��ʼ�����塢�ؼ�����Ӧ�仯
void CMainProgram_GUIDlg::InitialResize()
{
	// ��ȡ�������С
	CRect rect; 
	::GetWindowRect(m_hWnd,rect); 
	ScreenToClient(rect); 
	m_nDlgWidth = rect.right - rect.left; 
	m_nDlgHeight = rect.bottom - rect.top;

	// ��ȡ��Ļ�ֱ���
	m_nWidth = GetSystemMetrics(SM_CXSCREEN); 
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);

	// ����Ŵ���
	m_Multiple_width = float(m_nWidth)/float(m_nDlgWidth); 
	m_Multiple_height = float(m_nHeight)/float(m_nDlgHeight); 
	m_Sizechange_flag=true;	// �ж�ִ��ʱoninitdlg�Ƿ��Ѿ�ִ����
}

void CMainProgram_GUIDlg::ReSize(int nID,bool nZoom)
{
	CRect Rect; 
	if( GetDlgItem(nID)->GetSafeHwnd() )  
	{
		GetDlgItem(nID)->GetWindowRect(Rect); 
		ScreenToClient(Rect); 
		//����ؼ����Ͻǵ� 
		CPoint OldTLPoint,TLPoint; 
		OldTLPoint = Rect.TopLeft(); 
		TLPoint.x = long(OldTLPoint.x *m_Multiple_width); 
		TLPoint.y = long(OldTLPoint.y * m_Multiple_height );

		//����ؼ����½ǵ�
		CPoint OldBRPoint,BRPoint; 
		OldBRPoint = Rect.BottomRight(); 
		if(nZoom)
		{
			BRPoint.x = long(OldBRPoint.x *m_Multiple_width); 
			BRPoint.y = long(OldBRPoint.y *m_Multiple_height); 
		}
		else
		{
			BRPoint.x = long(TLPoint.x + Rect.Width()); 
			BRPoint.y = long(TLPoint.y + Rect.Height()); 
		}
		//�ƶ��ؼ����¾���
		Rect.SetRect(TLPoint,BRPoint); 
		GetDlgItem(nID)->MoveWindow(Rect,false); 
	}
}

void CMainProgram_GUIDlg::OnSize(UINT nType, int cx, int cy)
{
	if (m_Sizechange_flag)//���ȷ��OninitDlg�Ѿ��������.
	{
		//���Ŵ����С�仯�����ؼ��Ĵ�С��λ��
		ReSize(BTN_MIN);
		ReSize(BTN_MAX);
		ReSize(BTN_CLOSE);
		ReSize(BTN_OPENAVI);
		ReSize(BTN_OPENDVR);
		ReSize(BTN_CONFIG);

		ReSize(EDIT_INFO,true);
		ReSize(EDIT_POSITION,true);
		ReSize(EDIT_CENTER,true);

		ReSize(STATIC_RAW_FRAME,true);
		ReSize(STATIC_RESULT_FRAME,true);
		ReSize(STATIC_POSITION,true);
		ReSize(STATIC_CENTER,true);
		ReSize(STATIC_TITLE);
		ReSize(STATIC_VERSION);

		ReSize(LIST_PIGLET,true);
		ReSize(LIST_EVEN,true);

		ReSize(GROUP_EVEN,true);
		ReSize(GROUP_PIGLET,true);
		ReSize(GROUP_VIDEO,true);
		m_Multiple_width = float(1)/ m_Multiple_width ; 
		m_Multiple_height = float(1)/m_Multiple_height ; 
	}
	CDialogSK::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

void CMainProgram_GUIDlg::OnDestroy()
{
	CDialogSK::OnDestroy();
	LOG(_T("������ر�,��ʼ�����ڴ�\r\n"));
	//�ж�ͼ�����߳�
	if(m_pIPThread){
		m_cs_ImgPro.Lock();
		DWORD exitcode;
		if(GetExitCodeThread(m_pIPThread->m_hThread,&exitcode)){
			if (exitcode==STILL_ACTIVE){
				TerminateThread(m_pIPThread->m_hThread,0);
				delete m_pIPThread;
				m_pIPThread=NULL;
			}
		}else{
			DWORD nErrorNo = GetLastError ( ); // �õ��������
			LPTSTR lpBuffer;    
			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
				FORMAT_MESSAGE_IGNORE_INSERTS  | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				nErrorNo, // ���˴�����룬ͨ���ڳ����п��� GetLastError()��֮
				LANG_NEUTRAL,
				(LPTSTR) & lpBuffer,
				0 ,
				NULL );
			CString strErrorCause  =  lpBuffer  ?  lpBuffer : _T( " Sorry, cannot find this error info. " ); 
			//  Free the buffer. 
			LocalFree (lpBuffer);
			AfxMessageBox(strErrorCause);
		}
		delete m_pIPThread;
		m_pIPThread=NULL;
		m_cs_ImgPro.Unlock();
	}
	//�ж��¼�����߳�
	if(m_pEDThread){
		m_cs_Evendete.Lock();
		DWORD exitcode;
		if(GetExitCodeThread(m_pEDThread->m_hThread,&exitcode)){
			if (exitcode==STILL_ACTIVE){
				TerminateThread(m_pEDThread->m_hThread,0);
				delete m_pEDThread;
				m_pEDThread=NULL;
			}
		}else{
			DWORD nErrorNo = GetLastError ( ); // �õ��������
			LPTSTR lpBuffer;    
			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
				FORMAT_MESSAGE_IGNORE_INSERTS  | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				nErrorNo, // ���˴�����룬ͨ���ڳ����п��� GetLastError()��֮
				LANG_NEUTRAL,
				(LPTSTR) & lpBuffer,
				0 ,
				NULL );
			CString strErrorCause  =  lpBuffer  ?  lpBuffer : _T( " Sorry, cannot find this error info. " ); 
			//  Free the buffer. 
			LocalFree (lpBuffer);
			AfxMessageBox(strErrorCause);
		}
		delete m_pEDThread;
		m_pEDThread=NULL;
		m_cs_Evendete.Unlock();
	}
	m_titlefont.DeleteObject();
	//�ͷ���Ƶ��
	if(m_video){
		delete m_video;
		m_video=NULL;
	}
	//�ͷŸ�����
	if(m_track){
		delete m_track;
		m_track=NULL;
	}
	//�ͷŷ���������Դ
	if(m_AnalyDlg){
		m_AnalyDlg->DestroyWindow();
		delete m_AnalyDlg;
		m_AnalyDlg=NULL;
	}
	KillTimer(this->FilePlayTimer);
	LOG(_T("������ر�,�����ڴ����\r\n"));
	// TODO: �ڴ˴������Ϣ����������
}

void CMainProgram_GUIDlg::OnBnClickedMin()
{
	ShowWindow(SW_SHOWMINIMIZED);
}

void CMainProgram_GUIDlg::OnBnClickedMax()
{
	if(IsZoomed())
	{
		EnableEasyMove(1);
		ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		EnableEasyMove(0);
		ShowWindow(SW_SHOWMAXIMIZED);
	}
}

void CMainProgram_GUIDlg::OnBnClickedClose()
{
	OnCancel();
}

//����ͼ���̣߳�������Ŀ���⡢Ŀ����١���ͼ������Ϣ����������ʾ
UINT IPThreadProc( LPVOID pParam )
{
	LOG(_T("����ͼ�����߳�\r\n"));
	CMainProgram_GUIDlg* pObject = (CMainProgram_GUIDlg*)pParam;
	if (pObject == NULL){// || !pObject->IsKindOf(RUNTIME_CLASS(CMainProgram_GUIDlg)))
		LOG(_T("����ͼ�����߳�\r\n"));
		return 1;   // if CMainProgram_GUIDlg is not valid
	}
	while(pObject->m_threadType!=NULLTHREAD){
		Sleep(10);
		pObject->m_cs_ImgPro.Lock();
		if (VideoDevice::GetQueueSize()>0){
			IplImage *img=VideoDevice::GetFrontImgQueue();
			if(img && pObject->m_track){
				IplImage *blobImg=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
				IplImage *drawImg=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
				pObject->SetImage(img,STATIC_RAW_FRAME);
				pObject->m_track->GetBlobsImg(FindPigBlobs,img,blobImg,&(pObject->m_userdata));
				//�����ݽ��з���������ʾ�ڿؼ���
				pObject->m_track->UpdateTrack(0.6,20);
				if (pObject->GetDuration()>1.5)
				{//����1��
					pObject->RestoreTime();
					// ���������������߳�
					::PostMessage(pObject->GetSafeHwnd(),WM_DISPLAY_PIGINFO,NULL,NULL);
				}
				cvCopyImage(img,drawImg);
				pObject->m_track->DrawTrack(drawImg,CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX/* | CV_TRACK_RENDER_TO_CXLOG*/);		
				pObject->SetImage(drawImg,STATIC_RESULT_FRAME);
				cvReleaseImage(&blobImg);
				cvReleaseImage(&drawImg);
				VideoDevice::PopImgQueue();
			}
		}else{
			pObject->m_threadType=NULLTHREAD;//�����������߳�
		}
		pObject->m_cs_ImgPro.Unlock();
	}
	LOG(_T("����ͼ�����߳�\r\n"));
	return 0;   // thread completed successfully
}

//����¼��̣߳�ÿ��������ݿ��е����ݣ�����Ƿ����¼�������
//�¼���������Ϣ�����ܡ��ǻ�������ĸ���ܼ�ѹ�ȵ�
UINT EventDetectingThreadProc(LPVOID pParam)
{
	LOG(_T("�����¼�����߳�\r\n"));
	CMainProgram_GUIDlg* pObject = (CMainProgram_GUIDlg*)pParam;
	if (pObject == NULL || pObject->m_track==NULL){// || !pObject->IsKindOf(RUNTIME_CLASS(CMainProgram_GUIDlg)))
		LOG(_T("�����¼�����߳�\r\n"));
		return 1;   // if CMainProgram_GUIDlg is not valid
	}
	const int N=g_app_config.nSeconds;
	while(true){
		Sleep(10000);//10����һ��
		g_cs_db.Lock();
		bool isColse=true;//�Ƿ񿿽�ĸ��
		try{
			const PhataTrackContainter *pTracks=pObject->m_track->GetCurTracks();
			for (int i=0; i<pTracks->GetContainterSize(); ++i)
			{
				CTrack tmpTrack=(pTracks->GetTrack(i));
				if(tmpTrack.GetState()!=TRACKSTATE_ACTIVE || tmpTrack.m_lifetime<5 || tmpTrack.GetID()==1) continue;
				std::stringstream stream;  
				stream<<"SELECT ptx, pty, x, y, width, height, area, perimeter, speed, distance FROM ";
				stream<<tmpTrack.GetID()<<"  WHERE datediff('s',thetime,now()) <"<<N<<" order by TheTime";//���N���ڵ�����
				string strSql=stream.str();
				otl_stream dbcon;
				try{
					dbcon.open(50,strSql.c_str(),g_db);
					if(dbcon.eof()){//������
						continue;
					}
				}catch(otl_exception& p){
					//��������,��������
					continue;
				}
				vector<CMainProgram_GUIDlg::Pig_Action_Info> piginfoArray;
				CMainProgram_GUIDlg::Pig_Action_Info piginfo;
				while (!dbcon.eof()){
					dbcon>>piginfo.nInfo[0]>>piginfo.nInfo[1]>>piginfo.nInfo[2]>>piginfo.nInfo[3];
					dbcon>>piginfo.nInfo[4]>>piginfo.nInfo[5]>>piginfo.dInfo[0]>>piginfo.dInfo[1];
					dbcon>>piginfo.dInfo[2]>>piginfo.dInfo[3];
					piginfo.id=tmpTrack.GetID();
					piginfoArray.push_back(piginfo);
					if (piginfoArray.size()>1 && piginfo.dInfo[3]>piginfoArray.at(piginfoArray.size()-2).dInfo[3])
					{
						isColse=false;
					}
				}
				if(piginfoArray.size()<g_app_config.nMinRecord){//����֡��̫�٣���ʱ������
					continue;
				}
				double dsum=0,davg=0,dLen=0;
				CMainProgram_GUIDlg::Pig_Action_Info firstpiginfo=piginfoArray.at(0);//��һ֡����
				vector<double> disVec;
				for (vector<CMainProgram_GUIDlg::Pig_Action_Info>::iterator it=piginfoArray.begin()+1; it!=piginfoArray.end(); ++it)
				{
					CMainProgram_GUIDlg::Pig_Action_Info& tmpPiginfo=(*it);
					//ÿ֡���ĵ�仯���
					dLen=sqrt((firstpiginfo.nInfo[0]-tmpPiginfo.nInfo[0])*(firstpiginfo.nInfo[0]-tmpPiginfo.nInfo[0])*1.+
						(firstpiginfo.nInfo[1]-tmpPiginfo.nInfo[1])*(firstpiginfo.nInfo[1]-tmpPiginfo.nInfo[1]));
					firstpiginfo=tmpPiginfo;
					disVec.push_back(dLen);
				}
				dsum=std::accumulate(disVec.begin(),disVec.end(),0.);//���
				davg=dsum/(disVec.size()-1);//��ƽ��
				EVENTYPE eventype=EVEN_NORMAL;
				if (dsum<g_app_config.dRest)//��Ϣ��ǰN��λ��һֱ����
				{
					eventype=EVEN_REST;
				}else if (dsum<g_app_config.dLinger)//�ǻ���ǰN��λ�ñ仯����
				{
					eventype=EVEN_LINGER;
				}else if (dsum>g_app_config.dRun)//���ܣ�ǰN��λ�ñ仯��
				{
					eventype=EVEN_RUN;
				}
				// ���¼�����������߳�
				::PostMessage(pObject->GetSafeHwnd(),WM_DISPLAY_EVEN,(WPARAM)eventype,(LPARAM)tmpTrack.GetID());
				//����ĸ��ǰN����ĸ��ľ���Խ��ԽС
				if (isColse)
				{
					::PostMessage(pObject->GetSafeHwnd(),WM_DISPLAY_EVEN,(WPARAM)EVEN_CLOSE,(LPARAM)tmpTrack.GetID());
				}
				//�ܼ�ѹ��ǰN�뿿��ĸ����ʧ
			}
		}catch (otl_exception& p)
		{
			CString strErr,strStmText,strVarInfo;
			strStmText=p.stm_text;strVarInfo=p.var_info;
			strErr.Format(_T("%s\r\n%s\r\n%s\r\n%s"),p.msg,strStmText.GetBuffer(),p.sqlstate,strVarInfo.GetBuffer());
			MessageBox(NULL,strErr,_T("�����ݿ��ȡ��ֻ��Ϣ����"),MB_OK);
		}
		g_cs_db.Unlock();
	}
	LOG(_T("�����¼�����߳�\r\n"));
	return 1;
}

bool CMainProgram_GUIDlg::doStartEventDetectThread()
{
	//�����߳�
	m_cs_Evendete.Lock();
	DWORD exitcode;
	if(m_pEDThread){
		if(GetExitCodeThread(m_pEDThread->m_hThread,&exitcode)){
			if (exitcode==STILL_ACTIVE){
				TerminateThread(m_pEDThread->m_hThread,0);
				delete m_pEDThread; //��ʽɾ���̶߳���
				m_pEDThread=NULL;
			}
			m_pEDThread=AfxBeginThread(EventDetectingThreadProc,this,THREAD_PRIORITY_BELOW_NORMAL);
			//m_pIPThread->m_bAutoDelete=true;//����Ϊtrue,��AfxEndThread�ڶ�����Ϊtrue�̲߳Ż��Զ�ɾ��
			m_pEDThread->m_bAutoDelete=FALSE; //������ȷ���߳���ֹ���̶߳����Զ����٣� ���Ը����߳̾������ȡ�̵߳ķ���ֵ�� 
		}else{
			DWORD nErrorNo = GetLastError ( ); // �õ��������
			LPTSTR lpBuffer;    
			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
				FORMAT_MESSAGE_IGNORE_INSERTS  | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				nErrorNo, // ���˴�����룬ͨ���ڳ����п��� GetLastError()��֮
				LANG_NEUTRAL,
				(LPTSTR) & lpBuffer,
				0 ,
				NULL );
			CString strErrorCause  =  lpBuffer  ?  lpBuffer : _T( " Sorry, cannot find this error info. " ); 
			//  Free the buffer. 
			LocalFree (lpBuffer);
			AfxMessageBox(strErrorCause);
			m_cs_Evendete.Unlock();
			return false;
		}
	}else{
		m_pEDThread=AfxBeginThread(EventDetectingThreadProc,this,THREAD_PRIORITY_BELOW_NORMAL);
		//m_pIPThread->m_bAutoDelete=true;//����Ϊtrue,��AfxEndThread�ڶ�����Ϊtrue�̲߳Ż��Զ�ɾ��
		m_pEDThread->m_bAutoDelete=FALSE; //������ȷ���߳���ֹ���̶߳����Զ����٣� ���Ը����߳̾������ȡ�̵߳ķ���ֵ�� 
	}
	m_cs_Evendete.Unlock();
	return true;
}

bool CMainProgram_GUIDlg::doOpenDevice(DVRConfig_Info dvr,CString filepath/*=_T("")*/)
{
	m_cs_ImgPro.Lock();
	if (m_threadType==AVITHREAD)
	{
		if(m_video)delete m_video;
		m_video=new Filevideo();
	}else if(m_threadType==DVRTHREAD){
		if(m_video)delete m_video;
		m_video=new DHDvr();
	}
	//���ò���
	m_video->SetDeviceInfo(dvr.ipadd,dvr.port,dvr.username,dvr.pwd);
	if(!m_video->Init(GetDlgItem(STATIC_RAW_FRAME)->GetSafeHwnd())){
		AfxMessageBox(_T("��ʼ��ʧ��"));
		m_cs_ImgPro.Unlock();
		return false;
	}
	m_video->GetAllChannelName();
	if (m_video->GetChannelNum()<=0){
		AfxMessageBox(_T("���豸"));
		m_cs_ImgPro.Unlock();
		return false;
	}
	if(!m_video->Open(dvr.nchannel,filepath.GetBuffer(filepath.GetLength()))){
		AfxMessageBox(_T("���豸ʧ��"));
		m_cs_ImgPro.Unlock();
		return false;
	}

	IplImage *pFrame=m_video->QueryFrame();
	if(m_track) delete m_track;
	m_track=new MBBTrack();
	m_track->DoInit(pFrame);
	if(m_isInitFindPigParam){
		ReleaseLUT(&m_userdata.LUPTable);
		cvReleaseMemStorage(&m_userdata.storage);
		cvReleaseBGStatModel((CvBGStatModel**)&m_userdata.bg_model);
		m_isInitFindPigParam=false;
	}
	InitFindPigParam(pFrame);
	//�����߳�
	m_dFirstTime=0;
	DWORD exitcode;
	if(m_pIPThread){
		if(GetExitCodeThread(m_pIPThread->m_hThread,&exitcode)){
			if (exitcode==STILL_ACTIVE){
				TerminateThread(m_pIPThread->m_hThread,0);
				delete m_pIPThread; //��ʽɾ���̶߳���
				m_pIPThread=NULL;
			}
			LARGE_INTEGER litmp;
			QueryPerformanceCounter(&litmp); 
			m_dFirstTime = (double)litmp.QuadPart/ m_dFreq;//��ʼ��ʱ��
			m_pIPThread=AfxBeginThread(IPThreadProc,this,THREAD_PRIORITY_BELOW_NORMAL);
			if(!m_pIPThread){
				//m_pIPThread->m_bAutoDelete=true;//����Ϊtrue,��AfxEndThread�ڶ�����Ϊtrue�̲߳Ż��Զ�ɾ��
				m_pIPThread->m_bAutoDelete=FALSE; //������ȷ���߳���ֹ���̶߳����Զ����٣� ���Ը����߳̾������ȡ�̵߳ķ���ֵ�� 
				LOG(_T("����ͼ�����߳�\r\n"));
			}
		}else{
			DWORD nErrorNo = GetLastError ( ); // �õ��������
			LPTSTR lpBuffer;    
			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
				FORMAT_MESSAGE_IGNORE_INSERTS  | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				nErrorNo, // ���˴�����룬ͨ���ڳ����п��� GetLastError()��֮
				LANG_NEUTRAL,
				(LPTSTR) & lpBuffer,
				0 ,
				NULL );
			CString strErrorCause  =  lpBuffer  ?  lpBuffer : _T( " Sorry, cannot find this error info. " ); 
			//  Free the buffer. 
			LocalFree (lpBuffer);
			AfxMessageBox(strErrorCause);
			m_cs_ImgPro.Unlock();
			return false;
		}
	}else{
		LARGE_INTEGER litmp;
		QueryPerformanceCounter(&litmp); 
		m_dFirstTime = (double)litmp.QuadPart/ m_dFreq;//��ʼ��ʱ��
		m_pIPThread=AfxBeginThread(IPThreadProc,this,THREAD_PRIORITY_BELOW_NORMAL);
		if(!m_pIPThread){
			//m_pIPThread->m_bAutoDelete=true;//����Ϊtrue,��AfxEndThread�ڶ�����Ϊtrue�̲߳Ż��Զ�ɾ��
			m_pIPThread->m_bAutoDelete=FALSE; //������ȷ���߳���ֹ���̶߳����Զ����٣� ���Ը����߳̾������ȡ�̵߳ķ���ֵ�� 
			LOG(_T("����ͼ�����߳�\r\n"));
		}
	}
	m_cs_ImgPro.Unlock();
	return true;
}

void CMainProgram_GUIDlg::OnBnClickedOpenavi()
{
	CFileDialog fileopen(true,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("AVI Files (*.avi)|*.avi|All Files (*.*)|*.*||"));
	if (fileopen.DoModal()==IDOK)
	{
		CWaitCursor waitcursor;
		CString filepath=fileopen.GetPathName();
		LOG1(_T("����Ƶ�ļ�\"%s\"\r\n"),filepath.GetBuffer());
		m_threadType=AVITHREAD;
		//���ò���
		FilePlayTimer=SetTimer(AVIPLAYTIMER,1,NULL);
		DVRConfig_Info dvr_info;
		if(doOpenDevice(dvr_info,filepath)){
			CString strVideoInfo;
			strVideoInfo=_T("��Ƶ�ļ�·��Ϊ\"")+filepath+_T("\"");
			SetDlgItemText(EDIT_INFO,strVideoInfo);
		}
	}
}

void CMainProgram_GUIDlg::OnBnClickedOpendvr()
{
	CDlgDVRConfig config;
	if(IDOK==config.DoModal()){
		CWaitCursor waitcursor;
		DVRConfig_Info dvr_info=config.GetDVRConfigInfo();
		m_threadType=DVRTHREAD;
		//���ò���
		KillTimer(FilePlayTimer);
		if(doOpenDevice(dvr_info)){
			CString tmp;
			tmp=dvr_info.ipadd.c_str();
			CString strVideoInfo;
			strVideoInfo.Format(_T("IP��ַ��%s\r\n�˿ڣ�%d\r\nͨ����%d\r\n"),tmp.GetBuffer(),
				dvr_info.port,dvr_info.nchannel);
			SetDlgItemText(EDIT_INFO,strVideoInfo);
		}
	}
}

BOOL CMainProgram_GUIDlg::PreTranslateMessage(MSG* pMsg)
{
	//��������Ӧ�س���/ESC��
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN /*|| pMsg->wParam== VK_ESCAPE*/))
	{
		return TRUE;
	}
	return CDialogSK::PreTranslateMessage(pMsg);
}

void CMainProgram_GUIDlg::SetImage(IplImage* img,int nCtrlID)
{
	CRect rc;
	CWnd* pWnd=GetDlgItem(nCtrlID);
	pWnd->GetClientRect(rc);
	VideoDevice::DrawImgToHDC(pWnd->GetWindowDC()->GetSafeHdc(),rc,img);
}

void CMainProgram_GUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent==AVIPLAYTIMER && m_video){
		if(m_video->IsUsingCallBack()==false){
			m_video->QueryFrame();
		}
	}
	CDialogSK::OnTimer(nIDEvent);
}

LRESULT CMainProgram_GUIDlg::OnDisplayPigInfo( WPARAM wParam,LPARAM lParam )
{
	m_cs_ImgPro.Lock();
	const PhataTrackContainter *pTracks=m_track->GetCurTracks();
	CvPoint2D32f CurSowCenter=cvPoint2D32f(-1,-1);//ĸ��λ��
	for (int i=0; i<pTracks->GetContainterSize(); ++i)
	{
		CTrack tmpTrack=(pTracks->GetTrack(i));
		if(!(tmpTrack.GetState()==TRACKSTATE_ACTIVE && tmpTrack.m_lifetime>5)) continue;
		CvRect rc=tmpTrack.GetBoundingBox();
		if (tmpTrack.GetID()==1)
		{
			CurSowCenter.x=rc.x+rc.width/2;
			CurSowCenter.y=rc.y+rc.height/2;
			CString cs;
			cs.Format(_T("%d"),tmpTrack.GetID());
			SetDlgItemText(EDIT_CENTER,cs);
			cs.Format(_T("(%d,%d)"),rc.x+rc.width/2,rc.y+rc.height/2);
			SetDlgItemText(EDIT_POSITION,cs);
			break;
		}
		if (CurSowCenter.x<0 && CurSowCenter.y<0)
		{//ĸ����ʧ,ʹ��ǰһ֡�����ĵ�
			CurSowCenter.x=m_sowcenter.x;
			CurSowCenter.y=m_sowcenter.y;
		}else{//���浱ǰ֡�����ĵ�
			m_sowcenter.x=CurSowCenter.x;
			m_sowcenter.y=CurSowCenter.y;
		}
	}
	for (int i=0; i<pTracks->GetContainterSize(); ++i)
	{
		CTrack tmpTrack=(pTracks->GetTrack(i));
		if(!(tmpTrack.GetState()==TRACKSTATE_ACTIVE && tmpTrack.m_lifetime>5)) continue;
		CvRect rc=tmpTrack.GetBoundingBox();
		CStringArray csArray;
		CString cs;
		cs.Format(_T("%d"),tmpTrack.GetID());
		csArray.Add(cs);
		cs.Format(_T("(%d,%d)"),rc.x+rc.width/2,rc.y+rc.height/2);
		csArray.Add(cs);
		cs.Format(_T("(%d,%d,%d,%d)"),rc.x,rc.y,rc.width,rc.height);
		csArray.Add(cs);
		cs.Format(_T("%.2f"),tmpTrack.Area());
		csArray.Add(cs);
		cs.Format(_T("%.2f"),tmpTrack.Perimeter());
		csArray.Add(cs);

		csArray.Add(cs);//�ٶ�,��ʱ����

		double dis=sqrt((rc.x+rc.width/2-CurSowCenter.x)*(rc.x+rc.width/2-CurSowCenter.x)+
			(rc.y+rc.height/2-CurSowCenter.y)*(rc.y+rc.height/2-CurSowCenter.y));//��ĸ��ľ���
		cs.Format(_T("%.2f"),dis);
		csArray.Add(cs);

		insertPigletList(csArray);

		Pig_Action_Info piginfo;
		piginfo.id=tmpTrack.GetID();
		piginfo.nInfo[0]=rc.x+rc.width/2;
		piginfo.nInfo[1]=rc.y+rc.height/2;
		piginfo.nInfo[2]=rc.x;
		piginfo.nInfo[3]=rc.y;
		piginfo.nInfo[4]=rc.width;
		piginfo.nInfo[5]=rc.height;
		piginfo.dInfo[0]=tmpTrack.Area();
		piginfo.dInfo[1]=tmpTrack.Perimeter();
		piginfo.dInfo[2]=dis;
		piginfo.dInfo[3]=dis;
		insertPigletInfoToDB(piginfo);
		if (tmpTrack.GetID()==1)
		{
			SetDlgItemText(EDIT_CENTER,csArray.GetAt(1));
			SetDlgItemText(EDIT_POSITION,csArray.GetAt(2));
		}
	}
	m_cs_ImgPro.Unlock();
	return 0;
}

LRESULT CMainProgram_GUIDlg::OnDisplayEven( WPARAM wParam,LPARAM lParam )
{
	m_cs_Evendete.Lock();
	EVENTYPE eventype=(EVENTYPE)wParam;
	Even_Info eveninfo;
	eveninfo.pigno=(int)lParam;
	eveninfo.type=eventype;
	switch(eventype){
	case EVEN_CLOSE:
		eveninfo.des="����ĸ����";
		break;
	case EVEN_CRUSH:
		eveninfo.des="�ܼ�ѹ";
		break;
	case EVEN_LINGER:
		eveninfo.des="�ǻ���";
		break;
	case EVEN_NORMAL:
		eveninfo.des="����";
		break;
	case EVEN_REST:
		eveninfo.des="��Ϣ��";
		break;
	case EVEN_RUN:
		eveninfo.des="������";
		break;
	default:
		eveninfo.des="";
		break;
	}
	bool isnewPigID=true;
	for (int i=0; i<m_PiginfoVec.size(); ++i)
	{
		Pig_Info &piginfo=m_PiginfoVec.at(i);
		if (piginfo.id==eveninfo.pigno){
			isnewPigID=false;
			if (piginfo.state!=eveninfo.type){
				if (eveninfo.type==EVEN_NORMAL){
					m_PiginfoVec.at(i).state=eveninfo.type;//�ı�״̬
				}else{
					m_PiginfoVec.at(i).state=eveninfo.type;//�ı�״̬
					CStringArray strArray;
					CString strTmp;
					strArray.Add(CTime::GetTickCount().Format(_T("%Y-%m-%d %H:%M:%S")));
					strTmp.Format(_T("%d"),eveninfo.pigno);
					strArray.Add(strTmp);
					strTmp.Format(_T("%d"),eveninfo.type);
					strArray.Add(strTmp);
					strTmp=eveninfo.des.c_str();
					strArray.Add(strTmp);
					insertEvenList(strArray);
					insertEvenInfoToDB(eveninfo);
				}
			}
			break;//�ҵ����˳�
		}
	}
	m_cs_Evendete.Unlock();
	if (isnewPigID)
	{
		Pig_Info newInfo;
		newInfo.id=eveninfo.pigno;
		newInfo.state=eveninfo.type;
		m_PiginfoVec.push_back(newInfo);
	}
	return 0;
}

void CMainProgram_GUIDlg::insertPigletList(const CStringArray& csArray)
{//csArray��������Ϊ����ֻ��š���ֻ���ĵ㡢��ֻλ�á���ֻ�������ֻ�ܳ�����ֻ�ٶȡ���ĸ�����
	CListCtrl *pList=(CListCtrl*)GetDlgItem(LIST_PIGLET);
	CString csPigID;
	bool isnewPig=true;
	int nRows=pList->GetItemCount();
	int nCols=pList->GetHeaderCtrl()->GetItemCount();
	assert(nCols==csArray.GetCount());
	for (int i=0; i<nRows; ++i)
	{
		csPigID=pList->GetItemText(i,0);
		if (csPigID.CompareNoCase(csArray.GetAt(0))==0)//��ͬ����ID�Ѵ���
		{
			isnewPig=false;
			for (int j=1; j<nCols; ++j)
			{
				pList->SetItemText(i,j, csArray.GetAt(j)); 
			}
			break;//�ҵ����˳�for,����ѭ��
		}
	}
	if (isnewPig==true)//��Ŀ��
	{
		pList->InsertItem(0, csArray.GetAt(0)); //�豸ID
		for (int j=1; j<nCols; ++j)
		{
			pList->SetItemText(0,j, csArray.GetAt(j)); 
		}
	}
	pList->RedrawWindow();
}

void CMainProgram_GUIDlg::insertEvenList(const CStringArray& csArray)
{//csArray��������Ϊ��ʱ�䡢��ֻ��š��¼����͡���ϸ����
	CListCtrl *pList=(CListCtrl*)GetDlgItem(LIST_EVEN);
	CString csPigID;
	bool isnewEven=true;
	int nRows=pList->GetItemCount();
	int nCols=pList->GetHeaderCtrl()->GetItemCount();
	assert(nCols==csArray.GetCount());
	for (int i=0; i<nRows; ++i)
	{
		csPigID=pList->GetItemText(i,0);
		if (csPigID.CompareNoCase(csArray.GetAt(0))==0)//��ͬ����ID�Ѵ���
		{
			isnewEven=false;
			for (int j=1; j<nCols; ++j)
			{
				pList->SetItemText(i,j, csArray.GetAt(j)); 
			}
			break;//�ҵ����˳�for,����ѭ��
		}
	}
	if (isnewEven==true)//��Ŀ��
	{
		pList->InsertItem(0, csArray.GetAt(0)); //�豸ID
		for (int j=1; j<nCols; ++j)
		{
			pList->SetItemText(0,j, csArray.GetAt(j)); 
		}
	}
	pList->RedrawWindow();
}

bool CMainProgram_GUIDlg::insertPigletInfoToDB(const Pig_Action_Info& piginfo)
{//piginfo��������Ϊ����ֻ��š���ֻ���ĵ㡢��ֻλ�á���ֻ�������ֻ�ܳ�����ֻ�ٶȡ���ĸ�����
	try{
		g_cs_db.Lock();
		otl_stream dbcon;//��ȡ��������
		string strSql;
		std::stringstream stream;  
		string  strID;  
		stream<<piginfo.id;  
		strID =stream.str();
//		����Access���ݿⱾ��ġ�����Ա����û�з������������Ȩ�ޣ������Ҫ������ -> ѡ�� -> ��ͼ -> ���ض���ϵͳ����ǰ��Ĺ�ѡ�ϡ� 
//Ȼ�󣬹��� -> ��ȫ -> �û������Ȩ�ޣ��ڶ���������ѡ�� MSysObjects����ֻ�õ�������������Ҫ�õ������ģ�������ѡ�� ��
//Ȼ��Ȩ�������á���ȡ���ݡ�Ȩ�ޡ� 
		strSql="select * from MSysObjects WHERE [name]='"+strID+"' and [flags]=0 AND [Type]=1";//�ж�access���ݿ��е�ĳ�����Ƿ����
		dbcon.open(50,strSql.c_str(),g_db);
		if(dbcon.eof()){
			//�����ڣ��½���
			strSql="create table "+strID+" (TheTime datetime primary key,ptx integer not null";
			strSql+=",pty integer not null,x integer not null,y integer not null,width integer not null";
			strSql+=",height integer not null,area double,perimeter double,speed double,distance double)";
			otl_cursor::direct_exec(g_db,strSql.c_str());  // create table
		}
		//������¼����
		strSql="insert into "+strID+"(TheTime,ptx,pty,x,y,width,height,area,perimeter,speed,distance)";
		strSql+=" values(now(),:f1<int>,:f2<int>,:f3<int>,:f4<int>,:f5<int>,:f6<int>";
		strSql+=" ,:f7<double>,:f8<double>,:f9<double>,:f10<double>)";
		otl_stream o(50,strSql.c_str(),g_db);
		o<<piginfo.nInfo[0]<<piginfo.nInfo[1]<<piginfo.nInfo[2]<<piginfo.nInfo[3]<<piginfo.nInfo[4]<<piginfo.nInfo[5];
		o<<piginfo.dInfo[0]<<piginfo.dInfo[1]<<piginfo.dInfo[2]<<piginfo.dInfo[3];
		g_cs_db.Unlock();
		return true;
	}
	catch (otl_exception& p)
	{
		//CString strErr;
		//strErr.Format(_T("%s\r\n%s\r\n%s\r\n%s"),p.msg,p.stm_text,p.sqlstate,p.var_info);
		//MessageBox(strErr,_T("������ֻ��Ϣ����"),MB_OK);
		//��������,������.һ����������ͻ
		g_cs_db.Unlock();
		return false;
	}
}

bool CMainProgram_GUIDlg::insertEvenInfoToDB(const Even_Info& eveninfo)//����⵽���¼���Ϣ��ӽ����ݿ�
{//eveninfo��������Ϊ����ֻ��š��¼����͡��¼���ϸ����
		try{
			g_cs_db.Lock();
			otl_stream dbcon;//��ȡ��������
			string strSql;
			//		����Access���ݿⱾ��ġ�����Ա����û�з������������Ȩ�ޣ������Ҫ������ -> ѡ�� -> ��ͼ -> ���ض���ϵͳ����ǰ��Ĺ�ѡ�ϡ� 
			//Ȼ�󣬹��� -> ��ȫ -> �û������Ȩ�ޣ��ڶ���������ѡ�� MSysObjects����ֻ�õ�������������Ҫ�õ������ģ�������ѡ�� ��
			//Ȼ��Ȩ�������á���ȡ���ݡ�Ȩ�ޡ� 
			strSql="select * from MSysObjects WHERE [name]='Even' and [flags]=0 AND [Type]=1";//�ж�access���ݿ��е�ĳ�����Ƿ����
			dbcon.open(50,strSql.c_str(),g_db);
			if(dbcon.eof()){
				//�����ڣ��½���
				strSql="create table Even(TheTime datetime primary key,PigNo integer not null";
				strSql+=",Type integer not null,Description Text(255))";
				otl_cursor::direct_exec(g_db,strSql.c_str());  // create table
			}
			//������¼����
			strSql="insert into Even(TheTime,PigNo,Type,Description)";
			strSql+=" values(now(),:f1<int>,:f2<int>,:f3<char[256]>)";
			otl_stream o(50,strSql.c_str(),g_db);
			o<<eveninfo.pigno<<eveninfo.type<<eveninfo.des;
			g_cs_db.Unlock();
			return true;
		}
		catch (otl_exception& p)
		{
			//CString strErr;
			//strErr.Format(_T("%s\r\n%s\r\n%s\r\n%s"),p.msg,p.stm_text,p.sqlstate,p.var_info);
			//MessageBox(strErr,_T("�����¼���Ϣ����"),MB_OK);
			g_cs_db.Unlock();
			return false;
		}
}

void CMainProgram_GUIDlg::OnCancel()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(MessageBox(_T("ȷ��Ҫ�˳���? "),_T("�˳�"),MB_OKCANCEL) != IDOK) 
		return; 
	CDialogSK::OnCancel();
}

double CMainProgram_GUIDlg::GetDuration()
{
	LARGE_INTEGER litmp; 
	QueryPerformanceCounter(&litmp); 
	m_dSecondTime = (double)litmp.QuadPart/ m_dFreq;//�����ֵֹ 
	return m_dSecondTime-m_dFirstTime; // ��ö�Ӧ��ʱ��ֵ����λΪ��
}

void CMainProgram_GUIDlg::RestoreTime()
{
	m_dFirstTime=m_dSecondTime;
}

void CMainProgram_GUIDlg::OnBnClickedConfig()
{
	CDlgAppConfig config;
	config.DoModal();
}

void CMainProgram_GUIDlg::OnNMDblclkPiglet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CListCtrl*pListCtrl = (CListCtrl*)GetDlgItem(LIST_PIGLET); 
	int nItem=pNMItemActivate->iItem;
	if((nItem<0)||(nItem>=pListCtrl->GetItemCount()))//ѡ�з�Χ��������
	{
		*pResult = 0;
		return;
	}
	CString csPigID=pListCtrl->GetItemText(nItem,0);
	if (csPigID.CompareNoCase(_T("1"))==0)
	{
		*pResult = 0;
		return;
	}
	string strPigID=CT2A((LPTSTR)(LPCTSTR)csPigID);//CStringתstring
	int nPigID=_ttoi(csPigID.GetBuffer());
	Analy_Struct analyinfo;
	try{
		otl_stream conSelect;//��ȡ����
		std::stringstream stream;  
		stream<<"SELECT ptx, pty, x, y, width, height, area, perimeter, speed, distance FROM ";
		stream<<strPigID<<"  WHERE datediff('s',thetime,now()) <"<<g_app_config.nSeconds*10<<" order by TheTime";//���N���ڵ�����
		string strSql=stream.str();
		conSelect.open(1,strSql.c_str(),g_db);
		CMainProgram_GUIDlg::Pig_Action_Info piginfo;
		while (!conSelect.eof()){
			conSelect>>piginfo.nInfo[0]>>piginfo.nInfo[1]>>piginfo.nInfo[2]>>piginfo.nInfo[3];
			conSelect>>piginfo.nInfo[4]>>piginfo.nInfo[5]>>piginfo.dInfo[0]>>piginfo.dInfo[1];
			conSelect>>piginfo.dInfo[2]>>piginfo.dInfo[3];
			piginfo.id=nPigID;
			analyinfo.piginfoVec.push_back(piginfo);
		}
	}
	catch (otl_exception& p)
	{
		CString strErr,strStmText,strVarInfo;
		strStmText=p.stm_text;strVarInfo=p.var_info;
		strErr.Format(_T("%s\r\n%s\r\n%s\r\n%s"),p.msg,strStmText.GetBuffer(),p.sqlstate,strVarInfo.GetBuffer());
		MessageBox(strErr,_T("��ȡ���ݿ��еĴ������"),MB_OK);
		*pResult = 0;
		return;
	}
	analyinfo.nPigID=nPigID;
	analyinfo.img=cvCloneImage(VideoDevice::GetFrontImgQueue());
	if (m_AnalyDlg){
		m_AnalyDlg->SetAnalyseParam(analyinfo);
	}else{
		m_AnalyDlg=new CDlgAnalyse(analyinfo);
		m_AnalyDlg->Create(IDD_ANALYSE,this);///������ģ̬�Ի���
	}
	cvReleaseImage(&analyinfo.img);
	m_AnalyDlg->ShowWindow(SW_SHOW); ///��ʾ
	*pResult = 0;
}
