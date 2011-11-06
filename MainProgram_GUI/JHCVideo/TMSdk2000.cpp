#include "StdAfx.h"
#include "TMSdk2000.h"
#include "tianmin/DSStream.h"
#pragma comment(lib,"TIANMIN/DSStream.lib")

/////////////////////////////////��ʼ���ص���صĺ���///////////////////////////////////////////////
//����SDK2000ԭʼ��Ƶ���ݻص�����
//pDIBHead ָ�� DIB ͷ���� BITMAPINFO*����pDIBits ָ��������ݣ�pParam ���û���DSStream_GetVideoStream���Զ���Ĳ�����
void CALLBACK StreamNotify(const BYTE* pDIBHead, const BYTE* pDIBits, LPVOID pParam)
{
	//��ǰ���趨�ص����ݸ�ʽΪRGB24
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
/////////////////////////////////������ʼ���ص���صĺ���////////////////////////////////////////////////
TMSdk2000::TMSdk2000(void)
:m_VideoCardDevNum(0)
{
}

TMSdk2000::~TMSdk2000(void)
{
	Close();
	DSStream_Uninitialize();
}
bool TMSdk2000::Init(HWND hWnd/*=NULL*/)//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
{
	//�ɼ�������SDK�ĳ�ʼ��
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
		//AfxMessageBox("SDK��ʼ��ʧ�� �� �޿��õĿ�", MB_OK|MB_ICONSTOP, 0);
		return false;
	}
	return true;
}

bool TMSdk2000::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
{
	assert(deviceID>=0 && deviceID<m_VideoCardDevNum);//(�豸�Ŵ�0��ʼ,һ�ſ�Ϊһ���豸)
	int nPin=2;//�豸�����ţ����豸����������ͷ�Ķ˿ڡ����� SDK-2000 ������Χ�� 0-2��������·������Χ�� 0-4��
	/*////////���豸���������źţ�Ĭ���ǻ����豸�ģ�
    nPin=deviceID;
	deviceID=0;//��ֻ��һ���豸��һ�ſ���
	*/////////////////////////////////////////////////
	DSStream_DisconnectDevice(deviceID);
	const BOOL bOverlay = FALSE;
	HRESULT hr = DSStream_ConnectDevice(deviceID, bOverlay);
	//DSStream_SetOwnerWnd(deviceID,NULL);
	if(FAILED(hr))
	{
		//AfxMessageBox("������Ƶ��ʧ��!", MB_OK, 0);
		return false;
	}
	VIDEOSTREAMINFO vsi;
	DSStream_SetVideoStandard(deviceID,VideoStandard_PAL_D); 	//������Ƶ��ʽ
	DSStream_RouteInPinToOutPin(deviceID,nPin,0); 	//������ƵԴ

	DSStream_SetVideoPropertyValue(deviceID,VideoProperty_Brightness,5000); ////������Ƶ���ԣ����ȡ��Աȶȡ�ɫ�ȡ����Ͷȡ��ɸ�����Ҫ���� 
	DSStream_SetVideoPropertyValue(deviceID,VideoProperty_Contrast,5000); 
	DSStream_SetVideoPropertyValue(deviceID,VideoProperty_Hue,5000); 
	DSStream_SetVideoPropertyValue(deviceID,VideoProperty_Saturation,5000); 
	DSStream_GetVideoInfo(deviceID, &vsi, PREVIEW);
	//vsi.bmiHeader.biWidth=800; //����ͼ��ֱ��ʡ��ɸ�����Ҫ���ġ� 
	//vsi.bmiHeader.biHeight=600;
	vsi.subtype=VideoSubType_RGB24;
	DSStream_DisconnectPin(deviceID,PREVIEW); 
	DSStream_SetVideoInfo(deviceID,vsi,PREVIEW); 
	DSStream_ConnectPin(deviceID,PREVIEW);
	hr = DSStream_GetVideoStream(deviceID, StreamNotify, NULL);

	DestorySdk2000DefaultWnd();//DSStream_ConnectDevice���㴰������ΪNULL�������ǻᵯ��һ��ActiveMovie Window���ڳ�����
	                           //��д�˺���������������ص������ܹرմ˴��ڣ�һ�ر���Ƶ�Ͳ��ص��ˣ���
	if(FAILED(hr))
	{
		//MessageBox("��ʼ�������ص�ʧ��!");
		return false;
	}
	return true;
}
IplImage* TMSdk2000::QueryFrame()//�õ�һ֡ͼ��
{
	return NULL;
}
void TMSdk2000::Close()//�ر���Ƶ�豸
{
	for (int dwCard=0; dwCard<m_VideoCardDevNum; ++dwCard)
	{
		HRESULT hr = DSStream_DisconnectDevice(dwCard);
    }	
	VideoDevice::ClearImgQueue();
}
vector<Videostring> TMSdk2000::GetAllChannelName()//�õ������豸�����У�ͨ��/�豸������
{
	channelName.clear();
	Videostring strName;
	char szDeviceName[MAX_DEVICE_NUM][MAX_DEVICE_NAME_LEN];
	const char sz10Moons[] = {"10Moons"};
	int nCardNum=0;
	//Ѱ�ҿ��õĿ����޳���10Moons��
	HRESULT hr = DSStream_EnumVideoCaptureDev(szDeviceName, &nCardNum);
	m_VideoCardDevNum=nCardNum;
	if(FAILED(hr))
	{
		//AfxMessageBox("ö��10Moons��ʧ��", MB_OK, 0);
		return channelName;
	}	
	for (int dwCard=0; dwCard<m_VideoCardDevNum; ++dwCard)
	{
		string tmpstr;
		tmpstr.assign(szDeviceName[dwCard]);
		szDeviceName[dwCard][7] = '\0';
		if(_stricmp(szDeviceName[dwCard], sz10Moons) != 0) //����sdk��
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
//��ǰ�Ƿ�����·ͬ��ģʽ
bool TMSdk2000::Is4WayCard(int iCardID)
{
	long source = 0;
	DSStream_WhatInPinRouteToOutPin(iCardID, 0, &source);
	return (source==4);
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) //����ָ�������������ش���
{

	TCHAR wtitle[512];
	GetWindowText(hwnd, wtitle, sizeof(wtitle));
	if (_tcsstr(wtitle , _T("ActiveMovie Window"))) //���ڱ��⺬��ActiveMovie Window
	{
		//::SendMessage(hwnd, WM_CLOSE, 0, 0);
		::ShowWindow(hwnd,SW_HIDE);
	}
	return (true); //��Ҫ����
}
void TMSdk2000::DestorySdk2000DefaultWnd()
{
	EnumWindows(EnumWindowsProc, 0); //ö�����д���
}