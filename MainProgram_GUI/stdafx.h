
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��






#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <queue>
#include <map>
#include <io.h>
using namespace std;
//otl�޷�ʹ��like��ѯ���������������������like��ѯ�ľ�̬sql���
#define OTL_ODBC // Compile OTL 4.0/ODBC���������#include "otlv4.h"ǰ
// #define OTL_ODBC_UNIX // �����Unix��ʹ��UnixODBC������Ҫ�����
#define OTL_STL // Turn on STL features��������otl_stream��ʹ��<<,>>����string��STL�е���������
#if (defined(UNICODE)||defined(_UNICODE))
#define OTL_UNICODE_EXCEPTION_AND_RLOGON// Enable OTL Unicode rlogon and otl_exception for ODBC
#if defined (__GNUC__)
#define OTL_UNICODE_CHAR_TYPE SQLWCHAR
#else
#define OTL_UNICODE_CHAR_TYPE wchar_t
#endif
#endif
#include "otlv4.h"
#pragma comment(lib,"odbc32.lib")
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#ifdef _DEBUG
#pragma comment(lib,"ipcommd.lib")
#pragma comment(lib,"ipstddatad.lib")
#pragma comment(lib,"ipguid.lib")
#pragma comment(lib,"ipalgod.lib")
#pragma comment(lib,"iplearnd.lib")
#else
#pragma comment(lib,"ipcomm.lib")
#pragma comment(lib,"ipstddata.lib")
#pragma comment(lib,"ipgui.lib")
#pragma comment(lib,"ipalgo.lib")
#pragma comment(lib,"iplearn.lib")
#endif



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif