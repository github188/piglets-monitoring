// NumericEdit.cpp : ʵ���ļ�
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



// CNumericEdit ��Ϣ�������



void CNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString ss;
	char ch = char(nChar);
	GetWindowText(ss);  // ��õ�ǰ��������ַ�����

	// �Ե�ǰ�����ַ������ж�
	if(!isdigit(ch) && ch!= '\b' ){// ��������������˸��
		if(ch == '.'){ //��С����
			if(ss.Find('.') != -1){ // ����Ƿ��Ѿ�������С����
				return;
			}
		}else if (!((ch == '-' || ch=='+') && ss.IsEmpty())){//�����������ڵ�һλ
				return;
		}
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
//�����������ģ��CEdit�����ֹ淶���Ĺ��ܣ�Ҳ��������0011-->11��11.100 ->11.1  .1100->0.11
void CNumericEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: �ڴ˴������Ϣ����������
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
		if(m_sValue[index] != '0'){ // ֱ����һ������0���ַ�
			break;
		}
	}

	if(index < nLen){ // ����ȫ 0 �����
		m_sValue = m_sValue.Mid(index); // ����һ���� 0 �ַ�ǰ��� 0 ȥ��
		nLen = m_sValue.GetLength();
		int nDotPos = m_sValue.Find('.');
		if(nDotPos != -1){ // ����С����
			for(index = nLen - 1; index >= 0; index--){			
				if(m_sValue[index] != '0')
					break;
			}
			m_sValue = m_sValue.Left(index + 1); // ȥ����С�������0
		}

		nLen = m_sValue.GetLength();
		if(nDotPos == nLen - 1){ // С���������		
			m_sValue = m_sValue.Left(nLen - 1);
		}
		if(nDotPos == 0){ // С�������ʼ
			m_sValue = _T("0") + m_sValue;
		}
	}else{ // ȫ 0
		m_sValue = '0';
	}
	SetSel(0, -1);
	ReplaceSel(m_sValue);
}
