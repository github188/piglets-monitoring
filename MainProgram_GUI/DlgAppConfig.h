#pragma once
#include "afxcmn.h"
#include "CDialogSK.h"
#include "NumericEdit.h"
// CDlgAppConfig 对话框

class CDlgAppConfig : public CDialogSK
{
public:
	CDlgAppConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAppConfig();

// 对话框数据
	enum { IDD = IDD_APP_CONFIG };
	CBitmapButton m_Btn[5];  //最小化、最大化、关闭、确定、取消按钮
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

	CNumericEdit m_EditRest;
	CNumericEdit m_EditLinger;
	CNumericEdit m_EditRun;
public:
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
