// NumericEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "NumericEdit.h"


// CNumericEdit

IMPLEMENT_DYNAMIC(CNumericEdit, CEdit)

CNumericEdit::CNumericEdit()
{

}

CNumericEdit::~CNumericEdit()
{
}


BEGIN_MESSAGE_MAP(CNumericEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CNumericEdit 消息处理程序



void CNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString ss;
	char ch = char(nChar);
	GetWindowText(ss);  // 获得当前已输入的字符序列

	// 对当前输入字符进行判断
	if(!isdigit(ch) && ch!= '\b' ){// 如果不是数字与退格键
		if(ch == '.'){ //是小数点
			if(ss.Find('.') != -1){ // 检查是否已经输入了小数点
				return;
			}
		}else if (!((ch == '-' || ch=='+') && ss.IsEmpty())){//是正负号且在第一位
				return;
		}
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
//在输入结束后，模仿CEdit的数字规范化的功能，也就是类似0011-->11、11.100 ->11.1  .1100->0.11
void CNumericEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	CString m_sValue;
	GetWindowText(m_sValue);

	int index;
	TCHAR ch;
	int nLen = m_sValue.GetLength();

	if(nLen == 0){
		return;
	}
	ch = m_sValue[0];


	for(index = 0; index < nLen; index++){
		if(m_sValue[index] != '0'){ // 直到第一个不是0的字符
			break;
		}
	}

	if(index < nLen){ // 不是全 0 的情况
		m_sValue = m_sValue.Mid(index); // 将第一个非 0 字符前面的 0 去掉
		nLen = m_sValue.GetLength();
		int nDotPos = m_sValue.Find('.');
		if(nDotPos != -1){ // 含有小数点
			for(index = nLen - 1; index >= 0; index--){			
				if(m_sValue[index] != '0')
					break;
			}
			m_sValue = m_sValue.Left(index + 1); // 去掉了小数后面的0
		}

		nLen = m_sValue.GetLength();
		if(nDotPos == nLen - 1){ // 小数点在最后		
			m_sValue = m_sValue.Left(nLen - 1);
		}
		if(nDotPos == 0){ // 小数点在最开始
			m_sValue = _T("0") + m_sValue;
		}
	}else{ // 全 0
		m_sValue = '0';
	}
	SetSel(0, -1);
	ReplaceSel(m_sValue);
}
