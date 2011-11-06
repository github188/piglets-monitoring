#include "Usbvideo.h"
#include "CameraDS.h"

Usbvideo::Usbvideo(void)
:VideoDevice(false)
,m_img(NULL)
{

}

Usbvideo::~Usbvideo(void)
{
	Close();
}
bool Usbvideo::Init(HWND hWnd/*=NULL*/)//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
{
	return true;
}
bool Usbvideo::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//打开视频设备,filepath为打开视频文件时才需要
{
	assert(deviceID>=0 && deviceID<=GetChannelNum());//(设备号从0开始) 
	Close();
	assert(deviceID<=GetChannelNum());
	isCapture=m_pCap.OpenCamera(deviceID,false);
	return isCapture;
}
IplImage* Usbvideo::QueryFrame()//得到一帧图像
{
	//m_img=cvQueryFrame(m_pCap);
	//VideoDevice::PushIntoImgQueue(m_img);
	//return m_img;
	m_img=m_pCap.QueryFrame();
	VideoDevice::PushIntoImgQueue(m_img);
	return m_img;
}
void Usbvideo::Close()//关闭视频设备
{
	m_pCap.CloseCamera();
	isCapture=false;
	VideoDevice::ClearImgQueue();
}
vector<Videostring> Usbvideo::GetAllChannelName()//得到该类设备的所有（通道/设备）名称
{
	channelName.clear();
	//仅仅获取摄像头数目
	int cam_count = CCameraDS::CameraCount();
	//获取所有摄像头的名称
	if(cam_count==0)
		return channelName;
	for(int i=0; i < cam_count; i++)
	{
		char camera_name[1024];  
		int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name) );
		if(retval >0)
		{
			string strName;
			strName.assign(camera_name);
#ifdef _UNICODE
			wstring Ansifilepath=s2ws(strName);
			channelName.push_back(Ansifilepath);
#else
			channelName.push_back(strName);
#endif
		}
	}
	return channelName;
}
