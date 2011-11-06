#pragma once
#include "afxcmn.h"
#include "CDialogSK.h"
#include "MainProgram_GUIDlg.h"
#include "highgui.h"
// CDlgAnalyse �Ի���
typedef struct _ANALYSESTRUCT{
	int nPigID;
	IplImage *img;
	vector<CMainProgram_GUIDlg::Pig_Action_Info> piginfoVec;
}Analy_Struct;//��Ҫ��ͳ������������ݽṹ
class CDlgAnalyse : public CDialogSK
{
public:
	CDlgAnalyse(const Analy_Struct& analyinfo,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAnalyse();

// �Ի�������
	enum { IDD = IDD_ANALYSE };
	CBitmapButton m_Btn[4];  //��С������󻯡��رա�ȷ��
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
	int m_nPigID;//��ֻ���
	IplImage *m_Img;//�˶��켣ͼ��
	IplImage *m_StaticImg;//ͳ��ͼ��
	vector<CMainProgram_GUIDlg::Pig_Action_Info> m_PigInfoVec;//��ֻ�˶���Ϣ
	CvvImage m_ShowImg;
	void ShowMotionImg();
	void ShowStatisticsImg();
	void DrawImg();//���켣ͼ��ͳ��ͼ

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
