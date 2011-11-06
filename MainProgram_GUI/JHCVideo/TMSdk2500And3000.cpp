#include "StdAfx.h"
#include "TMSdk2500And3000.h"
#include "tianmin/sdk3000_7130.h"
#include <sstream>
#pragma comment(lib,"TIANMIN/sdk3000_7130.lib")

/////////////////////////////////初始化回调相关的函数////////////////////////////////////////////////
inline void YUY2_TO_RGB24(unsigned char *YUY2buff,unsigned char *RGBbuff,unsigned long dwSize)
{
	//R = Y + 1.14V
	//G = Y - 0.39U - 0.58V
	//B = Y + 2.03U
	// R = Y + 1.403V'
	//G = Y - 0.344U' - 0.714V'
	//B = Y + 1.770U'
	unsigned char *orgRGBbuff = RGBbuff;
	for(unsigned long count=0;count<dwSize;count+=4)
	{
		//Y0 U0 Y1 V0
		float Y0 = *YUY2buff;
		float U = *(++YUY2buff);
		float Y1 = *(++YUY2buff);
		float V = *(++YUY2buff);
		++YUY2buff;
		float R,G,B;

		R = (Y0 + 1.14f*V);
		G=(Y0 - 0.39f*U-0.58f*V);  
		B=(Y0 +2.03f*U);
		if(R<0) R =0;
		if(R>255) R=255;
		if(G<0) G =0;
		if(G>255) G=255;
		if(B<0) B =0;
		if(B>255) B=255;
		*(RGBbuff) =     (unsigned char)B;         
		*(++RGBbuff) =  (unsigned char)G;
		*(++RGBbuff) =  (unsigned char)R;
		R = (Y1 + 1.14f*V);
		G=(Y1 - 0.39f*U-0.58f*V);  
		B=(Y1 +2.03f*U)   ;
		if(R<0) R =0;
		if(R>255) R=255;
		if(G<0) G =0;
		if(G>255) G=255;
		if(B<0) B =0;
		if(B>255) B=255;
		*(++RGBbuff) =     (unsigned char)B;         
		*(++RGBbuff) =  (unsigned char)G;
		*(++RGBbuff) =  (unsigned char)R;
		++RGBbuff;
	}
}
//天敏sdk2500 & sdk3000原始视频数据回调函数
void CALLBACK VCAPrcCapSourceStream(long lnCardID, 
	long pBuf, 
	long lnWidth, 
	long lnHeight,
	long lnBiCount )
{
	//设备的YUY2数据处理(注意：之前要注册回调函数)
	unsigned char* pbuff=(unsigned char*)pBuf;
	eFieldFrequency frequency;
	VCAGetVideoFieldFrq ( lnCardID,&frequency );
	if (frequency!=FIELD_FREQ_0HZ)  // 有信号
	{
		unsigned char *pYUY2 = new unsigned char[lnWidth*lnHeight*2*2];
		unsigned char *pRGB = new unsigned char[lnWidth*lnHeight*4*2];
		for(int i = 0; i < lnHeight-1; i++)
		{
			memcpy(pYUY2 + (lnHeight-i-1)*lnWidth*2*2, pbuff, lnWidth * 2);
			memcpy(pYUY2 + (lnHeight-i-1)*lnWidth*2*2 + lnWidth*2, pbuff, lnWidth * 2);
			pbuff = pbuff + lnWidth*2;
		}
		IplImage *pframe =cvCreateImage(cvSize(lnWidth,lnHeight),IPL_DEPTH_8U,3);
		YUY2_TO_RGB24(pYUY2,pRGB,lnWidth * lnHeight * lnBiCount);
		memcpy(pframe->imageData,pRGB,lnWidth* lnHeight* 3);//原始数据格式为YUY2，需要转化为RGB24才能供IplImage使用
		cvFlip(pframe,NULL);
		VideoDevice::PushIntoImgQueue(pframe);
		cvReleaseImage(&pframe);
		delete []pYUY2;
		delete []pRGB;
	} 
}
/////////////////////////////////结束初始化回调相关的函数////////////////////////////////////////////////
TMSdk2500And3000::TMSdk2500And3000(void)
	:m_VideoCardChannelNum(0)
{
}

TMSdk2500And3000::~TMSdk2500And3000(void)
{
	Close();
	VCAUnInitSdk();
}
bool TMSdk2500And3000::Init(HWND hWnd/*=NULL*/)//初始化视频设备,hWnd(可选)为设备的overlay窗口句柄
{
	//采集卡自身SDK的初始化
	if ( S_OK!=VCAInitSdk())
	{
		return false;
	}
	int Num;
	if ( S_OK!=VCAGetDevNum(&Num))
	{
		return false;
	}
	m_VideoCardChannelNum=Num;
	return true;
}

bool TMSdk2500And3000::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//打开视频设备,filepath为打开视频文件时才需要
{
	assert(deviceID>=0 && deviceID<m_VideoCardChannelNum);//(设备号从0开始) 
	OpenChannelID=-1;
	VCAStopCapture(deviceID);
	VCADisConnectDevice( deviceID);
	//打开设备
	if( S_OK != VCAConnectDevice( deviceID, 
		TRUE, NULL, CSize(720,576), VIDEO_IN_1,25, VideoSubType_YUY2 ))
	{
		return false;
	}
	if ( S_OK!=VCAEnableCapSourceStream( deviceID, TRUE,Odd_Field,VCAPrcCapSourceStream))//注册视频捕获原始数据回调.
	{
		return false;
	}
	if ( S_OK!=VCAStartCapture(deviceID))//开始捕获，不保存为文件，用回调函数自己处理
	{
		return false;
	}
	OpenChannelID=deviceID;
	return true;
}
IplImage* TMSdk2500And3000::QueryFrame()//得到一帧图像
{
	return NULL;

}
void TMSdk2500And3000::Close()//关闭视频设备
{
	VCAStopCapture(OpenChannelID);
	VCADisConnectDevice( OpenChannelID);
	VideoDevice::ClearImgQueue();
}
vector<Videostring> TMSdk2500And3000::GetAllChannelName()//得到该类设备的所有（通道/设备）名称
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
