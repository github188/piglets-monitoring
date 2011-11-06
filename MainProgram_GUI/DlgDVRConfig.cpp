// DlgDVRConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "MainProgram_GUI.h"
#include "DlgDVRConfig.h"
#include "afxdialogex.h"

#define OTL_STL // Turn on STL features，可用于otl_stream流使用<<,>>操作string等STL中的数据类型

// CDlgDVRConfig 对话框
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
	m_titlefont.CreatePointFont(105,_T("微软雅黑"));
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


// CDlgDVRConfig 消息处理程序
void CDlgDVRConfig::InitWidowsBG()
{
	//设置任务栏显示窗体的Title信息
	CString str;
	this->GetDlgItemText(STATIC_TITLE,str);
	SetWindowText(str);
	EnableEasyMove(1);                       // 允许移动
	SetBitmap(_T("res/dvrconfigbg.bmp"));    // 设置背景位图
	SetStyle(LO_STRETCH);                   // 伸缩图片以适合对话框大小
	SetTransparentColor(RGB(0, 255, 0));     // 设置透明色为绿色
	CRect   temprect(0,0,470,450);			 // 设置窗体的初始化大小
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(), SWP_NOZORDER|SWP_NOMOVE);

	GetDlgItem(BTN_MAX)->EnableWindow(FALSE);
}

void CDlgDVRConfig::InitButtonCtrl()
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
	// TODO: 在此添加控件通知处理程序代码
	CDialogSK::OnCancel();
}


BOOL CDlgDVRConfig::OnInitDialog()
{
	CDialogSK::OnInitDialog();
	InitWidowsBG();
	InitButtonCtrl();
	InitialResize();// 初始化窗体、控件自适应变化
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
	// 异常: OCX 属性页应返回 FALSE
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
// 初始化窗体、控件自适应变化
void CDlgDVRConfig::InitialResize()
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
void CDlgDVRConfig::ReSize(int nID,bool nZoom)
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

void CDlgDVRConfig::OnSize(UINT nType, int cx, int cy)
{
	if (m_Sizechange_flag)//如果确定OninitDlg已经调用完毕.
	{
		//随着窗体大小变化调整控件的大小及位置
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
	//屏蔽其响应回车键/ESC键
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
		otl_stream dbcon;//获取类别的连接
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
		MessageBox(strErr,_T("从数据库获取DVR信息出错"),MB_OK);
		g_cs_db.Unlock();
		return false;
	}
}

bool CDlgDVRConfig::WriteDVRConfigInfoToDB()
{
	//写入数据库
	g_cs_db.Lock();
	otl_stream conInsert;//获取连接
	conInsert.set_lob_stream_mode(true); // set the "lob stream mode" flag
	otl_lob_stream lob; // LOB stream for reading/writing unlimited number of bytes regardless of the buffer size.
	try{
		otl_stream conSelect;//获取连接
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
		MessageBox(strErr,_T("写入数据库获取出错:DVR信息"),MB_OK);
		g_cs_db.Unlock();
		return false;
	}
}

