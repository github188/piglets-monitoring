#pragma once


// CNumericEdit

class CNumericEdit : public CEdit
{
	DECLARE_DYNAMIC(CNumericEdit)

public:
	CNumericEdit();
	virtual ~CNumericEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
//ʹ�÷���
//����ĶԻ�������ӱ����� 
//prvate: 
//CMyEdit   m_Edit; 
//�ڶԻ����OnInitDialog()��ʼ���� 
//	BOOL   CAboutDlg::OnInitDialog()   
//{ 
//	... 
//		//�ؼ��������ͻ� 
//		m_Edit.SubclassDlgItem(IDC_YOUR_EDIT,this); 
//	.... 
//} 
//-------------- 
//	�����˰�����Ӧ��ͷ�ļ��� 
