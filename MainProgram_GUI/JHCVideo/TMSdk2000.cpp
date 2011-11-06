#include "StdAfx.h"
#include "TMSdk2000.h"
#include "tianmin/DSStream.h"
#pragma comment(lib,"TIANMIN/DSStream.lib")

/////////////////////////////////初始化回调相关的函数///////////////////////////////////////////////
//天敏SDK2000原始视频数据回调函数
//pDIBHead 指向 DIB 头（即 BITMAPINFO*），pDIBits 指向点阵数据，pParam 是用户在DSStream_GetVideoStream中自定义的参数。
void CALLBACK StreamNotify(const BYTE* pDIBHead, const BYTE* pDIBits, LPVOID pParam)
{
	//先前已设定回调数据格式为RGB24
	BITMAPINFO *bmi=(BITMAPINFO*)pDIBHead;
	ULONG nWidth,nHeight;
	nWidth=bmi->bmiHeader.biWidth;
	nHeight=bmi->bmiHeader.biHeight>0 ? bmi->bmiHeader.biHeight:-bmi->bmiHeader.biHeight;
	IplImage *pframe =cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,bmi->bmiHeader.biBitCount >>3);
	int imgsize=bmi->bmiHeader.biHeight*((bmi->bmiHeader.biWidth * bmi->bmiHeader.biBitCount /8 +3) & (-4));
	memcpy((unsigned char*)pframe->imageData,pDIBits,imgsize);
	cvFlip(pframe,NULL);
	VideoDevice::PushIntoImgQueue(pframe);
	cvReleaseImage(&pframe);
}
/////////////////////////////////结束初始化回调相关的函数////////////////////////////////////////////////
TMSdk2000::TMSdk2000(void)
:m_VideoCardDevNum(0)
{
}

TMSdk2000::~TMSdk2000(void)
{
	Close();
	DSStream_Uninitialize();
}
bool TMSdk2000::Init(HWND hWnd/*=NULL*/)//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
{
	//采集卡自身SDK的初始化
	HRESULT hr=DSStream_Initialize();
	if (FAILED(hr))
	{
		return false;
	}
	int CardNum=0;
	hr = DSStream_GetCardNumber(&CardNum);
	m_VideoCardDevNum=CardNum;
	if(FAILED(hr) || m_VideoCardDevNum<=0)
	{
		//AfxMessageBox("SDK初始化失败 或 无可用的卡", MB_OK|MB_ICONSTOP, 0);
		return false;
	}
	return true;
}

bool TMSdk2000::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//打开视频设备,filepath为打开视频文件时才需要
{
	assert(deviceID>=0 && deviceID<m_VideoCardDevNum);//(设备号从0开始,一张卡为一个设备)
	int nPin=2;//设备的引脚，即设备上连接摄像头的端口。对于 SDK-2000 卡，范围是 0-2；对于四路卡，范围是 0-4。
	/*////////打开设备：基于引脚号（默认是基于设备的）
    nPin=deviceID;
	deviceID=0;//即只有一个设备（一张卡）
	*/////////////////////////////////////////////////
	DSStream_DisconnectDevice(deviceID);
	const BOOL bOverlay = FALSE;
	HRESULT hr = DSStream_ConnectDevice(deviceID, bOverlay);
	//DSStream_SetOwnerWnd(deviceID,NULL);
	if(FAILED(hr))
	{
		//AfxMessageBox("连接视频卡失败!", MB_OK, 0);
		return false;
	}
	VIDEOSTREAMINFO vsi;
	DSStream_SetVideoStandard(deviceID,VideoStandard_PAL_D); 	//设置视频制式
	DSStream_RouteInPinToOutPin(deviceID,nPin,0); 	//设置视频源

	DSStream_SetVideoPropertyValue(deviceID,VideoProperty_Brightness,5000); ////设置视频属性：亮度、对比度、色度、饱和度。可根据需要更改 
	DSStream_SetVideoPropertyValue(deviceID,VideoProperty_Contrast,5000); 
	DSStream_SetVideoPropertyValue(deviceID,VideoProperty_Hue,5000); 
	DSStream_SetVideoPropertyValue(deviceID,VideoProperty_Saturation,5000); 
	DSStream_GetVideoInfo(deviceID, &vsi, PREVIEW);
	//vsi.bmiHeader.biWidth=800; //设置图像分辨率。可根据需要更改。 
	//vsi.bmiHeader.biHeight=600;
	vsi.subtype=VideoSubType_RGB24;
	DSStream_DisconnectPin(deviceID,PREVIEW); 
	DSStream_SetVideoInfo(deviceID,vsi,PREVIEW); 
	DSStream_ConnectPin(deviceID,PREVIEW);
	hr = DSStream_GetVideoStream(deviceID, StreamNotify, NULL);

	DestorySdk2000DefaultWnd();//DSStream_ConnectDevice把你窗口设置为NULL后，它还是会弹出一个ActiveMovie Window窗口出来，
	                           //故写此函数把这个弹窗隐藏掉（不能关闭此窗口，一关闭视频就不回调了）。
	if(FAILED(hr))
	{
		//MessageBox("开始数据流回调失败!");
		return false;
	}
	return true;
}
IplImage* TMSdk2000::QueryFrame()//得到一帧图像
{
	return NULL;
}
void TMSdk2000::Close()//关闭视频设备
{
	for (int dwCard=0; dwCard<m_VideoCardDevNum; ++dwCard)
	{
		HRESULT hr = DSStream_DisconnectDevice(dwCard);
    }	
	VideoDevice::ClearImgQueue();
}
vector<Videostring> TMSdk2000::GetAllChannelName()//得到该类设备的所有（通道/设备）名称
{
	channelName.clear();
	Videostring strName;
	char szDeviceName[MAX_DEVICE_NUM][MAX_DEVICE_NAME_LEN];
	const char sz10Moons[] = {"10Moons"};
	int nCardNum=0;
	//寻找可用的卡，剔除非10Moons卡
	HRESULT hr = DSStream_EnumVideoCaptureDev(szDeviceName, &nCardNum);
	m_VideoCardDevNum=nCardNum;
	if(FAILED(hr))
	{
		//AfxMessageBox("枚举10Moons卡失败", MB_OK, 0);
		return channelName;
	}	
	for (int dwCard=0; dwCard<m_VideoCardDevNum; ++dwCard)
	{
		string tmpstr;
		tmpstr.assign(szDeviceName[dwCard]);
		szDeviceName[dwCard][7] = '\0';
		if(_stricmp(szDeviceName[dwCard], sz10Moons) != 0) //不是sdk卡
		{
			continue;
		}
#ifdef _UNICODE
		strName=s2ws(tmpstr);
#else
		strName=tmpstr;
#endif
		channelName.push_back(strName);
	}
	m_VideoCardDevNum=channelName.size();
	return channelName;
}
//当前是否处于四路同屏模式
bool TMSdk2000::Is4WayCard(int iCardID)
{
	long source = 0;
	DSStream_WhatInPinRouteToOutPin(iCardID, 0, &source);
	return (source==4);
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) //满足指定条件，则隐藏窗口
{

	TCHAR wtitle[512];
	GetWindowText(hwnd, wtitle, sizeof(wtitle));
	if (_tcsstr(wtitle , _T("ActiveMovie Window"))) //窗口标题含有ActiveMovie Window
	{
		//::SendMessage(hwnd, WM_CLOSE, 0, 0);
		::ShowWindow(hwnd,SW_HIDE);
	}
	return (true); //不要忘了
}
void TMSdk2000::DestorySdk2000DefaultWnd()
{
	EnumWindows(EnumWindowsProc, 0); //枚举所有窗口
}