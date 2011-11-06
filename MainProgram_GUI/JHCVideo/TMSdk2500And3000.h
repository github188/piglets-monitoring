#pragma once
#include "videodevice.h"
//天敏Dvr card,芯片Sa7130,支持10MOONS SDK3000/2500系统产品
class TMSdk2500And3000 :
	public VideoDevice
{
public:
	TMSdk2500And3000(void);
	virtual ~TMSdk2500And3000(void);
	size_t m_VideoCardChannelNum;
	size_t OpenChannelID;
public:
	bool Init(HWND hWnd=NULL);//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
	bool Open(size_t deviceID,Videostring filepath=_T(""));//打开视频设备,filepath为打开视频文件时才需要
	IplImage* QueryFrame();//得到一帧图像
	void Close();//关闭视频设备
	vector<Videostring> GetAllChannelName();//得到该类设备的所有（通道/设备）名称
public:
	//重写
	size_t GetChannelNum()const { return m_VideoCardChannelNum;};//得到该类设备的总（通道/设备）数
};
