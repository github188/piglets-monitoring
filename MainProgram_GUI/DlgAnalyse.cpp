// DlgDVRConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainProgram_GUI.h"
#include "DlgAnalyse.h"
#include "afxdialogex.h"
#include  <algorithm> 
//opencv������ֵĹ���
#include ".\\opencv�������\\CvxText.h"
#pragma comment(lib, ".\\opencv�������\\lib\\freetype235.lib")
// CDlgAnalyse �Ի���
enum BTN_ID{
	BTN_ID_MIN=0,
	BTN_ID_MAX=1,
	BTN_ID_CLOSE=2,
	BTN_ID_OK=3
};
const int WIDTH=400;//ͳ��ͼ�Ŀ��
const int HEIGHT=300;//ͳ��ͼ�ĸ߶�
const int INTERVAL=5;//ͳ�����ݼ��
const int MARGIN=40;//��������ͼ���Ե�ľ���
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
	m_titlefont.CreatePointFont(105,_T("΢���ź�"));
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


// CDlgAnalyse ��Ϣ�������
void CDlgAnalyse::InitWidowsBG()
{
	//������������ʾ�����Title��Ϣ
	CString str;
	str.Format(_T("%d��������ݷ���"),m_nPigID);
	this->SetDlgItemText(STATIC_TITLE,str);
	SetWindowText(str);
	EnableEasyMove(1);                       // �����ƶ�
	SetBitmap(_T("res/analysebg.bmp"));    // ���ñ���λͼ
	SetStyle(LO_STRETCH);                   // ����ͼƬ���ʺ϶Ի����С
	SetTransparentColor(RGB(0, 255, 0));     // ����͸��ɫΪ��ɫ
	CRect   temprect(0,0,500,680);			 // ���ô���ĳ�ʼ����С
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(), SWP_NOZORDER|SWP_NOMOVE);
}

void CDlgAnalyse::InitButtonCtrl()
{
	// ��С����󻯹رհ�ť�ػ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogSK::OnOK();
}

BOOL CDlgAnalyse::OnInitDialog()
{
	CDialogSK::OnInitDialog();
	InitWidowsBG();
	InitButtonCtrl();
	InitialResize();// ��ʼ�����塢�ؼ�����Ӧ�仯
	TimerID=SetTimer(1,30,NULL);
	DrawImg();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
// ��ʼ�����塢�ؼ�����Ӧ�仯
void CDlgAnalyse::InitialResize()
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

void CDlgAnalyse::ReSize(int nID,bool nZoom)
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
		if(nZoom){
			BRPoint.x = long(OldBRPoint.x *m_Multiple_width); 
			BRPoint.y = long(OldBRPoint.y *m_Multiple_height); 
		}else{
			BRPoint.x = long(TLPoint.x + Rect.Width()); 
			BRPoint.y = long(TLPoint.y + Rect.Height()); 
		}
		//�ƶ��ؼ����¾���
		Rect.SetRect(TLPoint,BRPoint); 
		GetDlgItem(nID)->MoveWindow(Rect,false); 
	}
}

void CDlgAnalyse::OnSize(UINT nType, int cx, int cy)
{
	if (m_Sizechange_flag)//���ȷ��OninitDlg�Ѿ��������.
	{
		//���Ŵ����С�仯�����ؼ��Ĵ�С��λ��
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
	//��������Ӧ�س���/ESC��
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
		m_ShowImg.CopyOf(m_Img,1);    // �˴��ڶ�������������CopyOf��Ĭ�ϲ���-1��������1�� ����8λ��ͨ��ͼ������ʾ
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
		m_ShowImg.CopyOf(m_StaticImg,1);    // �˴��ڶ�������������CopyOf��Ĭ�ϲ���-1��������1�� ����8λ��ͨ��ͼ������ʾ
		m_ShowImg.DrawToHDC(pWnd->GetWindowDC()->GetSafeHdc(),&rc);
	}
}

void CDlgAnalyse::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	str.Format(_T("%d��������ݷ���"),m_nPigID);
	this->SetDlgItemText(STATIC_TITLE,str);
	SetWindowText(str);

	CRect rc;
	CWnd* pWnd=GetDlgItem(STATIC_TITLE);
	pWnd->GetClientRect(rc);
	InvalidateRect(rc,TRUE);

	DrawImg();
}

void CDlgAnalyse::DrawImg()
{//���켣ͼ
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
	//��ͳ��ͼ
	CvPoint orginPoint;//����ԭ����ͼ���е�λ��
	orginPoint.x=MARGIN;
	orginPoint.y=HEIGHT+MARGIN;
	cvLine(m_StaticImg,orginPoint,cvPoint(WIDTH+MARGIN,HEIGHT+MARGIN),CV_RGB(255,0,0),2);//x��
	cvLine(m_StaticImg,cvPoint(WIDTH,HEIGHT+MARGIN*0.5),cvPoint(WIDTH+MARGIN,HEIGHT+MARGIN),CV_RGB(255,0,0),2);//x���ϼ�ͷ
	cvLine(m_StaticImg,cvPoint(WIDTH,HEIGHT+MARGIN*1.5),cvPoint(WIDTH+MARGIN,HEIGHT+MARGIN),CV_RGB(255,0,0),2);//x���¼�ͷ
	cvLine(m_StaticImg,orginPoint,cvPoint(MARGIN,MARGIN),CV_RGB(255,0,0),2);//y��
	cvLine(m_StaticImg,cvPoint(MARGIN*0.5,MARGIN*2),cvPoint(MARGIN,MARGIN),CV_RGB(255,0,0),2);//y�����ͷ
	cvLine(m_StaticImg,cvPoint(MARGIN*1.5,MARGIN*2),cvPoint(MARGIN,MARGIN),CV_RGB(255,0,0),2);//y���Ҽ�ͷ

	vector<double> motionVec;//�˶�������-ǰ֡���ĵ����֡���ĵ�ľ���
	vector<double> disVec;//��ĸ����������
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
	int nXGap=WIDTH/(nCount-1);//x����ÿ����ļ��:��һ���㲻��,��ΪҪ�����˶���
	double nYGap=HEIGHT/maxValue;//y������������ϵ��
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
		cvLine(m_StaticImg,startPoint,endPoint,CV_RGB(0,255,0),1,CV_AA);//��ɫ���˶���ͳ��
		cvLine(m_StaticImg,disStartPoint,disEndPoint,CV_RGB(0,0,255),1,CV_AA);//��ɫ����ĸ�����ͳ��
		startPoint.x=endPoint.x;
		startPoint.y=endPoint.y;
		disStartPoint.x=disEndPoint.x;
		disStartPoint.y=disEndPoint.y;
		//setlocale(LC_ALL,   "Chinese_China");
		CvxText text(".\\res\\STXINGKA.ttf");//��������쳣,�����msvcrt.lib����Ե�
		text.putText(m_StaticImg,_T("��ɫ�߱�ʾ�˶���ͳ��"), cvPoint(WIDTH*0.7, MARGIN*0.5), CV_RGB(0,255,0));
		text.putText(m_StaticImg,_T("��ɫ�߱�ʾ��ĸ�����"), cvPoint(WIDTH*0.7, MARGIN*1.2), CV_RGB(0,0,255));
	}
}

void CDlgAnalyse::OnDestroy()
{
	CDialogSK::OnDestroy();
	KillTimer(TimerID);
	// TODO: �ڴ˴������Ϣ����������
}
