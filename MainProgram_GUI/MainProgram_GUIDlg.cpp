// MainProgram_GUIDlg.cpp : 实现文件
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
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogSK
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// CMainProgram_GUIDlg 对话框



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
	m_titlefont.CreatePointFont(105,_T("微软雅黑"));
	m_sowcenter=cvPoint2D32f(-1,-1);//初始化为负的
	m_track=new MBBTrack();

	LARGE_INTEGER litmp;
	if (!QueryPerformanceFrequency(&litmp)){
		LOG1(_T("本地电脑不支持精确定时器:%d\r\n"),litmp.QuadPart);
	}
	m_dFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率 
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

// CMainProgram_GUIDlg 消息处理程序
// 自定义窗体
void CMainProgram_GUIDlg::InitWindowsBG()
{
	//设置任务栏显示窗体的Title信息
	CString str;
	this->GetDlgItemText(STATIC_TITLE,str);
	SetWindowText(str);
	EnableEasyMove(1);                       // 允许移动
	SetBitmap(_T("res/bg.bmp"));              // 设置背景位图
	SetStyle(LO_STRETCH);                   // 伸缩图片以适合对话框大小
	SetTransparentColor(RGB(0, 255, 0));     // 设置透明色为绿色
	CRect   temprect(0,0,972,752);			 // 设置窗体的初始化大小
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(), SWP_NOZORDER|SWP_NOMOVE);
}

void CMainProgram_GUIDlg::InitButtonCtrl()
{
	// 最小化最大化关闭按钮重绘
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
	// 仔猪信息List控件的初始化
	CListCtrl *pList=(CListCtrl*)GetDlgItem(LIST_PIGLET);
	pList->GetWindowRect(listrc);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_LABELTIP); 
	pList->InsertColumn(0,_T("猪只编号"),LVCFMT_CENTER,nwidth);
	pList->InsertColumn(1,_T("中心点"),LVCFMT_CENTER,(listrc.Width()-nwidth)/6);
	pList->InsertColumn(2,_T("位置"),LVCFMT_CENTER,(listrc.Width()-nwidth)/4);
	pList->InsertColumn(3,_T("面积"),LVCFMT_CENTER,(listrc.Width()-nwidth)/6 );
	pList->InsertColumn(4,_T("周长"),LVCFMT_CENTER,(listrc.Width()-nwidth)/6 );
	pList->InsertColumn(5,_T("速度"),LVCFMT_CENTER,0 );
	pList->InsertColumn(6,_T("与母猪距离"),LVCFMT_CENTER,(listrc.Width()-nwidth)/6 );
	// 事件信息List控件的初始化
	pList=(CListCtrl*)GetDlgItem(LIST_EVEN);
	pList->GetWindowRect(listrc);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_LABELTIP); 
	pList->InsertColumn(0,_T("时间"),LVCFMT_CENTER,(listrc.Width()-nwidth)*0.4);
	pList->InsertColumn(1,_T("猪只编号"),LVCFMT_CENTER,nwidth);
	pList->InsertColumn(2,_T("事件类型"),LVCFMT_CENTER,0);
	pList->InsertColumn(3,_T("详细描述"),LVCFMT_CENTER,(listrc.Width()-nwidth)*0.6);
}
BOOL CMainProgram_GUIDlg::OnInitDialog()
{
	LOG(_T("初始化主界面\r\n"));
	CDialogSK::OnInitDialog();
	InitWindowsBG();
	InitButtonCtrl();
	InitialResize();// 初始化窗体、控件自适应变化
	InitListCtrl();
	this->doStartEventDetectThread();//开启事件检测线程
	LOG(_T("完成主界面初始化\r\n"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
bool CMainProgram_GUIDlg::InitFindPigParam(IplImage *img)
{
	/////检测猪只
	string ImgPath=".\\res\\";
	if (ImgPath.at(ImgPath.length()-1)!='\\')
	{
		ImgPath+='\\';
	}

	string LUPName;
	LUPName=ImgPath+"母猪Y_CrCbSMinMaxIndex查找表.dat";
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainProgram_GUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogSK::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
// 初始化窗体、控件自适应变化
void CMainProgram_GUIDlg::InitialResize()
{
	// 获取程序窗体大小
	CRect rect; 
	::GetWindowRect(m_hWnd,rect); 
	ScreenToClient(rect); 
	m_nDlgWidth = rect.right - rect.left; 
	m_nDlgHeight = rect.bottom - rect.top;

	// 获取屏幕分辨率
	m_nWidth = GetSystemMetrics(SM_CXSCREEN); 
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);

	// 计算放大倍数
	m_Multiple_width = float(m_nWidth)/float(m_nDlgWidth); 
	m_Multiple_height = float(m_nHeight)/float(m_nDlgHeight); 
	m_Sizechange_flag=true;	// 判断执行时oninitdlg是否已经执行了
}

void CMainProgram_GUIDlg::ReSize(int nID,bool nZoom)
{
	CRect Rect; 
	if( GetDlgItem(nID)->GetSafeHwnd() )  
	{
		GetDlgItem(nID)->GetWindowRect(Rect); 
		ScreenToClient(Rect); 
		//计算控件左上角点 
		CPoint OldTLPoint,TLPoint; 
		OldTLPoint = Rect.TopLeft(); 
		TLPoint.x = long(OldTLPoint.x *m_Multiple_width); 
		TLPoint.y = long(OldTLPoint.y * m_Multiple_height );

		//计算控件右下角点
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
		//移动控件到新矩形
		Rect.SetRect(TLPoint,BRPoint); 
		GetDlgItem(nID)->MoveWindow(Rect,false); 
	}
}

void CMainProgram_GUIDlg::OnSize(UINT nType, int cx, int cy)
{
	if (m_Sizechange_flag)//如果确定OninitDlg已经调用完毕.
	{
		//随着窗体大小变化调整控件的大小及位置
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

	// TODO: 在此处添加消息处理程序代码
}

void CMainProgram_GUIDlg::OnDestroy()
{
	CDialogSK::OnDestroy();
	LOG(_T("主界面关闭,开始回收内存\r\n"));
	//中断图像处理线程
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
			DWORD nErrorNo = GetLastError ( ); // 得到错误代码
			LPTSTR lpBuffer;    
			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
				FORMAT_MESSAGE_IGNORE_INSERTS  | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				nErrorNo, // 此乃错误代码，通常在程序中可由 GetLastError()得之
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
	//中断事件检测线程
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
			DWORD nErrorNo = GetLastError ( ); // 得到错误代码
			LPTSTR lpBuffer;    
			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
				FORMAT_MESSAGE_IGNORE_INSERTS  | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				nErrorNo, // 此乃错误代码，通常在程序中可由 GetLastError()得之
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
	//释放视频流
	if(m_video){
		delete m_video;
		m_video=NULL;
	}
	//释放跟踪器
	if(m_track){
		delete m_track;
		m_track=NULL;
	}
	//释放分析窗口资源
	if(m_AnalyDlg){
		m_AnalyDlg->DestroyWindow();
		delete m_AnalyDlg;
		m_AnalyDlg=NULL;
	}
	KillTimer(this->FilePlayTimer);
	LOG(_T("主界面关闭,回收内存完成\r\n"));
	// TODO: 在此处添加消息处理程序代码
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

//处理图像线程，包括：目标检测、目标跟踪、画图，发消息给主窗体显示
UINT IPThreadProc( LPVOID pParam )
{
	LOG(_T("启动图像处理线程\r\n"));
	CMainProgram_GUIDlg* pObject = (CMainProgram_GUIDlg*)pParam;
	if (pObject == NULL){// || !pObject->IsKindOf(RUNTIME_CLASS(CMainProgram_GUIDlg)))
		LOG(_T("结束图像处理线程\r\n"));
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
				//对数据进行分析，并显示在控件上
				pObject->m_track->UpdateTrack(0.6,20);
				if (pObject->GetDuration()>1.5)
				{//经过1秒
					pObject->RestoreTime();
					// 将计算结果发给主线程
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
			pObject->m_threadType=NULLTHREAD;//播放完后结束线程
		}
		pObject->m_cs_ImgPro.Unlock();
	}
	LOG(_T("结束图像处理线程\r\n"));
	return 0;   // thread completed successfully
}

//检测事件线程：每秒分析数据库中的数据，检测是否有事件发生。
//事件包括：休息、奔跑、徘徊、靠近母猪、受挤压等等
UINT EventDetectingThreadProc(LPVOID pParam)
{
	LOG(_T("启动事件检测线程\r\n"));
	CMainProgram_GUIDlg* pObject = (CMainProgram_GUIDlg*)pParam;
	if (pObject == NULL || pObject->m_track==NULL){// || !pObject->IsKindOf(RUNTIME_CLASS(CMainProgram_GUIDlg)))
		LOG(_T("结束事件检测线程\r\n"));
		return 1;   // if CMainProgram_GUIDlg is not valid
	}
	const int N=g_app_config.nSeconds;
	while(true){
		Sleep(10000);//10秒检测一次
		g_cs_db.Lock();
		bool isColse=true;//是否靠近母猪
		try{
			const PhataTrackContainter *pTracks=pObject->m_track->GetCurTracks();
			for (int i=0; i<pTracks->GetContainterSize(); ++i)
			{
				CTrack tmpTrack=(pTracks->GetTrack(i));
				if(tmpTrack.GetState()!=TRACKSTATE_ACTIVE || tmpTrack.m_lifetime<5 || tmpTrack.GetID()==1) continue;
				std::stringstream stream;  
				stream<<"SELECT ptx, pty, x, y, width, height, area, perimeter, speed, distance FROM ";
				stream<<tmpTrack.GetID()<<"  WHERE datediff('s',thetime,now()) <"<<N<<" order by TheTime";//获得N秒内的数据
				string strSql=stream.str();
				otl_stream dbcon;
				try{
					dbcon.open(50,strSql.c_str(),g_db);
					if(dbcon.eof()){//不存在
						continue;
					}
				}catch(otl_exception& p){
					//出错不处理,继续运行
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
				if(piginfoArray.size()<g_app_config.nMinRecord){//存在帧数太少，暂时不分析
					continue;
				}
				double dsum=0,davg=0,dLen=0;
				CMainProgram_GUIDlg::Pig_Action_Info firstpiginfo=piginfoArray.at(0);//第一帧数据
				vector<double> disVec;
				for (vector<CMainProgram_GUIDlg::Pig_Action_Info>::iterator it=piginfoArray.begin()+1; it!=piginfoArray.end(); ++it)
				{
					CMainProgram_GUIDlg::Pig_Action_Info& tmpPiginfo=(*it);
					//每帧中心点变化情况
					dLen=sqrt((firstpiginfo.nInfo[0]-tmpPiginfo.nInfo[0])*(firstpiginfo.nInfo[0]-tmpPiginfo.nInfo[0])*1.+
						(firstpiginfo.nInfo[1]-tmpPiginfo.nInfo[1])*(firstpiginfo.nInfo[1]-tmpPiginfo.nInfo[1]));
					firstpiginfo=tmpPiginfo;
					disVec.push_back(dLen);
				}
				dsum=std::accumulate(disVec.begin(),disVec.end(),0.);//求和
				davg=dsum/(disVec.size()-1);//求平均
				EVENTYPE eventype=EVEN_NORMAL;
				if (dsum<g_app_config.dRest)//休息：前N秒位置一直不变
				{
					eventype=EVEN_REST;
				}else if (dsum<g_app_config.dLinger)//徘徊：前N秒位置变化不大
				{
					eventype=EVEN_LINGER;
				}else if (dsum>g_app_config.dRun)//奔跑：前N秒位置变化大
				{
					eventype=EVEN_RUN;
				}
				// 将事件结果发给主线程
				::PostMessage(pObject->GetSafeHwnd(),WM_DISPLAY_EVEN,(WPARAM)eventype,(LPARAM)tmpTrack.GetID());
				//靠近母猪：前N秒与母猪的距离越来越小
				if (isColse)
				{
					::PostMessage(pObject->GetSafeHwnd(),WM_DISPLAY_EVEN,(WPARAM)EVEN_CLOSE,(LPARAM)tmpTrack.GetID());
				}
				//受挤压：前N秒靠近母猪，消失
			}
		}catch (otl_exception& p)
		{
			CString strErr,strStmText,strVarInfo;
			strStmText=p.stm_text;strVarInfo=p.var_info;
			strErr.Format(_T("%s\r\n%s\r\n%s\r\n%s"),p.msg,strStmText.GetBuffer(),p.sqlstate,strVarInfo.GetBuffer());
			MessageBox(NULL,strErr,_T("从数据库获取猪只信息出错"),MB_OK);
		}
		g_cs_db.Unlock();
	}
	LOG(_T("结束事件检测线程\r\n"));
	return 1;
}

bool CMainProgram_GUIDlg::doStartEventDetectThread()
{
	//开启线程
	m_cs_Evendete.Lock();
	DWORD exitcode;
	if(m_pEDThread){
		if(GetExitCodeThread(m_pEDThread->m_hThread,&exitcode)){
			if (exitcode==STILL_ACTIVE){
				TerminateThread(m_pEDThread->m_hThread,0);
				delete m_pEDThread; //显式删除线程对象
				m_pEDThread=NULL;
			}
			m_pEDThread=AfxBeginThread(EventDetectingThreadProc,this,THREAD_PRIORITY_BELOW_NORMAL);
			//m_pIPThread->m_bAutoDelete=true;//必须为true,且AfxEndThread第二参数为true线程才会自动删除
			m_pEDThread->m_bAutoDelete=FALSE; //这样能确保线程终止后，线程对象不自动销毁， 可以根据线程句柄来获取线程的返回值。 
		}else{
			DWORD nErrorNo = GetLastError ( ); // 得到错误代码
			LPTSTR lpBuffer;    
			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
				FORMAT_MESSAGE_IGNORE_INSERTS  | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				nErrorNo, // 此乃错误代码，通常在程序中可由 GetLastError()得之
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
		//m_pIPThread->m_bAutoDelete=true;//必须为true,且AfxEndThread第二参数为true线程才会自动删除
		m_pEDThread->m_bAutoDelete=FALSE; //这样能确保线程终止后，线程对象不自动销毁， 可以根据线程句柄来获取线程的返回值。 
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
	//调用步骤
	m_video->SetDeviceInfo(dvr.ipadd,dvr.port,dvr.username,dvr.pwd);
	if(!m_video->Init(GetDlgItem(STATIC_RAW_FRAME)->GetSafeHwnd())){
		AfxMessageBox(_T("初始化失败"));
		m_cs_ImgPro.Unlock();
		return false;
	}
	m_video->GetAllChannelName();
	if (m_video->GetChannelNum()<=0){
		AfxMessageBox(_T("无设备"));
		m_cs_ImgPro.Unlock();
		return false;
	}
	if(!m_video->Open(dvr.nchannel,filepath.GetBuffer(filepath.GetLength()))){
		AfxMessageBox(_T("打开设备失败"));
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
	//开启线程
	m_dFirstTime=0;
	DWORD exitcode;
	if(m_pIPThread){
		if(GetExitCodeThread(m_pIPThread->m_hThread,&exitcode)){
			if (exitcode==STILL_ACTIVE){
				TerminateThread(m_pIPThread->m_hThread,0);
				delete m_pIPThread; //显式删除线程对象
				m_pIPThread=NULL;
			}
			LARGE_INTEGER litmp;
			QueryPerformanceCounter(&litmp); 
			m_dFirstTime = (double)litmp.QuadPart/ m_dFreq;//初始化时间
			m_pIPThread=AfxBeginThread(IPThreadProc,this,THREAD_PRIORITY_BELOW_NORMAL);
			if(!m_pIPThread){
				//m_pIPThread->m_bAutoDelete=true;//必须为true,且AfxEndThread第二参数为true线程才会自动删除
				m_pIPThread->m_bAutoDelete=FALSE; //这样能确保线程终止后，线程对象不自动销毁， 可以根据线程句柄来获取线程的返回值。 
				LOG(_T("创建图像处理线程\r\n"));
			}
		}else{
			DWORD nErrorNo = GetLastError ( ); // 得到错误代码
			LPTSTR lpBuffer;    
			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
				FORMAT_MESSAGE_IGNORE_INSERTS  | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				nErrorNo, // 此乃错误代码，通常在程序中可由 GetLastError()得之
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
		m_dFirstTime = (double)litmp.QuadPart/ m_dFreq;//初始化时间
		m_pIPThread=AfxBeginThread(IPThreadProc,this,THREAD_PRIORITY_BELOW_NORMAL);
		if(!m_pIPThread){
			//m_pIPThread->m_bAutoDelete=true;//必须为true,且AfxEndThread第二参数为true线程才会自动删除
			m_pIPThread->m_bAutoDelete=FALSE; //这样能确保线程终止后，线程对象不自动销毁， 可以根据线程句柄来获取线程的返回值。 
			LOG(_T("创建图像处理线程\r\n"));
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
		LOG1(_T("打开视频文件\"%s\"\r\n"),filepath.GetBuffer());
		m_threadType=AVITHREAD;
		//调用步骤
		FilePlayTimer=SetTimer(AVIPLAYTIMER,1,NULL);
		DVRConfig_Info dvr_info;
		if(doOpenDevice(dvr_info,filepath)){
			CString strVideoInfo;
			strVideoInfo=_T("视频文件路径为\"")+filepath+_T("\"");
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
		//调用步骤
		KillTimer(FilePlayTimer);
		if(doOpenDevice(dvr_info)){
			CString tmp;
			tmp=dvr_info.ipadd.c_str();
			CString strVideoInfo;
			strVideoInfo.Format(_T("IP地址：%s\r\n端口：%d\r\n通道：%d\r\n"),tmp.GetBuffer(),
				dvr_info.port,dvr_info.nchannel);
			SetDlgItemText(EDIT_INFO,strVideoInfo);
		}
	}
}

BOOL CMainProgram_GUIDlg::PreTranslateMessage(MSG* pMsg)
{
	//屏蔽其响应回车键/ESC键
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	CvPoint2D32f CurSowCenter=cvPoint2D32f(-1,-1);//母猪位置
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
		{//母猪消失,使用前一帧的中心点
			CurSowCenter.x=m_sowcenter.x;
			CurSowCenter.y=m_sowcenter.y;
		}else{//保存当前帧的中心点
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

		csArray.Add(cs);//速度,暂时不用

		double dis=sqrt((rc.x+rc.width/2-CurSowCenter.x)*(rc.x+rc.width/2-CurSowCenter.x)+
			(rc.y+rc.height/2-CurSowCenter.y)*(rc.y+rc.height/2-CurSowCenter.y));//与母猪的距离
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
		eveninfo.des="靠近母猪中";
		break;
	case EVEN_CRUSH:
		eveninfo.des="受挤压";
		break;
	case EVEN_LINGER:
		eveninfo.des="徘徊中";
		break;
	case EVEN_NORMAL:
		eveninfo.des="正常";
		break;
	case EVEN_REST:
		eveninfo.des="休息中";
		break;
	case EVEN_RUN:
		eveninfo.des="奔跑中";
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
					m_PiginfoVec.at(i).state=eveninfo.type;//改变状态
				}else{
					m_PiginfoVec.at(i).state=eveninfo.type;//改变状态
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
			break;//找到就退出
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
{//csArray内容依次为：猪只编号、猪只中心点、猪只位置、猪只面积、猪只周长、猪只速度、与母猪距离
	CListCtrl *pList=(CListCtrl*)GetDlgItem(LIST_PIGLET);
	CString csPigID;
	bool isnewPig=true;
	int nRows=pList->GetItemCount();
	int nCols=pList->GetHeaderCtrl()->GetItemCount();
	assert(nCols==csArray.GetCount());
	for (int i=0; i<nRows; ++i)
	{
		csPigID=pList->GetItemText(i,0);
		if (csPigID.CompareNoCase(csArray.GetAt(0))==0)//相同，此ID已存在
		{
			isnewPig=false;
			for (int j=1; j<nCols; ++j)
			{
				pList->SetItemText(i,j, csArray.GetAt(j)); 
			}
			break;//找到则退出for,不再循环
		}
	}
	if (isnewPig==true)//新目标
	{
		pList->InsertItem(0, csArray.GetAt(0)); //设备ID
		for (int j=1; j<nCols; ++j)
		{
			pList->SetItemText(0,j, csArray.GetAt(j)); 
		}
	}
	pList->RedrawWindow();
}

void CMainProgram_GUIDlg::insertEvenList(const CStringArray& csArray)
{//csArray内容依次为：时间、猪只编号、事件类型、详细描述
	CListCtrl *pList=(CListCtrl*)GetDlgItem(LIST_EVEN);
	CString csPigID;
	bool isnewEven=true;
	int nRows=pList->GetItemCount();
	int nCols=pList->GetHeaderCtrl()->GetItemCount();
	assert(nCols==csArray.GetCount());
	for (int i=0; i<nRows; ++i)
	{
		csPigID=pList->GetItemText(i,0);
		if (csPigID.CompareNoCase(csArray.GetAt(0))==0)//相同，此ID已存在
		{
			isnewEven=false;
			for (int j=1; j<nCols; ++j)
			{
				pList->SetItemText(i,j, csArray.GetAt(j)); 
			}
			break;//找到则退出for,不再循环
		}
	}
	if (isnewEven==true)//新目标
	{
		pList->InsertItem(0, csArray.GetAt(0)); //设备ID
		for (int j=1; j<nCols; ++j)
		{
			pList->SetItemText(0,j, csArray.GetAt(j)); 
		}
	}
	pList->RedrawWindow();
}

bool CMainProgram_GUIDlg::insertPigletInfoToDB(const Pig_Action_Info& piginfo)
{//piginfo内容依次为：猪只编号、猪只中心点、猪只位置、猪只面积、猪只周长、猪只速度、与母猪距离
	try{
		g_cs_db.Lock();
		otl_stream dbcon;//获取查表的连接
		string strSql;
		std::stringstream stream;  
		string  strID;  
		stream<<piginfo.id;  
		strID =stream.str();
//		由于Access数据库本身的“管理员”组没有访问隐含对象的权限，因此先要：工具 -> 选项 -> 视图 -> 隐藏对象、系统对象，前面的勾选上。 
//然后，工具 -> 安全 -> 用户与组的权限，在对象名称中选定 MSysObjects（我只用到这个对象，如果需要用到其它的，可以再选） ，
//然后权限中设置“读取数据”权限。 
		strSql="select * from MSysObjects WHERE [name]='"+strID+"' and [flags]=0 AND [Type]=1";//判断access数据库中的某个表是否存在
		dbcon.open(50,strSql.c_str(),g_db);
		if(dbcon.eof()){
			//不存在，新建表
			strSql="create table "+strID+" (TheTime datetime primary key,ptx integer not null";
			strSql+=",pty integer not null,x integer not null,y integer not null,width integer not null";
			strSql+=",height integer not null,area double,perimeter double,speed double,distance double)";
			otl_cursor::direct_exec(g_db,strSql.c_str());  // create table
		}
		//新增记录进表
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
		//MessageBox(strErr,_T("新增猪只信息出错"),MB_OK);
		//出错算了,不处理.一般是主键冲突
		g_cs_db.Unlock();
		return false;
	}
}

bool CMainProgram_GUIDlg::insertEvenInfoToDB(const Even_Info& eveninfo)//将检测到的事件信息添加进数据库
{//eveninfo内容依次为：猪只编号、事件类型、事件详细描述
		try{
			g_cs_db.Lock();
			otl_stream dbcon;//获取查表的连接
			string strSql;
			//		由于Access数据库本身的“管理员”组没有访问隐含对象的权限，因此先要：工具 -> 选项 -> 视图 -> 隐藏对象、系统对象，前面的勾选上。 
			//然后，工具 -> 安全 -> 用户与组的权限，在对象名称中选定 MSysObjects（我只用到这个对象，如果需要用到其它的，可以再选） ，
			//然后权限中设置“读取数据”权限。 
			strSql="select * from MSysObjects WHERE [name]='Even' and [flags]=0 AND [Type]=1";//判断access数据库中的某个表是否存在
			dbcon.open(50,strSql.c_str(),g_db);
			if(dbcon.eof()){
				//不存在，新建表
				strSql="create table Even(TheTime datetime primary key,PigNo integer not null";
				strSql+=",Type integer not null,Description Text(255))";
				otl_cursor::direct_exec(g_db,strSql.c_str());  // create table
			}
			//新增记录进表
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
			//MessageBox(strErr,_T("新增事件信息出错"),MB_OK);
			g_cs_db.Unlock();
			return false;
		}
}

void CMainProgram_GUIDlg::OnCancel()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(MessageBox(_T("确定要退出吗? "),_T("退出"),MB_OKCANCEL) != IDOK) 
		return; 
	CDialogSK::OnCancel();
}

double CMainProgram_GUIDlg::GetDuration()
{
	LARGE_INTEGER litmp; 
	QueryPerformanceCounter(&litmp); 
	m_dSecondTime = (double)litmp.QuadPart/ m_dFreq;//获得中止值 
	return m_dSecondTime-m_dFirstTime; // 获得对应的时间值，单位为秒
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
	if((nItem<0)||(nItem>=pListCtrl->GetItemCount()))//选中范围不在行上
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
	string strPigID=CT2A((LPTSTR)(LPCTSTR)csPigID);//CString转string
	int nPigID=_ttoi(csPigID.GetBuffer());
	Analy_Struct analyinfo;
	try{
		otl_stream conSelect;//获取连接
		std::stringstream stream;  
		stream<<"SELECT ptx, pty, x, y, width, height, area, perimeter, speed, distance FROM ";
		stream<<strPigID<<"  WHERE datediff('s',thetime,now()) <"<<g_app_config.nSeconds*10<<" order by TheTime";//获得N秒内的数据
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
		MessageBox(strErr,_T("读取数据库中的代码出错"),MB_OK);
		*pResult = 0;
		return;
	}
	analyinfo.nPigID=nPigID;
	analyinfo.img=cvCloneImage(VideoDevice::GetFrontImgQueue());
	if (m_AnalyDlg){
		m_AnalyDlg->SetAnalyseParam(analyinfo);
	}else{
		m_AnalyDlg=new CDlgAnalyse(analyinfo);
		m_AnalyDlg->Create(IDD_ANALYSE,this);///创建非模态对话框
	}
	cvReleaseImage(&analyinfo.img);
	m_AnalyDlg->ShowWindow(SW_SHOW); ///显示
	*pResult = 0;
}
