
// MainProgram_GUIDlg.h : 头文件
//

#pragma once
#include "CDialogSK.h"
#include "FindPigObject.h"
#include "JHCVideo/VideoDevice.h"
#include "../MainProgram/PhataTrackBlobs.h"
#include "DlgDVRConfig.h"
//#include "DlgAnalyse.h"
//otl无法使用like查询来接受输入参数，但可用like查询的静态sql语句
#define OTL_ODBC // Compile OTL 4.0/ODBC，必须放在#include "otlv4.h"前
// #define OTL_ODBC_UNIX // 如果在Unix下使用UnixODBC，则需要这个宏
#define OTL_STL // Turn on STL features，可用于otl_stream流使用<<,>>操作string等STL中的数据类型
#if (defined(UNICODE)||defined(_UNICODE))//对于异常类otl_exception无效，otl_exception只支持char数组
#define OTL_UNICODE // Enable Unicode OTL for MS SQL SNAC
#define OTL_UNICODE_CHAR_TYPE wchar_t
#define OTL_ODBC_TIME_ZONE
#endif
#pragma comment(lib,"odbc32.lib")
#include "otlv4.h"

#define AVITHREAD 1//开启本地视频线程
#define DVRTHREAD 2//开启DVR线程
#define NULLTHREAD 3//无多线程

typedef enum _EVENTYPE{
	EVEN_REST=1,//休息：前N秒位置一直不到
	EVEN_RUN,//奔跑：前N秒位置变化大
	EVEN_LINGER,//徘徊：前N秒位置变化不大
    EVEN_CLOSE,//靠近母猪：前N秒与母猪的距离越来越小
	EVEN_CRUSH,//受挤压：前N秒靠近母猪，消失
	EVEN_NORMAL//正常：正常行走
}EVENTYPE;

class CDlgAnalyse;//先声明
// CMainProgram_GUIDlg 对话框
class CMainProgram_GUIDlg : public CDialogSK
{
// 构造
public:
	CMainProgram_GUIDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_MAINPROGRAM_GUI_DIALOG };
	typedef struct _PIGACTIONINFO{
		int id;
		int nInfo[6];
		double dInfo[4];
	}Pig_Action_Info;
	typedef struct _EVENINFO{
		int pigno;
		int type;
		string des;
	}Even_Info;
	typedef struct _PIGINFO{
		int id;
		int state;
	}Pig_Info;

protected:
	HICON m_hIcon;
	CBitmapButton m_Btn[6];  //最小化、最大化、关闭、打开本地视频、DVR按钮、配置按钮

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	//用于做控件跟随窗口变化的临时变量
	int m_nDlgWidth,m_nDlgHeight,m_nWidth,m_nHeight;
	float m_Multiple_width,m_Multiple_height;
	bool m_Sizechange_flag;
	CFont m_titlefont;				//窗体Title的字体
	vector<Pig_Info> m_PiginfoVec;//猪只信息,用于判断状态的改变
	CvPoint2D32f m_sowcenter;//母猪中心点
	void InitialResize();//初始化窗体，控件自适应大小
	void ReSize(int nID,bool nZoom = false);//根据窗体的缩放改变控件的位置与大小,nZoom表控件大小缩不缩放
	void InitWindowsBG();
	void InitButtonCtrl();
	void InitListCtrl();
	void insertPigletList(const CStringArray& csArray);//将track插入列表中，有改无加
	void insertEvenList(const CStringArray& csArray);//将事件插入列表中，有改无加

	bool insertPigletInfoToDB(const Pig_Action_Info& piginfo);//将仔猪信息添加进数据库，用于事件检测
	bool insertEvenInfoToDB(const Even_Info& eveninfo);//将检测到的事件信息添加进数据库
	bool doOpenDevice(DVRConfig_Info dvr,CString filepath=_T(""));	//打开设备时初始化各变量

	//视频相关变量
	VideoDevice *m_video;
	void SetImage(IplImage* img,int nCtrlID);

	//目标跟踪变量
	PhataTrackBlobs *m_track;
	bool InitFindPigParam(IplImage *img);
	findpigblobs_userdata m_userdata;
	bool m_isInitFindPigParam;//是否已经初始化m_userdata，因为初始化后需要释放内存

 	//定时器
	UINT_PTR FilePlayTimer
		;
   //线程变量
	//图像处理线程
	CCriticalSection m_cs_ImgPro;
	CWinThread *m_pIPThread;
	int m_threadType;//线程类型，1为本地视频线程，2为DVR视频线程，3为无开启多线程
	friend UINT IPThreadProc( LPVOID pParam );//友元函数
	//事件检测线程
	CWinThread *m_pEDThread;
	CCriticalSection m_cs_Evendete;
	friend UINT EventDetectingThreadProc( LPVOID pParam );//友元函数
	bool doStartEventDetectThread();//开启事件检测线程

	//高精度计时相关函数与变量
	double m_dFreq;//定时器的时钟频率
	double m_dFirstTime;//开始时间
	double m_dSecondTime;//持续时间(当前时间-开始时间)
	double GetDuration();//得到持续时间 
	void RestoreTime();//恢复定时器的开始时间

	//分析窗口
	CDlgAnalyse *m_AnalyDlg;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedMin();
	afx_msg void OnBnClickedMax();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOpenavi();
	afx_msg void OnBnClickedOpendvr();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnDisplayPigInfo(WPARAM wParam,LPARAM lParam);//将猪只信息显示在list控件上
	afx_msg LRESULT OnDisplayEven(WPARAM wParam,LPARAM lParam);//将事件信息显示在list控件上
	afx_msg void OnCancel();
	afx_msg void OnBnClickedConfig();
	afx_msg void OnNMDblclkPiglet(NMHDR *pNMHDR, LRESULT *pResult);
};
