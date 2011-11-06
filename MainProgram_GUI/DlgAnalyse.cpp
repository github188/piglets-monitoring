// DlgDVRConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "MainProgram_GUI.h"
#include "DlgAnalyse.h"
#include "afxdialogex.h"
#include  <algorithm> 
//opencv输出汉字的功能
#include ".\\opencv输出汉字\\CvxText.h"
#pragma comment(lib, ".\\opencv输出汉字\\lib\\freetype235.lib")
// CDlgAnalyse 对话框
enum BTN_ID{
	BTN_ID_MIN=0,
	BTN_ID_MAX=1,
	BTN_ID_CLOSE=2,
	BTN_ID_OK=3
};
const int WIDTH=400;//统计图的宽度
const int HEIGHT=300;//统计图的高度
const int INTERVAL=5;//统计数据间隔
const int MARGIN=40;//坐标轴离图像边缘的距离
CDlgAnalyse::CDlgAnalyse(const Analy_Struct& analyinfo,CWnd* pParent /*=NULL*/)
	: CDialogSK(CDlgAnalyse::IDD, pParent)
	,m_Img(NULL)
	,m_StaticImg(NULL)
	,m_Sizechange_flag(false)
	,m_Multiple_width(0)
	,m_Multiple_height(0)
	,m_nDlgHeight(0)
	,m_nDlgWidth(0)
	,m_nHeight(0)
	,m_nWidth(0)
{
	m_titlefont.CreatePointFont(105,_T("微软雅黑"));
	m_nPigID=analyinfo.nPigID;
	m_PigInfoVec=analyinfo.piginfoVec;
	if(analyinfo.img){
		m_Img=cvCloneImage(analyinfo.img);
	}
	m_StaticImg=cvCreateImage(cvSize(WIDTH+MARGIN*2,HEIGHT+MARGIN*2),IPL_DEPTH_8U,3);
	cvSet(m_StaticImg,cvScalarAll(255));
}

CDlgAnalyse::~CDlgAnalyse()
{
	m_titlefont.DeleteObject();
	if (m_Img){
		cvReleaseImage(&m_Img);
	}
	if (m_StaticImg){
		cvReleaseImage(&m_StaticImg);
	}
}

void CDlgAnalyse::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAnalyse, CDialogSK)
	ON_BN_CLICKED(BTN_MIN, &CDlgAnalyse::OnBnClickedMin)
	ON_BN_CLICKED(BTN_CLOSE, &CDlgAnalyse::OnBnClickedClose)
	ON_BN_CLICKED(BTN_MAX, &CDlgAnalyse::OnBnClickedMax)
	ON_BN_CLICKED(IDOK, &CDlgAnalyse::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgAnalyse 消息处理程序
void CDlgAnalyse::InitWidowsBG()
{
	//设置任务栏显示窗体的Title信息
	CString str;
	str.Format(_T("%d号猪的数据分析"),m_nPigID);
	this->SetDlgItemText(STATIC_TITLE,str);
	SetWindowText(str);
	EnableEasyMove(1);                       // 允许移动
	SetBitmap(_T("res/analysebg.bmp"));    // 设置背景位图
	SetStyle(LO_STRETCH);                   // 伸缩图片以适合对话框大小
	SetTransparentColor(RGB(0, 255, 0));     // 设置透明色为绿色
	CRect   temprect(0,0,500,680);			 // 设置窗体的初始化大小
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(), SWP_NOZORDER|SWP_NOMOVE);
}

void CDlgAnalyse::InitButtonCtrl()
{
	// 最小化最大化关闭按钮重绘
	m_Btn[BTN_ID_MIN].AutoLoad(BTN_MIN, this); 
	m_Btn[BTN_ID_MIN].LoadBitmaps(IDB_MIN,IDB_MIN_CLICK); 
	m_Btn[BTN_ID_MIN].RedrawWindow();
	m_Btn[BTN_ID_MIN].SizeToContent();
	m_Btn[BTN_ID_MAX].AutoLoad(BTN_MAX, this); 
	m_Btn[BTN_ID_MAX].LoadBitmaps(IDB_MAX,IDB_MAX_CLICK,NULL,IDB_MAX_DIS); 
	m_Btn[BTN_ID_MAX].RedrawWindow();
	m_Btn[BTN_ID_MAX].SizeToContent();
	m_Btn[BTN_ID_CLOSE].AutoLoad(BTN_CLOSE, this); 
	m_Btn[BTN_ID_CLOSE].LoadBitmaps(IDB_CLOSE,IDB_CLOSE_CLICK); 
	m_Btn[BTN_ID_CLOSE].RedrawWindow();
	m_Btn[BTN_ID_CLOSE].SizeToContent();
	m_Btn[BTN_ID_OK].AutoLoad(IDOK, this); 
	m_Btn[BTN_ID_OK].LoadBitmaps(IDB_OK,IDB_OK_CLICK,IDB_OK,IDB_OK_DIS); 
	m_Btn[BTN_ID_OK].RedrawWindow();
	m_Btn[BTN_ID_OK].SizeToContent();
}
void CDlgAnalyse::OnBnClickedMin()
{
	ShowWindow(SW_SHOWMINIMIZED);
}


void CDlgAnalyse::OnBnClickedClose()
{
	OnCancel();
}


void CDlgAnalyse::OnBnClickedMax()
{
	if(IsZoomed()){
		EnableEasyMove(1);
		ShowWindow(SW_SHOWNORMAL);
	}else{
		EnableEasyMove(0);
		ShowWindow(SW_SHOWMAXIMIZED);
	}
}


void CDlgAnalyse::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogSK::OnOK();
}

BOOL CDlgAnalyse::OnInitDialog()
{
	CDialogSK::OnInitDialog();
	InitWidowsBG();
	InitButtonCtrl();
	InitialResize();// 初始化窗体、控件自适应变化
	TimerID=SetTimer(1,30,NULL);
	DrawImg();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CDlgAnalyse::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		GROUPDOWN_COLOR=RGB(132,168,216),
	};

	STATIC_BKCOLOR static_BkColor = NULL_COLOR;
	HBRUSH return_hbr = hbr;

	switch (pWnd->GetDlgCtrlID())
	{
	case STATIC_TITLE:
		static_BkColor = TITLE_COLOR;
		break;
	case GROUP_MOTION:
		static_BkColor=GROUPUP_COLOR;
		break;
	case GROUP_STATISTICS:
		static_BkColor=GROUPDOWN_COLOR;
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
void CDlgAnalyse::InitialResize()
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

void CDlgAnalyse::ReSize(int nID,bool nZoom)
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
		if(nZoom){
			BRPoint.x = long(OldBRPoint.x *m_Multiple_width); 
			BRPoint.y = long(OldBRPoint.y *m_Multiple_height); 
		}else{
			BRPoint.x = long(TLPoint.x + Rect.Width()); 
			BRPoint.y = long(TLPoint.y + Rect.Height()); 
		}
		//移动控件到新矩形
		Rect.SetRect(TLPoint,BRPoint); 
		GetDlgItem(nID)->MoveWindow(Rect,false); 
	}
}

void CDlgAnalyse::OnSize(UINT nType, int cx, int cy)
{
	if (m_Sizechange_flag)//如果确定OninitDlg已经调用完毕.
	{
		//随着窗体大小变化调整控件的大小及位置
		ReSize(BTN_MIN);
		ReSize(BTN_MAX);
		ReSize(BTN_CLOSE);
		ReSize(IDOK);
		ReSize(IDCANCEL);

		ReSize(PIC_STATISTICS,true);
		ReSize(PIC_MOTION,true);

		ReSize(GROUP_MOTION,true);
		ReSize(GROUP_STATISTICS,true);
		m_Multiple_width = float(1)/ m_Multiple_width ; 
		m_Multiple_height = float(1)/m_Multiple_height ; 
	}
	CDialogSK::OnSize(nType, cx, cy);
}

BOOL CDlgAnalyse::PreTranslateMessage(MSG* pMsg)
{
	//屏蔽其响应回车键/ESC键
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam== VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialogSK::PreTranslateMessage(pMsg);
}

void CDlgAnalyse::ShowMotionImg()
{
	if (!m_Img){
		return;
	}
	CRect rc;
	CWnd* pWnd=GetDlgItem(PIC_MOTION);
	pWnd->GetClientRect(rc);
	if (m_Img && pWnd->GetWindowDC()->GetSafeHdc()){
		m_ShowImg.CopyOf(m_Img,1);    // 此处第二个参数不能用CopyOf的默认参数-1，必须用1， 否则8位单通道图像不能显示
		m_ShowImg.DrawToHDC(pWnd->GetWindowDC()->GetSafeHdc(),&rc);
	}
}

void CDlgAnalyse::ShowStatisticsImg()
{
	if (!m_StaticImg){
		return;
	}
	CRect rc;
	CWnd* pWnd=GetDlgItem(PIC_STATISTICS);
	pWnd->GetClientRect(rc);
	if (m_StaticImg && pWnd->GetWindowDC()->GetSafeHdc()){
		m_ShowImg.CopyOf(m_StaticImg,1);    // 此处第二个参数不能用CopyOf的默认参数-1，必须用1， 否则8位单通道图像不能显示
		m_ShowImg.DrawToHDC(pWnd->GetWindowDC()->GetSafeHdc(),&rc);
	}
}

void CDlgAnalyse::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowMotionImg();
	ShowStatisticsImg();
	CDialogSK::OnTimer(nIDEvent);
}

void CDlgAnalyse::SetAnalyseParam( const Analy_Struct& analyinfo )
{
	m_nPigID=analyinfo.nPigID;
	m_PigInfoVec=analyinfo.piginfoVec;
	if(analyinfo.img){
		if (m_Img){
			cvReleaseImage(&m_Img);
		}
		m_Img=cvCloneImage(analyinfo.img);
	}
	if(m_StaticImg){
		m_StaticImg=cvCreateImage(cvSize(WIDTH+MARGIN*2,HEIGHT+MARGIN*2),IPL_DEPTH_8U,3);
		cvSet(m_StaticImg,cvScalarAll(255));
	}

	CString str;
	str.Format(_T("%d号猪的数据分析"),m_nPigID);
	this->SetDlgItemText(STATIC_TITLE,str);
	SetWindowText(str);

	CRect rc;
	CWnd* pWnd=GetDlgItem(STATIC_TITLE);
	pWnd->GetClientRect(rc);
	InvalidateRect(rc,TRUE);

	DrawImg();
}

void CDlgAnalyse::DrawImg()
{//画轨迹图
	CvPoint startPoint,endPoint;
	const int nCount=m_PigInfoVec.size();
	if (nCount<3){
		return;
	}
	for (int i=0; i<nCount; ++i)
	{
		CMainProgram_GUIDlg::Pig_Action_Info &piginfo=m_PigInfoVec.at(i);
		if (0==i){
			startPoint.x=piginfo.nInfo[0];
			startPoint.y=piginfo.nInfo[1];
			continue;
		}
		endPoint.x=piginfo.nInfo[0];
		endPoint.y=piginfo.nInfo[1];
		cvLine(m_Img,startPoint,endPoint,CV_RGB(255,0,0));
		startPoint.x=endPoint.x;
		startPoint.y=endPoint.y;
	}
	//画统计图
	CvPoint orginPoint;//坐标原点在图像中的位置
	orginPoint.x=MARGIN;
	orginPoint.y=HEIGHT+MARGIN;
	cvLine(m_StaticImg,orginPoint,cvPoint(WIDTH+MARGIN,HEIGHT+MARGIN),CV_RGB(255,0,0),2);//x轴
	cvLine(m_StaticImg,cvPoint(WIDTH,HEIGHT+MARGIN*0.5),cvPoint(WIDTH+MARGIN,HEIGHT+MARGIN),CV_RGB(255,0,0),2);//x轴上箭头
	cvLine(m_StaticImg,cvPoint(WIDTH,HEIGHT+MARGIN*1.5),cvPoint(WIDTH+MARGIN,HEIGHT+MARGIN),CV_RGB(255,0,0),2);//x轴下箭头
	cvLine(m_StaticImg,orginPoint,cvPoint(MARGIN,MARGIN),CV_RGB(255,0,0),2);//y轴
	cvLine(m_StaticImg,cvPoint(MARGIN*0.5,MARGIN*2),cvPoint(MARGIN,MARGIN),CV_RGB(255,0,0),2);//y轴左箭头
	cvLine(m_StaticImg,cvPoint(MARGIN*1.5,MARGIN*2),cvPoint(MARGIN,MARGIN),CV_RGB(255,0,0),2);//y轴右箭头

	vector<double> motionVec;//运动量向量-前帧中心点与后帧中心点的距离
	vector<double> disVec;//与母猪距离的向量
	for (int i=0; i<nCount; ++i)
	{
		CMainProgram_GUIDlg::Pig_Action_Info &piginfo=m_PigInfoVec.at(i);
		if (0==i){
			startPoint.x=piginfo.nInfo[0];
			startPoint.y=piginfo.nInfo[1];
			continue;
		}
		endPoint.x=piginfo.nInfo[0];
		endPoint.y=piginfo.nInfo[1];
		double motion=sqrt((startPoint.x-endPoint.x)*(startPoint.x-endPoint.x)*1.+
			(startPoint.y-endPoint.y)*(startPoint.y-endPoint.y)*1.);
		motionVec.push_back(motion);
		disVec.push_back(piginfo.dInfo[3]);
		startPoint.x=endPoint.x;
		startPoint.y=endPoint.y;
	}
	assert(motionVec.size()==disVec.size());
	double maxMotion=*max_element(motionVec.begin(),motionVec.end());
	double maxDis=*max_element(disVec.begin(),disVec.end());
	double maxValue=maxMotion > maxDis ? maxMotion : maxDis;
	int nXGap=WIDTH/(nCount-1);//x轴中每个点的间隔:第一个点不计,因为要计算运动量
	double nYGap=HEIGHT/maxValue;//y轴中数据缩放系数
	CvPoint disStartPoint,disEndPoint;
	for (int i=0; i<motionVec.size(); ++i)
	{
		maxMotion=motionVec.at(i);
		maxDis=disVec.at(i);
		if (0==i){
			startPoint.x=MARGIN+i*nXGap;
			startPoint.y=HEIGHT+MARGIN-maxMotion*nYGap;
			disStartPoint.x=MARGIN+i*nXGap;
			disStartPoint.y=HEIGHT+MARGIN-maxDis*nYGap;
			continue;
		}
		endPoint.x=MARGIN+i*nXGap;
		endPoint.y=HEIGHT+MARGIN-maxMotion*nYGap;
		disEndPoint.x=MARGIN+i*nXGap;
		disEndPoint.y=HEIGHT+MARGIN-maxDis*nYGap;
		cvLine(m_StaticImg,startPoint,endPoint,CV_RGB(0,255,0),1,CV_AA);//绿色的运动量统计
		cvLine(m_StaticImg,disStartPoint,disEndPoint,CV_RGB(0,0,255),1,CV_AA);//蓝色的与母猪距离统计
		startPoint.x=endPoint.x;
		startPoint.y=endPoint.y;
		disStartPoint.x=disEndPoint.x;
		disStartPoint.y=disEndPoint.y;
		//setlocale(LC_ALL,   "Chinese_China");
		CvxText text(".\\res\\STXINGKA.ttf");//如果出现异常,必须把msvcrt.lib库忽略掉
		text.putText(m_StaticImg,_T("绿色线表示运动量统计"), cvPoint(WIDTH*0.7, MARGIN*0.5), CV_RGB(0,255,0));
		text.putText(m_StaticImg,_T("蓝色线表示与母猪距离"), cvPoint(WIDTH*0.7, MARGIN*1.2), CV_RGB(0,0,255));
	}
}

void CDlgAnalyse::OnDestroy()
{
	CDialogSK::OnDestroy();
	KillTimer(TimerID);
	// TODO: 在此处添加消息处理程序代码
}
