#pragma once
#include "afxcmn.h"
#include "CDialogSK.h"
#include "MainProgram_GUIDlg.h"
#include "highgui.h"
// CDlgAnalyse 对话框
typedef struct _ANALYSESTRUCT{
	int nPigID;
	IplImage *img;
	vector<CMainProgram_GUIDlg::Pig_Action_Info> piginfoVec;
}Analy_Struct;//需要传统分析界面的数据结构
class CDlgAnalyse : public CDialogSK
{
public:
	CDlgAnalyse(const Analy_Struct& analyinfo,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAnalyse();

// 对话框数据
	enum { IDD = IDD_ANALYSE };
	CBitmapButton m_Btn[4];  //最小化、最大化、关闭、确定
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	//用于做控件跟随窗口变化的临时变量
	int m_nDlgWidth,m_nDlgHeight,m_nWidth,m_nHeight;
	float m_Multiple_width,m_Multiple_height;
	bool m_Sizechange_flag;
	CFont m_titlefont;				//窗体Title的字体
	void InitialResize();//初始化窗体，控件自适应大小
	void ReSize(int nID,bool nZoom = false);//根据窗体的缩放改变控件的位置与大小,nZoom表控件大小缩不缩放
	void InitWidowsBG();
	void InitButtonCtrl();
	int m_nPigID;//猪只编号
	IplImage *m_Img;//运动轨迹图像
	IplImage *m_StaticImg;//统计图像
	vector<CMainProgram_GUIDlg::Pig_Action_Info> m_PigInfoVec;//猪只运动信息
	CvvImage m_ShowImg;
	void ShowMotionImg();
	void ShowStatisticsImg();
	void DrawImg();//画轨迹图与统计图

	int TimerID;
public:
	void SetAnalyseParam(const Analy_Struct& analyinfo);
	afx_msg void OnBnClickedMin();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedMax();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
