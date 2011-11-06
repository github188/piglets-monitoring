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
bool Filevideo::Init(HWND hWnd/*=NULL*/)//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
{
	return true;
}
bool Filevideo::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//打开视频设备,filepath为打开视频文件时才需要
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
IplImage* Filevideo::QueryFrame()//得到一帧图像
{
	m_img=cvQueryFrame(m_pCap);
	VideoDevice::PushIntoImgQueue(m_img);
	return m_img;
}
void Filevideo::Close()//关闭视频设备
{
	if (m_pCap)
	{
		cvReleaseCapture(&m_pCap);
		m_pCap=NULL;
	}
	isCapture=false;
	VideoDevice::ClearImgQueue();
}
vector<Videostring> Filevideo::GetAllChannelName()//得到该类设备的所有（通道/设备）名称
{
	channelName.clear();
	channelName.push_back(_T("AVI文件视频"));
	return channelName;
}
