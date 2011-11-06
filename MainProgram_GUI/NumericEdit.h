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
//使用方法
//在你的对话框中添加变量： 
//prvate: 
//CMyEdit   m_Edit; 
//在对话框的OnInitDialog()初始化！ 
//	BOOL   CAboutDlg::OnInitDialog()   
//{ 
//	... 
//		//控件的子类型化 
//		m_Edit.SubclassDlgItem(IDC_YOUR_EDIT,this); 
//	.... 
//} 
//-------------- 
//	别忘了包含相应的头文件！ 
