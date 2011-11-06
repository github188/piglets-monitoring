#pragma once
#include "videodevice.h"

class Filevideo :
	public VideoDevice//要public继承，不然无法把指针赋值给基类(VideoDevice类)指针
{
public:
	Filevideo(void);
	virtual ~Filevideo(void);
public://重写纯虚函数
	bool Init(HWND hWnd=NULL);//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
	bool Open(size_t deviceID,Videostring filepath=_T(""));//打开视频设备,filepath为打开视频文件时才需要
	IplImage* QueryFrame();//得到一帧图像
	void Close();//关闭视频设备
	vector<Videostring> GetAllChannelName();//得到该类设备的所有（通道/设备）名称
private:
	CvCapture *m_pCap;
	IplImage *m_img;//从CvCapture返回的函数，不需要用户释放
};
