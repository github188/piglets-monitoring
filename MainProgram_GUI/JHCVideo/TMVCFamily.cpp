
#include "StdAfx.h"
#include "tianmin/Sa7134Capture.h"
#include "TMVCFamily.h"
#include <sstream>

#pragma comment(lib,"tianmin/Sa7134Capture.lib")
/////////////////////////////////��ʼ���ص���صĺ���////////////////////////////////////////////////
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

//����VCϵ��ԭʼ��Ƶ���ݻص�����
void CALLBACK VCAPrcVidCapCallBack(unsigned long dwCard, unsigned char *pbuff,unsigned long dwSize)
{
    //�豸��YUY2���ݴ���(ע�⣺֮ǰҪע��ص�����)
	eFieldFrequency frequency;
	VCAGetVidFieldFrq ( dwCard,frequency );
	if (frequency!=FIELD_FREQ_0HZ)  // ���ź�
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
		memcpy(pframe->imageData,pRGB,nWidth* nHeight* 3);//ԭʼ���ݸ�ʽΪYUY2����Ҫת��ΪRGB24���ܹ�IplImageʹ��
		cvFlip(pframe,NULL);
		VideoDevice::PushIntoImgQueue(pframe);
		cvReleaseImage(&pframe);
		delete []pYUY2;
		delete []pRGB;
	} 
}
/////////////////////////////////������ʼ���ص���صĺ���////////////////////////////////////////////////
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
bool TMVCFamily::Init(HWND hWnd/*=NULL*/)//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
{
	//�ɼ�������SDK�ĳ�ʼ��
	if ( !VCAInitSdk(hWnd,PCI_MEMORY_VIDEOMEMORY) )
	{
		//::AfxMessageBox(_T("VC404��������ʼ��ʧ�ܣ�"),MB_OK);
		return false;
	}	
	m_VideoCardChannelNum = VCAGetDevNum();
	if ( m_VideoCardChannelNum==0 )
	{
		//::AfxMessageBox(_T("��ȷ��VC404�����������Ƿ�װ�ɹ���"),MB_OK);
		return false;
	}
	return true;
}

bool TMVCFamily::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
{
	assert(deviceID>=0 && deviceID<=m_VideoCardChannelNum);//(�豸�Ŵ�0��ʼ) 
	OpenChannelID=-1;
	VCAStopVideoCapture(deviceID);
	VCACloseDevice( deviceID);
	//���豸
	if(!VCAOpenDevice( deviceID, NULL))
	{
		//CString strTemp;
		//strTemp.Format( _T("�ɼ����豸��ͨ��[%d]����"), deviceID );
		//::AfxMessageBox( strTemp );
		return false;
	}
	if (!VCARegVidCapCallBack( deviceID, VCAPrcVidCapCallBack))//ע����Ƶ����ԭʼ���ݻص�.
	{
		return false;
	}
	BOOL bres=VCAStartVideoCapture(deviceID,CAP_ORIGIN_STREAM,MPEG4_AVIFILE_ONLY,"");//��ʼ���񣬲�����Ϊ�ļ����ûص������Լ�����

	if (!bres)
	{
		return false;
	}
	OpenChannelID=deviceID;
	return true;
}
IplImage* TMVCFamily::QueryFrame()//�õ�һ֡ͼ��
{
	return NULL;

}
void TMVCFamily::Close()//�ر���Ƶ�豸
{
	VCACloseDevice( OpenChannelID);
	VCAStopVideoCapture(OpenChannelID);
	VideoDevice::ClearImgQueue();
}
vector<Videostring> TMVCFamily::GetAllChannelName()//�õ������豸�����У�ͨ��/�豸������
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