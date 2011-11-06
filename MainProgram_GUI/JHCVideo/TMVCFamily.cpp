
#include "StdAfx.h"
#include "tianmin/Sa7134Capture.h"
#include "TMVCFamily.h"
#include <sstream>

#pragma comment(lib,"tianmin/Sa7134Capture.lib")
/////////////////////////////////初始化回调相关的函数////////////////////////////////////////////////
unsigned char inline clip255(LONG v)
{
	if(v<0) v=0;
	else if(v>255) v=255;
	return (unsigned char)v;
}
void inline YUY2_TO_RGB24(unsigned char *YUY2buff,unsigned char *RGBbuff,unsigned long dwSize)
{

	unsigned char *orgRGBbuff = RGBbuff;
	for(unsigned long count=0;count<dwSize;count+=4)
	{
		//Y0 U0 Y1 V0
		unsigned char Y0 = *YUY2buff;
		unsigned char U = *(++YUY2buff);
		unsigned char Y1 = *(++YUY2buff);
		unsigned char V = *(++YUY2buff);
		++YUY2buff;

		LONG Y,C,D,E;
		unsigned char R,G,B;

		Y = Y0;
		C = Y-16;
		D = U-128;
		E = V-128;
		R = clip255(( 298 * C           + 409 * E + 128) >> 8);
		G = clip255(( 298 * C - 100 * D - 208 * E + 128) >> 8);
		B = clip255(( 298 * C + 516 * D           + 128) >> 8);

		*(RGBbuff) =   B;          
		*(++RGBbuff) = G;  
		*(++RGBbuff) = R;        

		Y = Y1;
		C = Y-16;
		D = U-128;
		E = V-128;
		R = clip255(( 298 * C           + 409 * E + 128) >> 8);
		G = clip255(( 298 * C - 100 * D - 208 * E + 128) >> 8);
		B = clip255(( 298 * C + 516 * D           + 128) >> 8);
		*(++RGBbuff) =   B;          
		*(++RGBbuff) = G;  
		*(++RGBbuff) = R;        
		++RGBbuff;

	}
}

//天敏VC系列原始视频数据回调函数
void CALLBACK VCAPrcVidCapCallBack(unsigned long dwCard, unsigned char *pbuff,unsigned long dwSize)
{
    //设备的YUY2数据处理(注意：之前要注册回调函数)
	eFieldFrequency frequency;
	VCAGetVidFieldFrq ( dwCard,frequency );
	if (frequency!=FIELD_FREQ_0HZ)  // 有信号
	{
		ULONG nWidth,nHeight;
		VCAGetVidCapSize(dwCard,nWidth,nHeight);
		unsigned char *pYUY2 = new unsigned char[nWidth*nHeight*2*2];
		unsigned char *pRGB = new unsigned char[nWidth*nHeight*4*2];
		for(ULONG i = 0; i < nHeight-1; i++)
		{
			memcpy(pYUY2 + (nHeight-i-1)*nWidth*2*2, pbuff, nWidth * 2);
			memcpy(pYUY2 + (nHeight-i-1)*nWidth*2*2 + nWidth*2, pbuff, nWidth * 2);
			pbuff = pbuff + nWidth*2;
		}
		IplImage *pframe =cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,3);
		YUY2_TO_RGB24(pYUY2,pRGB,dwSize);
		memcpy(pframe->imageData,pRGB,nWidth* nHeight* 3);//原始数据格式为YUY2，需要转化为RGB24才能供IplImage使用
		cvFlip(pframe,NULL);
		VideoDevice::PushIntoImgQueue(pframe);
		cvReleaseImage(&pframe);
		delete []pYUY2;
		delete []pRGB;
	} 
}
/////////////////////////////////结束初始化回调相关的函数////////////////////////////////////////////////
TMVCFamily::TMVCFamily(void)
:m_VideoCardChannelNum(0)
,OpenChannelID(-1)
{
}

TMVCFamily::~TMVCFamily(void)
{
	Close();
	VCAUnInitSdk();
}
bool TMVCFamily::Init(HWND hWnd/*=NULL*/)//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
{
	//采集卡自身SDK的初始化
	if ( !VCAInitSdk(hWnd,PCI_MEMORY_VIDEOMEMORY) )
	{
		//::AfxMessageBox(_T("VC404卡驱动初始化失败！"),MB_OK);
		return false;
	}	
	m_VideoCardChannelNum = VCAGetDevNum();
	if ( m_VideoCardChannelNum==0 )
	{
		//::AfxMessageBox(_T("请确定VC404卡驱动程序是否安装成功！"),MB_OK);
		return false;
	}
	return true;
}

bool TMVCFamily::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//打开视频设备,filepath为打开视频文件时才需要
{
	assert(deviceID>=0 && deviceID<=m_VideoCardChannelNum);//(设备号从0开始) 
	OpenChannelID=-1;
	VCAStopVideoCapture(deviceID);
	VCACloseDevice( deviceID);
	//打开设备
	if(!VCAOpenDevice( deviceID, NULL))
	{
		//CString strTemp;
		//strTemp.Format( _T("采集卡设备打开通道[%d]出错！"), deviceID );
		//::AfxMessageBox( strTemp );
		return false;
	}
	if (!VCARegVidCapCallBack( deviceID, VCAPrcVidCapCallBack))//注册视频捕获原始数据回调.
	{
		return false;
	}
	BOOL bres=VCAStartVideoCapture(deviceID,CAP_ORIGIN_STREAM,MPEG4_AVIFILE_ONLY,"");//开始捕获，不保存为文件，用回调函数自己处理

	if (!bres)
	{
		return false;
	}
	OpenChannelID=deviceID;
	return true;
}
IplImage* TMVCFamily::QueryFrame()//得到一帧图像
{
	return NULL;

}
void TMVCFamily::Close()//关闭视频设备
{
	VCACloseDevice( OpenChannelID);
	VCAStopVideoCapture(OpenChannelID);
	VideoDevice::ClearImgQueue();
}
vector<Videostring> TMVCFamily::GetAllChannelName()//得到该类设备的所有（通道/设备）名称
{
	channelName.clear();
	Videostring strName;
	for (int dwCard=0; dwCard<m_VideoCardChannelNum; ++dwCard)
	{
#ifdef _UNICODE
		std::wstringstream stream;
		stream<<_T("通道")<<dwCard;
		strName=stream.str();
#else
		std::stringstream stream;
		stream<<_T("通道")<<dwCard;
		strName=stream.str();
#endif
		channelName.push_back(strName);
	}
	return channelName;
}