// DlgAppConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainProgram_GUI.h"
#include "DlgAppConfig.h"
#include "afxdialogex.h"


// CDlgAppConfig �Ի���
enum BTN_ID{
	BTN_ID_MIN=0,
	BTN_ID_MAX=1,
	BTN_ID_CLOSE=2,
	BTN_ID_OK=3,
	BTN_ID_CANCEL=4
};

CDlgAppConfig::CDlgAppConfig(CWnd* pParent /*=NULL*/)
	: CDialogSK(CDlgAppConfig::IDD, pParent)
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

CDlgAppConfig::~CDlgAppConfig()
{
	m_titlefont.DeleteObject();
}

void CDlgAppConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgAppConfig, CDialogSK)
	ON_BN_CLICKED(BTN_MIN, &CDlgAppConfig::OnBnClickedMin)
	ON_BN_CLICKED(BTN_CLOSE, &CDlgAppConfig::OnBnClickedClose)
	ON_BN_CLICKED(BTN_MAX, &CDlgAppConfig::OnBnClickedMax)
	ON_BN_CLICKED(IDOK, &CDlgAppConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgAppConfig::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDlgAppConfig ��Ϣ�������
void CDlgAppConfig::InitWidowsBG()
{
	//������������ʾ�����Title��Ϣ
	CString str;
	this->GetDlgItemText(STATIC_TITLE,str);
	SetWindowText(str);
	EnableEasyMove(1);                       // �����ƶ�
	SetBitmap(_T("res/appconfigbg.bmp"));    // ���ñ���λͼ
	SetStyle(LO_STRETCH);                   // ����ͼƬ���ʺ϶Ի����С
	SetTransparentColor(RGB(0, 255, 0));     // ����͸��ɫΪ��ɫ
	CRect   temprect(0,0,470,450);			 // ���ô���ĳ�ʼ����С
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(), SWP_NOZORDER|SWP_NOMOVE);
	GetDlgItem(BTN_MAX)->EnableWindow(FALSE);
}

void CDlgAppConfig::InitButtonCtrl()
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

void CDlgAppConfig::OnBnClickedMin()
{
	ShowWindow(SW_SHOWMINIMIZED);
}

void CDlgAppConfig::OnBnClickedClose()
{
	OnCancel();
}

void CDlgAppConfig::OnBnClickedMax()
{
	if(IsZoomed()){
		EnableEasyMove(1);
		ShowWindow(SW_SHOWNORMAL);
	}else{
		EnableEasyMove(0);
		ShowWindow(SW_SHOWMAXIMIZED);
	}
}

void CDlgAppConfig::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	App_Config appconfig;
	CString strTmp;
	GetDlgItemText(EDIT_SECOND,strTmp);
	appconfig.nSeconds =_ttoi((LPCTSTR)strTmp);  
	GetDlgItemText(EDIT_RECORD,strTmp);
	appconfig.nMinRecord =_ttoi((LPCTSTR)strTmp);  
	GetDlgItemText(EDIT_REST,strTmp);
	appconfig.dRest =_ttof((LPCTSTR)strTmp);  
	GetDlgItemText(EDIT_LINGER,strTmp);
	appconfig.dLinger =_ttof((LPCTSTR)strTmp);  
	GetDlgItemText(EDIT_RUN,strTmp);
	appconfig.dRun =_ttof((LPCTSTR)strTmp);  
	if (G_WriteAppConfigToDB(appconfig)){
		g_app_config.nSeconds=appconfig.nSeconds;
		g_app_config.nMinRecord=appconfig.nMinRecord;
		g_app_config.dRest=appconfig.dRest;
		g_app_config.dLinger=appconfig.dLinger;
		g_app_config.dRun=appconfig.dRun;
	}
	CDialogSK::OnOK();
}

void CDlgAppConfig::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogSK::OnCancel();
}

BOOL CDlgAppConfig::OnInitDialog()
{
	CDialogSK::OnInitDialog();
	InitWidowsBG();
	InitButtonCtrl();
	InitialResize();// ��ʼ�����塢�ؼ�����Ӧ�仯
	//���໯,ֻ��������ֵ�ı༭��
	m_EditRest.SubclassDlgItem(EDIT_REST,this); 
	m_EditLinger.SubclassDlgItem(EDIT_LINGER,this); 
	m_EditRun.SubclassDlgItem(EDIT_RUN,this);

	App_Config appconfig;
	CString strTmp;
	if(!G_ReadAppConfigFromDB(appconfig)){
	   
	}
	strTmp.Format(_T("%d"),appconfig.nSeconds);
	SetDlgItemText(EDIT_SECOND,strTmp);
	strTmp.Format(_T("%d"),appconfig.nMinRecord);
	SetDlgItemText(EDIT_RECORD,strTmp);
	strTmp.Format(_T("%f"),appconfig.dRest);
	SetDlgItemText(EDIT_REST,strTmp);
	strTmp.Format(_T("%f"),appconfig.dLinger);
	SetDlgItemText(EDIT_LINGER,strTmp);
	strTmp.Format(_T("%f"),appconfig.dRun);
	SetDlgItemText(EDIT_RUN,strTmp);
	if (!G_WriteAppConfigToDB(appconfig))
	{
		return true;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH CDlgAppConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	case STATIC_SECOND1:
	case STATIC_SECOND2:
	case STATIC_RECORD1:
	case STATIC_RECORD2:
	case STATIC_REST1:
	case STATIC_REST2:
	case STATIC_LINGER1:
	case STATIC_LINGER2:
	case STATIC_RUN1:
	case STATIC_RUN2:
		static_BkColor = STATIC_COLOR;
		break;
	case EDIT_SECOND:
	case EDIT_RECORD:
	case EDIT_REST:
	case EDIT_LINGER:
	case EDIT_RUN:
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
void CDlgAppConfig::InitialResize()
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

void CDlgAppConfig::ReSize(int nID,bool nZoom)
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

void CDlgAppConfig::OnSize(UINT nType, int cx, int cy)
{
	if (m_Sizechange_flag)//���ȷ��OninitDlg�Ѿ��������.
	{
		//���Ŵ����С�仯�����ؼ��Ĵ�С��λ��
		ReSize(BTN_MIN);
		ReSize(BTN_MAX);
		ReSize(BTN_CLOSE);
		ReSize(IDOK);
		ReSize(IDCANCEL);


		ReSize(EDIT_SECOND,true);
		ReSize(EDIT_RECORD,true);
		ReSize(EDIT_REST,true);
		ReSize(EDIT_LINGER,true);
		ReSize(EDIT_RUN,true);

		ReSize(STATIC_SECOND1,true);
		ReSize(STATIC_SECOND2,true);
		ReSize(STATIC_RECORD1,true);
		ReSize(STATIC_RECORD2,true);
		ReSize(STATIC_REST1,true);
		ReSize(STATIC_REST2,true);
		ReSize(STATIC_LINGER1,true);
		ReSize(STATIC_LINGER2,true);
		ReSize(STATIC_RUN1,true);
		ReSize(STATIC_RUN2,true);
		ReSize(STATIC_TITLE);

		ReSize(GROUP_CONFIG,true);
		m_Multiple_width = float(1)/ m_Multiple_width ; 
		m_Multiple_height = float(1)/m_Multiple_height ; 
	}
	CDialogSK::OnSize(nType, cx, cy);
}

BOOL CDlgAppConfig::PreTranslateMessage(MSG* pMsg)
{
	//��������Ӧ�س���/ESC��
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam== VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialogSK::PreTranslateMessage(pMsg);
}
