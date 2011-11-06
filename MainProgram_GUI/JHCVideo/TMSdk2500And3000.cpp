#include "StdAfx.h"
#include "TMSdk2500And3000.h"
#include "tianmin/sdk3000_7130.h"
#include <sstream>
#pragma comment(lib,"TIANMIN/sdk3000_7130.lib")

/////////////////////////////////��ʼ���ص���صĺ���////////////////////////////////////////////////
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
//����sdk2500 & sdk3000ԭʼ��Ƶ���ݻص�����
void CALLBACK VCAPrcCapSourceStream(long lnCardID, 
	long pBuf, 
	long lnWidth, 
	long lnHeight,
	long lnBiCount )
{
	//�豸��YUY2���ݴ���(ע�⣺֮ǰҪע��ص�����)
	unsigned char* pbuff=(unsigned char*)pBuf;
	eFieldFrequency frequency;
	VCAGetVideoFieldFrq ( lnCardID,&frequency );
	if (frequency!=FIELD_FREQ_0HZ)  // ���ź�
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
		memcpy(pframe->imageData,pRGB,lnWidth* lnHeight* 3);//ԭʼ���ݸ�ʽΪYUY2����Ҫת��ΪRGB24���ܹ�IplImageʹ��
		cvFlip(pframe,NULL);
		VideoDevice::PushIntoImgQueue(pframe);
		cvReleaseImage(&pframe);
		delete []pYUY2;
		delete []pRGB;
	} 
}
/////////////////////////////////������ʼ���ص���صĺ���////////////////////////////////////////////////
TMSdk2500And3000::TMSdk2500And3000(void)
	:m_VideoCardChannelNum(0)
{
}

TMSdk2500And3000::~TMSdk2500And3000(void)
{
	Close();
	VCAUnInitSdk();
}
bool TMSdk2500And3000::Init(HWND hWnd/*=NULL*/)//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
{
	//�ɼ�������SDK�ĳ�ʼ��
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

bool TMSdk2500And3000::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
{
	assert(deviceID>=0 && deviceID<m_VideoCardChannelNum);//(�豸�Ŵ�0��ʼ) 
	OpenChannelID=-1;
	VCAStopCapture(deviceID);
	VCADisConnectDevice( deviceID);
	//���豸
	if( S_OK != VCAConnectDevice( deviceID, 
		TRUE, NULL, CSize(720,576), VIDEO_IN_1,25, VideoSubType_YUY2 ))
	{
		return false;
	}
	if ( S_OK!=VCAEnableCapSourceStream( deviceID, TRUE,Odd_Field,VCAPrcCapSourceStream))//ע����Ƶ����ԭʼ���ݻص�.
	{
		return false;
	}
	if ( S_OK!=VCAStartCapture(deviceID))//��ʼ���񣬲�����Ϊ�ļ����ûص������Լ�����
	{
		return false;
	}
	OpenChannelID=deviceID;
	return true;
}
IplImage* TMSdk2500And3000::QueryFrame()//�õ�һ֡ͼ��
{
	return NULL;

}
void TMSdk2500And3000::Close()//�ر���Ƶ�豸
{
	VCAStopCapture(OpenChannelID);
	VCADisConnectDevice( OpenChannelID);
	VideoDevice::ClearImgQueue();
}
vector<Videostring> TMSdk2500And3000::GetAllChannelName()//�õ������豸�����У�ͨ��/�豸������
{
	channelName.clear();
	Videostring strName;
	for (int dwCard=0; dwCard<m_VideoCardChannelNum; ++dwCard)
	{
#ifdef _UNICODE
		std::wstringstream stream;
		stream<<_T("ͨ��")<<dwCard;
		strName=stream.str();
#else
		std::stringstream stream;
		stream<<_T("ͨ��")<<dwCard;
		strName=stream.str();
#endif
		channelName.push_back(strName);
	}
	return channelName;

}
