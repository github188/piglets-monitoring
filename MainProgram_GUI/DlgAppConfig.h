#pragma once
#include "afxcmn.h"
#include "CDialogSK.h"
#include "NumericEdit.h"
// CDlgAppConfig �Ի���

class CDlgAppConfig : public CDialogSK
{
public:
	CDlgAppConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAppConfig();

// �Ի�������
	enum { IDD = IDD_APP_CONFIG };
	CBitmapButton m_Btn[5];  //��С������󻯡��رա�ȷ����ȡ����ť
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	//�������ؼ����洰�ڱ仯����ʱ����
	int m_nDlgWidth,m_nDlgHeight,m_nWidth,m_nHeight;
	float m_Multiple_width,m_Multiple_height;
	bool m_Sizechange_flag;
	CFont m_titlefont;				//����Title������
	void InitialResize();//��ʼ�����壬�ؼ�����Ӧ��С
	void ReSize(int nID,bool nZoom = false);//���ݴ�������Ÿı�ؼ���λ�����С,nZoom��ؼ���С��������
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
