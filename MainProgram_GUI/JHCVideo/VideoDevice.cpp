#include "VideoDevice.h"

CCriticalSection g_cs_imgqueue;
CCriticalSection g_cs_analysis;
CvvImage VideoDevice::m_ShowImg;//��̬��Ա������ʼ��,����Ĭ���޲ι��캯��
queue<IplImage*> VideoDevice::img_queue;//��̬��Ա������ʼ��,����Ĭ���޲ι��캯��

VideoDevice::VideoDevice(bool isUsingCallBack/*=true*/)
:isCapture(false)
,usingCallback(isUsingCallBack)
{
}

VideoDevice::~VideoDevice(void)
{
}
bool VideoDevice::DrawImgToHDC( HDC hDCDst, CRect rect,IplImage *img )
{
	if (!img || !hDCDst)
	{
		return false;
	}
	m_ShowImg.CopyOf(img,1);    // �˴��ڶ�������������CopyOf��Ĭ�ϲ���-1��������1�� ����8λ��ͨ��ͼ������ʾ
	m_ShowImg.DrawToHDC(hDCDst,&rect);
	return true;
}
void VideoDevice::PushIntoImgQueue(IplImage *pframe)
{
	if (!pframe)
	{
		return;
	}
	g_cs_imgqueue.Lock();
	IplImage *img=cvCloneImage(pframe);
	VideoDevice::img_queue.push(img);
	g_cs_imgqueue.Unlock();
}
void VideoDevice::ClearImgQueue()
{
	g_cs_imgqueue.Lock();
	while (!VideoDevice::img_queue.empty())//���ͼ�����
	{
		IplImage *img=VideoDevice::img_queue.front();
		cvReleaseImage(&img);
		VideoDevice::img_queue.pop();
	}
	g_cs_imgqueue.Unlock();
}
IplImage* VideoDevice::GetFrontImgQueue()
{
	g_cs_imgqueue.Lock();
	IplImage *img=NULL;
	if(!VideoDevice::img_queue.empty())//���ͼ�����
	{
		img=VideoDevice::img_queue.front();
	}
	g_cs_imgqueue.Unlock();
	return img;
}
void VideoDevice::PopImgQueue()
{
	g_cs_imgqueue.Lock();
	if(!VideoDevice::img_queue.empty())//���ͼ�����
	{
		IplImage *img=VideoDevice::img_queue.front();
		cvReleaseImage(&img);
		VideoDevice::img_queue.pop();
	}
	g_cs_imgqueue.Unlock();
}
size_t VideoDevice::GetQueueSize()
{ 
	g_cs_imgqueue.Lock();
	size_t si=img_queue.size();
	g_cs_imgqueue.Unlock();
	return si;
}

std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::wstring s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs"); 
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return result;
}