// DlgDVRConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainProgram_GUI.h"
#include "DlgDVRConfig.h"
#include "afxdialogex.h"


// CDlgDVRConfig �Ի���
enum BTN_ID{
	BTN_ID_MIN=0,
	BTN_ID_MAX=1,
	BTN_ID_CLOSE=2,
	BTN_ID_OK=3,
	BTN_ID_CANCEL=4
};

CDlgDVRConfig::CDlgDVRConfig(CWnd* pParent /*=NULL*/)
	: CDialogSK(CDlgDVRConfig::IDD, pParent)
	,m_Sizechange_flag(false)
	,m_Multiple_width(0)
	,m_Multiple_height(0)
	,m_nDlgHeight(0)
	,m_nDlgWidth(0)
	,m_nHeight(0)
	,m_nWidth(0)
{
	m_titlefont.CreatePointFont(105,_T("΢���ź�"));
}

CDlgDVRConfig::~CDlgDVRConfig()
{
	m_titlefont.DeleteObject();
}

void CDlgDVRConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDVRConfig, CDialogSK)
	ON_BN_CLICKED(BTN_MIN, &CDlgDVRConfig::OnBnClickedMin)
	ON_BN_CLICKED(BTN_CLOSE, &CDlgDVRConfig::OnBnClickedClose)
	ON_BN_CLICKED(BTN_MAX, &CDlgDVRConfig::OnBnClickedMax)
	ON_BN_CLICKED(IDOK, &CDlgDVRConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgDVRConfig::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgDVRConfig ��Ϣ�������
void CDlgDVRConfig::InitWidowsBG()
{
	//������������ʾ�����Title��Ϣ
	CString str;
	this->GetDlgItemText(STATIC_TITLE,str);
	SetWindowText(str);
	EnableEasyMove(1);                       // �����ƶ�
	SetBitmap(_T("res/dvrconfigbg.bmp"));    // ���ñ���λͼ
	SetStyle(LO_STRETCH);                   // ����ͼƬ���ʺ϶Ի����С
	SetTransparentColor(RGB(0, 255, 0));     // ����͸��ɫΪ��ɫ
	CRect   temprect(0,0,470,450);			 // ���ô���ĳ�ʼ����С
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(), SWP_NOZORDER|SWP_NOMOVE);
}

void CDlgDVRConfig::InitButtonCtrl()
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
	m_Btn[BTN_ID_CANCEL].AutoLoad(IDCANCEL, this); 
	m_Btn[BTN_ID_CANCEL].LoadBitmaps(IDB_CANCEL,IDB_CANCEL_CLICK,IDB_CANCEL,IDB_CANCEL_DIS); 
	m_Btn[BTN_ID_CANCEL].RedrawWindow();
	m_Btn[BTN_ID_CANCEL].SizeToContent();
}
void CDlgDVRConfig::OnBnClickedMin()
{
	ShowWindow(SW_SHOWMINIMIZED);
}


void CDlgDVRConfig::OnBnClickedClose()
{
	OnCancel();
}


void CDlgDVRConfig::OnBnClickedMax()
{
	if(IsZoomed()){
		EnableEasyMove(1);
		ShowWindow(SW_SHOWNORMAL);
	}else{
		EnableEasyMove(0);
		ShowWindow(SW_SHOWMAXIMIZED);
	}
}


void CDlgDVRConfig::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogSK::OnOK();
}


void CDlgDVRConfig::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogSK::OnCancel();
}


BOOL CDlgDVRConfig::OnInitDialog()
{
	CDialogSK::OnInitDialog();
	InitWidowsBG();
	InitButtonCtrl();
	InitialResize();// ��ʼ�����塢�ؼ�����Ӧ�仯
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CDlgDVRConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		static_BkColor = TITLE_COLOR;
		break;
	case STATIC_IPADD:
	case STATIC_PORT:
	case STATIC_USERNAME:
	case STATIC_PWD:
	case STATIC_CHANNEL:
	case STATIC_REMARK:
		static_BkColor = STATIC_COLOR;
		break;
	case EDIT_PORT:
	case EDIT_USERNAME:
	case EDIT_PWD:
	case EDIT_CHANNEL:
	case EDIT_REMARK:
		static_BkColor=EDIT_COLOR;
		break;
	case GROUP_CONFIG:
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
void CDlgDVRConfig::InitialResize()
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
void CDlgDVRConfig::ReSize(int nID,bool nZoom)
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

void CDlgDVRConfig::OnSize(UINT nType, int cx, int cy)
{
	if (m_Sizechange_flag)//���ȷ��OninitDlg�Ѿ��������.
	{
		//���Ŵ����С�仯�����ؼ��Ĵ�С��λ��
		ReSize(BTN_MIN);
		ReSize(BTN_MAX);
		ReSize(BTN_CLOSE);
		ReSize(IDOK);
		ReSize(IDCANCEL);

		ReSize(IPADDRESS,true);

		ReSize(EDIT_PORT,true);
		ReSize(EDIT_USERNAME,true);
		ReSize(EDIT_PWD,true);
		ReSize(EDIT_CHANNEL,true);
		ReSize(EDIT_REMARK,true);

		ReSize(STATIC_IPADD,true);
		ReSize(STATIC_PORT,true);
		ReSize(STATIC_USERNAME,true);
		ReSize(STATIC_PWD,true);
		ReSize(STATIC_CHANNEL,true);
		ReSize(STATIC_REMARK,true);
		ReSize(STATIC_TITLE);

		ReSize(GROUP_CONFIG,true);
		m_Multiple_width = float(1)/ m_Multiple_width ; 
		m_Multiple_height = float(1)/m_Multiple_height ; 
	}
	CDialogSK::OnSize(nType, cx, cy);
}


BOOL CDlgDVRConfig::PreTranslateMessage(MSG* pMsg)
{
	//��������Ӧ�س���/ESC��
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam== VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialogSK::PreTranslateMessage(pMsg);
}
