#pragma once
#include "afxcmn.h"
#include "CDialogSK.h"
#include <string>
using std::string;
// CDlgDVRConfig 对话框
typedef struct _DVRCONFIG{
	string ipadd;
	int port;
	string username;
	string pwd;
	int nchannel;
	string remark;
}DVRConfig_Info;
class CDlgDVRConfig : public CDialogSK
{
public:
	CDlgDVRConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDVRConfig();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
private:
// 对话框数据
	enum { IDD = IDD_DVR_CONFIG };
	CBitmapButton m_Btn[5];  //最小化、最大化、关闭、确定、取消按钮
	//用于做控件跟随窗口变化的临时变量
	int m_nDlgWidth,m_nDlgHeight,m_nWidth,m_nHeight;
	float m_Multiple_width,m_Multiple_height;
	bool m_Sizechange_flag;
	CFont m_titlefont;				//窗体Title的字体
	DVRConfig_Info m_dvrInfo;
	void InitialResize();//初始化窗体，控件自适应大小
	void ReSize(int nID,bool nZoom = false);//根据窗体的缩放改变控件的位置与大小,nZoom表控件大小缩不缩放
	void InitWidowsBG();
	void InitButtonCtrl();
	bool ReadDVRConfigInfoFromDB();//从数据库获取DVR配置信息用于填充m_dvrInfo变量
	bool WriteDVRConfigInfoToDB();//把m_dvrInfo变量信息写入数据库

public:
	CIPAddressCtrl m_IPAdd;
	DVRConfig_Info GetDVRConfigInfo()const{return m_dvrInfo;};
	afx_msg void OnBnClickedMin();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedMax();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
