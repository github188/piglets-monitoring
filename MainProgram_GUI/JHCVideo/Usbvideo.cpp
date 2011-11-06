#include "Usbvideo.h"
#include "CameraDS.h"

Usbvideo::Usbvideo(void)
:VideoDevice(false)
,m_img(NULL)
{

}

Usbvideo::~Usbvideo(void)
{
	Close();
}
bool Usbvideo::Init(HWND hWnd/*=NULL*/)//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
{
	return true;
}
bool Usbvideo::Open(size_t deviceID,Videostring filepath/*=_T("")*/)//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
{
	assert(deviceID>=0 && deviceID<=GetChannelNum());//(�豸�Ŵ�0��ʼ) 
	Close();
	assert(deviceID<=GetChannelNum());
	isCapture=m_pCap.OpenCamera(deviceID,false);
	return isCapture;
}
IplImage* Usbvideo::QueryFrame()//�õ�һ֡ͼ��
{
	//m_img=cvQueryFrame(m_pCap);
	//VideoDevice::PushIntoImgQueue(m_img);
	//return m_img;
	m_img=m_pCap.QueryFrame();
	VideoDevice::PushIntoImgQueue(m_img);
	return m_img;
}
void Usbvideo::Close()//�ر���Ƶ�豸
{
	m_pCap.CloseCamera();
	isCapture=false;
	VideoDevice::ClearImgQueue();
}
vector<Videostring> Usbvideo::GetAllChannelName()//�õ������豸�����У�ͨ��/�豸������
{
	channelName.clear();
	//������ȡ����ͷ��Ŀ
	int cam_count = CCameraDS::CameraCount();
	//��ȡ��������ͷ������
	if(cam_count==0)
		return channelName;
	for(int i=0; i < cam_count; i++)
	{
		char camera_name[1024];  
		int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name) );
		if(retval >0)
		{
			string strName;
			strName.assign(camera_name);
#ifdef _UNICODE
			wstring Ansifilepath=s2ws(strName);
			channelName.push_back(Ansifilepath);
#else
			channelName.push_back(strName);
#endif
		}
	}
	return channelName;
}
