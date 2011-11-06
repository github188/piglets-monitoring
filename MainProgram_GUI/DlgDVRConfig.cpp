// DlgDVRConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainProgram_GUI.h"
#include "DlgDVRConfig.h"
#include "afxdialogex.h"

#define OTL_STL // Turn on STL features��������otl_stream��ʹ��<<,>>����string��STL�е���������

// CDlgDVRConfig �Ի���
enum BTN_ID{
	BTN_ID_MIN=0,
	BTN_ID_MAX=1,
	BTN_ID_CLOSE=2,
	BTN_ID_OK=3,
	BTN_ID_CANCEL=4,
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
	m_dvrInfo.ipadd="";
	m_dvrInfo.nchannel=-1;
	m_dvrInfo.port=0;
	m_dvrInfo.pwd="";
	m_dvrInfo.remark="";
	m_dvrInfo.username="";
}

CDlgDVRConfig::~CDlgDVRConfig()
{
	m_titlefont.DeleteObject();
}

void CDlgDVRConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	DDX_Control(pDX, IPADDRESS, m_IPAdd);
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

	GetDlgItem(BTN_MAX)->EnableWindow(FALSE);
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
	CString strTmp;
	m_IPAdd.GetWindowText(strTmp);
	m_dvrInfo.ipadd=CT2A((LPTSTR)(LPCTSTR)strTmp);
	m_dvrInfo.port=GetDlgItemInt(EDIT_PORT,NULL,true);
	m_dvrInfo.nchannel=GetDlgItemInt(EDIT_CHANNEL,NULL,true);
	GetDlgItem(EDIT_PWD)->GetWindowText(strTmp);
	m_dvrInfo.pwd=CT2A((LPTSTR)(LPCTSTR)strTmp);
	GetDlgItem(EDIT_REMARK)->GetWindowText(strTmp);
	m_dvrInfo.remark=CT2A((LPTSTR)(LPCTSTR)strTmp);
	GetDlgItem(EDIT_USERNAME)->GetWindowText(strTmp);
	m_dvrInfo.username=CT2A((LPTSTR)(LPCTSTR)strTmp);

	WriteDVRConfigInfoToDB();
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
	if(ReadDVRConfigInfoFromDB()){
		CString strTmp;
		strTmp=m_dvrInfo.ipadd.c_str();
		m_IPAdd.SetWindowText(strTmp);
		SetDlgItemInt(EDIT_PORT,m_dvrInfo.port,true);
		SetDlgItemInt(EDIT_CHANNEL,m_dvrInfo.nchannel,true);
		strTmp=m_dvrInfo.pwd.c_str();
		GetDlgItem(EDIT_PWD)->SetWindowText(strTmp);
		strTmp=m_dvrInfo.remark.c_str();
		GetDlgItem(EDIT_REMARK)->SetWindowText(strTmp);
		strTmp=m_dvrInfo.username.c_str();
		GetDlgItem(EDIT_USERNAME)->SetWindowText(strTmp);
	}
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
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam== VK_ESCAPE)
		&& pMsg->hwnd !=GetDlgItem(EDIT_REMARK)->GetSafeHwnd())
	{
		return TRUE;
	}
	return CDialogSK::PreTranslateMessage(pMsg);
}

bool CDlgDVRConfig::ReadDVRConfigInfoFromDB()
{
	try{
		g_cs_db.Lock();
		otl_stream dbcon;//��ȡ��������
		string strSql;
		strSql="select top 1 IPAddress,Port,UserName,PWD,Channel,Remark from DVR_Config";
		dbcon.open(50,strSql.c_str(),g_db);
		if(!dbcon.eof()){
			dbcon>>m_dvrInfo.ipadd;
			dbcon>>m_dvrInfo.port;
			dbcon>>m_dvrInfo.username;
			dbcon>>m_dvrInfo.pwd;
			dbcon>>m_dvrInfo.nchannel;
			dbcon>>m_dvrInfo.remark;
		}else{
			g_cs_db.Unlock();
			return false;
		}
		g_cs_db.Unlock();
		return true;
	}
	catch (otl_exception& p)
	{
		CString strErr,strStmText,strVarInfo;
		strStmText=p.stm_text;strVarInfo=p.var_info;
		strErr.Format(_T("%s\r\n%s\r\n%s\r\n%s"),p.msg,strStmText.GetBuffer(),p.sqlstate,strVarInfo.GetBuffer());
		MessageBox(strErr,_T("�����ݿ��ȡDVR��Ϣ����"),MB_OK);
		g_cs_db.Unlock();
		return false;
	}
}

bool CDlgDVRConfig::WriteDVRConfigInfoToDB()
{
	//д�����ݿ�
	g_cs_db.Lock();
	otl_stream conInsert;//��ȡ����
	conInsert.set_lob_stream_mode(true); // set the "lob stream mode" flag
	otl_lob_stream lob; // LOB stream for reading/writing unlimited number of bytes regardless of the buffer size.
	try{
		otl_stream conSelect;//��ȡ����
		conSelect.set_lob_stream_mode(true); // set the "lob stream mode" flag
		string strSql="Select * from DVR_Config";
		conSelect.open(50,strSql.c_str(),g_db);
		if (!conSelect.eof()){
			strSql=("update DVR_Config set IPAddress=:1<char[256]>,Port=:2<int> ,UserName=:3<char[256]>, ");
			strSql+=("	PWD=:4<char[256]>,Channel=:5<int>,Remark=:6<raw_long>");
			conInsert.open(1,strSql.c_str(),g_db);
			conInsert.set_commit(0);// setting stream "auto-commit" to "off". It is required when LOB stream mode is used.
			conInsert<<m_dvrInfo.ipadd<<m_dvrInfo.port<<m_dvrInfo.username<<m_dvrInfo.pwd<<m_dvrInfo.nchannel;
			conInsert<<lob;	
		}else{
			strSql=("insert into DVR_Config(IPAddress,Port,UserName,PWD,Channel,Remark) ");
			strSql+=(" values(:1<char[256]>,:2<int>,:3<char[256]>,:4<char[256]>,:5<int>,:6<raw_long>)");
			conInsert.open(1,strSql.c_str(),g_db);
			conInsert.set_commit(0);// setting stream "auto-commit" to "off". It is required when LOB stream mode is used.
			conInsert<<m_dvrInfo.ipadd<<m_dvrInfo.port<<m_dvrInfo.username<<m_dvrInfo.pwd<<m_dvrInfo.nchannel;
			conInsert<<lob;	
		}
		lob.set_len(m_dvrInfo.remark.length());
		lob<<m_dvrInfo.remark;
		lob.close();
		g_db.commit();
		g_cs_db.Unlock();
		return true;
	}
	catch (otl_exception& p)
	{
		CString strErr,strStmText,strVarInfo;
		strStmText=p.stm_text;strVarInfo=p.var_info;
		strErr.Format(_T("%s\r\n%s\r\n%s\r\n%s"),p.msg,strStmText.GetBuffer(),p.sqlstate,strVarInfo.GetBuffer());
		MessageBox(strErr,_T("д�����ݿ��ȡ����:DVR��Ϣ"),MB_OK);
		g_cs_db.Unlock();
		return false;
	}
}

