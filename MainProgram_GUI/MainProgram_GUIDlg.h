
// MainProgram_GUIDlg.h : ͷ�ļ�
//

#pragma once
#include "CDialogSK.h"
#include "FindPigObject.h"
#include "JHCVideo/VideoDevice.h"
#include "../MainProgram/PhataTrackBlobs.h"
#include "DlgDVRConfig.h"
//#include "DlgAnalyse.h"
//otl�޷�ʹ��like��ѯ���������������������like��ѯ�ľ�̬sql���
#define OTL_ODBC // Compile OTL 4.0/ODBC���������#include "otlv4.h"ǰ
// #define OTL_ODBC_UNIX // �����Unix��ʹ��UnixODBC������Ҫ�����
#define OTL_STL // Turn on STL features��������otl_stream��ʹ��<<,>>����string��STL�е���������
#if (defined(UNICODE)||defined(_UNICODE))//�����쳣��otl_exception��Ч��otl_exceptionֻ֧��char����
#define OTL_UNICODE // Enable Unicode OTL for MS SQL SNAC
#define OTL_UNICODE_CHAR_TYPE wchar_t
#define OTL_ODBC_TIME_ZONE
#endif
#pragma comment(lib,"odbc32.lib")
#include "otlv4.h"

#define AVITHREAD 1//����������Ƶ�߳�
#define DVRTHREAD 2//����DVR�߳�
#define NULLTHREAD 3//�޶��߳�

typedef enum _EVENTYPE{
	EVEN_REST=1,//��Ϣ��ǰN��λ��һֱ����
	EVEN_RUN,//���ܣ�ǰN��λ�ñ仯��
	EVEN_LINGER,//�ǻ���ǰN��λ�ñ仯����
    EVEN_CLOSE,//����ĸ��ǰN����ĸ��ľ���Խ��ԽС
	EVEN_CRUSH,//�ܼ�ѹ��ǰN�뿿��ĸ����ʧ
	EVEN_NORMAL//��������������
}EVENTYPE;

class CDlgAnalyse;//������
// CMainProgram_GUIDlg �Ի���
class CMainProgram_GUIDlg : public CDialogSK
{
// ����
public:
	CMainProgram_GUIDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
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
	CBitmapButton m_Btn[6];  //��С������󻯡��رա��򿪱�����Ƶ��DVR��ť�����ð�ť

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	//�������ؼ����洰�ڱ仯����ʱ����
	int m_nDlgWidth,m_nDlgHeight,m_nWidth,m_nHeight;
	float m_Multiple_width,m_Multiple_height;
	bool m_Sizechange_flag;
	CFont m_titlefont;				//����Title������
	vector<Pig_Info> m_PiginfoVec;//��ֻ��Ϣ,�����ж�״̬�ĸı�
	CvPoint2D32f m_sowcenter;//ĸ�����ĵ�
	void InitialResize();//��ʼ�����壬�ؼ�����Ӧ��С
	void ReSize(int nID,bool nZoom = false);//���ݴ�������Ÿı�ؼ���λ�����С,nZoom��ؼ���С��������
	void InitWindowsBG();
	void InitButtonCtrl();
	void InitListCtrl();
	void insertPigletList(const CStringArray& csArray);//��track�����б��У��и��޼�
	void insertEvenList(const CStringArray& csArray);//���¼������б��У��и��޼�

	bool insertPigletInfoToDB(const Pig_Action_Info& piginfo);//��������Ϣ��ӽ����ݿ⣬�����¼����
	bool insertEvenInfoToDB(const Even_Info& eveninfo);//����⵽���¼���Ϣ��ӽ����ݿ�
	bool doOpenDevice(DVRConfig_Info dvr,CString filepath=_T(""));	//���豸ʱ��ʼ��������

	//��Ƶ��ر���
	VideoDevice *m_video;
	void SetImage(IplImage* img,int nCtrlID);

	//Ŀ����ٱ���
	PhataTrackBlobs *m_track;
	bool InitFindPigParam(IplImage *img);
	findpigblobs_userdata m_userdata;
	bool m_isInitFindPigParam;//�Ƿ��Ѿ���ʼ��m_userdata����Ϊ��ʼ������Ҫ�ͷ��ڴ�

 	//��ʱ��
	UINT_PTR FilePlayTimer
		;
   //�̱߳���
	//ͼ�����߳�
	CCriticalSection m_cs_ImgPro;
	CWinThread *m_pIPThread;
	int m_threadType;//�߳����ͣ�1Ϊ������Ƶ�̣߳�2ΪDVR��Ƶ�̣߳�3Ϊ�޿������߳�
	friend UINT IPThreadProc( LPVOID pParam );//��Ԫ����
	//�¼�����߳�
	CWinThread *m_pEDThread;
	CCriticalSection m_cs_Evendete;
	friend UINT EventDetectingThreadProc( LPVOID pParam );//��Ԫ����
	bool doStartEventDetectThread();//�����¼�����߳�

	//�߾��ȼ�ʱ��غ��������
	double m_dFreq;//��ʱ����ʱ��Ƶ��
	double m_dFirstTime;//��ʼʱ��
	double m_dSecondTime;//����ʱ��(��ǰʱ��-��ʼʱ��)
	double GetDuration();//�õ�����ʱ�� 
	void RestoreTime();//�ָ���ʱ���Ŀ�ʼʱ��

	//��������
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
	afx_msg LRESULT OnDisplayPigInfo(WPARAM wParam,LPARAM lParam);//����ֻ��Ϣ��ʾ��list�ؼ���
	afx_msg LRESULT OnDisplayEven(WPARAM wParam,LPARAM lParam);//���¼���Ϣ��ʾ��list�ؼ���
	afx_msg void OnCancel();
	afx_msg void OnBnClickedConfig();
	afx_msg void OnNMDblclkPiglet(NMHDR *pNMHDR, LRESULT *pResult);
};
