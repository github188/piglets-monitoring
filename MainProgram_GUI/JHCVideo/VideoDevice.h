#pragma once
#include <string>
#include <vector>
#include <queue>
#include <afxmt.h>
#include "cv.h"
#include "highgui.h"
using std::string;
using std::wstring;
using std::vector;
using std::queue;
#if CV_MAJOR_VERSION==2 && CV_MINOR_VERSION==1
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
#elif  CV_MAJOR_VERSION==2 && CV_MINOR_VERSION==0
	#ifdef _DEBUG
	#pragma comment(lib,"cv200d.lib")
	#pragma comment(lib,"cxcore200d.lib")
	#pragma comment(lib,"highgui200d.lib")
	#pragma comment(lib,"cvaux200d.lib")
	#else
	#pragma comment(lib,"cv200.lib")
	#pragma comment(lib,"cxcore200.lib")
	#pragma comment(lib,"highgui200.lib")
	#pragma comment(lib,"cvaux200.lib")
	#endif
#endif

#ifdef _UNICODE
typedef wstring  Videostring; 
#else
typedef string   Videostring;
#endif
// 临界区对象
extern CCriticalSection g_cs_imgqueue;
extern CCriticalSection g_cs_analysis;

class VideoDevice
{
public:
	VideoDevice(bool isUsingCallBack=true);
	virtual ~VideoDevice(void);
	bool IsCaptureing()const { return isCapture;};//设备是否正在捕获视频
	bool IsUsingCallBack()const { return usingCallback;};//设备是否使用回调函数捕获图像序列
public://静态函数
	static void PushIntoImgQueue(IplImage *m_pframe);
	static size_t GetQueueSize();
	static IplImage* GetFrontImgQueue();
	static void PopImgQueue();
	static void ClearImgQueue();
	static bool DrawImgToHDC( HDC hDCDst, CRect rect,IplImage *img );//把图像img显示在控件hDCDst上
public://纯虚函数
	virtual bool Init(HWND hWnd=NULL)=0;//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
	virtual bool Open(size_t deviceID,Videostring filepath=_T(""))=0;//打开视频设备,filepath为打开视频文件时才需要
	virtual IplImage* QueryFrame()=0;//得到一帧图像,使用回调函数的设备则不需要调用些函数
	virtual void Close()=0;//关闭视频设备
	virtual vector<Videostring> GetAllChannelName()=0;//得到该类设备的所有（通道/设备）名称
public://虚函数，可重写
	virtual void SetDeviceInfo(string ipAdd, long port, string userName, string password){return;};
	virtual size_t GetChannelNum()const { return channelName.size();};//得到该类设备的总（通道/设备）数
protected:
	vector<Videostring> channelName;//默认为空
	bool isCapture;//=false;
	bool usingCallback;//=true;是否使用回调函数采集图像序列（默认是，只有文件与USB接口类为是）
protected://静态变量
	static CvvImage m_ShowImg;
	static queue<IplImage*>img_queue;//存储图像的队列
};
//全局函数
std::string ws2s(const std::wstring& ws);//把wstring类型字符串转化为string类型
std::wstring s2ws(const std::string& s);//把string类型字符串转化为wstring类型