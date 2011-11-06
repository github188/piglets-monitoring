#include "Filevideo.h"

Filevideo::Filevideo(void)
:VideoDevice(false)
,m_pCap(NULL)
,m_img(NULL)
{
}

Filevideo::~Filevideo(void)
{
	Close();
}
bool Filevideo::Init(HWND hWnd/*=NULL*/)//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
{
	return true;
}
bool Filevideo::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
{
	Close();
#ifdef _UNICODE
	string Ansifilepath=ws2s(filepath);
	m_pCap = cvCreateFileCapture(Ansifilepath.c_str());
#else
	m_pCap = cvCreateFileCapture(filepath.c_str());
#endif
	isCapture=(m_pCap!=NULL);
	return isCapture;
}
IplImage* Filevideo::QueryFrame()//�õ�һ֡ͼ��
{
	m_img=cvQueryFrame(m_pCap);
	VideoDevice::PushIntoImgQueue(m_img);
	return m_img;
}
void Filevideo::Close()//�ر���Ƶ�豸
{
	if (m_pCap)
	{
		cvReleaseCapture(&m_pCap);
		m_pCap=NULL;
	}
	isCapture=false;
	VideoDevice::ClearImgQueue();
}
vector<Videostring> Filevideo::GetAllChannelName()//�õ������豸�����У�ͨ��/�豸������
{
	channelName.clear();
	channelName.push_back(_T("AVI�ļ���Ƶ"));
	return channelName;
}
