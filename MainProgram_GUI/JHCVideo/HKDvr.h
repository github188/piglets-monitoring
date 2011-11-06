#pragma once
#include "videodevice.h"
#include "hikang/HCNetSDK.h"
//设备信息列表结构
typedef struct _HKDeviceInfo {
	long   lID;         //设备iD,为界面显示辅助用
	string DeviceName;//设备名称,为界面显示辅助用
	string IPAdd;     //设备IP地址字符
	long   lPort;       //登录端口号
	string UserNanme; //登录用户名
	string Pwd;       //登录密码
	NET_DVR_DEVICEINFO_V30 DeviceInfo;//设备参数
}HKDeviceInfo,*PHKDeviceInfo;
class HKDvr :
	public VideoDevice
{
public:
	HKDvr(void);
	virtual ~HKDvr(void);
	HKDeviceInfo devInfo;
	long m_lUserID;//登录设备后返回的用户ID
	long m_lRealPlay;//预览返回值
	long m_lPort;////用于保存软解码的播放库port号(播放器与有通道，也设备的通道不相干)
	size_t m_VideoDeviceChannelNum;
public:
	bool Init(HWND hWnd=NULL);//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
	bool Open(size_t nChannel,Videostring filepath=_T(""));//打开视频设备,filepath为打开视频文件时才需要
	IplImage* QueryFrame();//得到一帧图像
	void Close();//关闭视频设备
	vector<Videostring> GetAllChannelName();//得到该类设备的所有（通道/设备）名称
public:
	//重写
	void SetDeviceInfo(string ipAdd, long port, string userName, string password);
	size_t GetChannelNum()const { return m_VideoDeviceChannelNum;};//得到该类设备的总（通道/设备）数
};
