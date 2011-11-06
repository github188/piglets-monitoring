#pragma once
#include "videodevice.h"
#include "DaHua/dhassistant.h"
#include "DaHua/dhnetsdk.h"
//大华DVR类，支持所有大华网络硬盘录像机、网络视频服务器、网络摄像机、网络球机等网络监控产品
//设备信息列表结构
typedef struct _DHDeviceInfo {
	long   lID;         //设备iD,为界面显示辅助用
	string DeviceName;//设备名称,为界面显示辅助用
	string IPAdd;     //设备IP地址字符
	long   lPort;       //登录端口号
	string UserNanme; //登录用户名
	string Pwd;       //登录密码
	NET_DEVICEINFO DeviceInfo;//设备参数
}DHDeviceInfo,*PDHDeviceInfo;
class DHDvr :
	public VideoDevice
{
public:
	DHDvr(void);
	virtual ~DHDvr(void);
	DHDeviceInfo devInfo;
	long m_lUserID;//登录设备后返回的用户ID
	long m_lRealPlay;//预览返回值
	long m_lPort;////用于保存软解码的播放库port号(播放器与有通道，也设备的通道不相干)
	size_t m_VideoDeviceChannelNum;
public:
	bool Init(HWND hWnd=NULL);//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
	bool Open(size_t deviceID,Videostring filepath=_T(""));//打开视频设备,filepath为打开视频文件时才需要
	IplImage* QueryFrame();//得到一帧图像
	void Close();//关闭视频设备
	vector<Videostring> GetAllChannelName();//得到该类设备的所有（通道/设备）名称
	HWND m_hwnd;
public:
	//重写
	void SetDeviceInfo(string ipAdd, long port, string userName, string password);
	size_t GetChannelNum()const { return m_VideoDeviceChannelNum;};//得到该类设备的总（通道/设备）数
};
