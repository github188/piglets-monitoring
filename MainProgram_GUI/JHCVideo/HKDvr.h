#pragma once
#include "videodevice.h"
#include "hikang/HCNetSDK.h"
//�豸��Ϣ�б�ṹ
typedef struct _HKDeviceInfo {
	long   lID;         //�豸iD,Ϊ������ʾ������
	string DeviceName;//�豸����,Ϊ������ʾ������
	string IPAdd;     //�豸IP��ַ�ַ�
	long   lPort;       //��¼�˿ں�
	string UserNanme; //��¼�û���
	string Pwd;       //��¼����
	NET_DVR_DEVICEINFO_V30 DeviceInfo;//�豸����
}HKDeviceInfo,*PHKDeviceInfo;
class HKDvr :
	public VideoDevice
{
public:
	HKDvr(void);
	virtual ~HKDvr(void);
	HKDeviceInfo devInfo;
	long m_lUserID;//��¼�豸�󷵻ص��û�ID
	long m_lRealPlay;//Ԥ������ֵ
	long m_lPort;////���ڱ��������Ĳ��ſ�port��(����������ͨ����Ҳ�豸��ͨ�������)
	size_t m_VideoDeviceChannelNum;
public:
	bool Init(HWND hWnd=NULL);//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
	bool Open(size_t nChannel,Videostring filepath=_T(""));//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
	IplImage* QueryFrame();//�õ�һ֡ͼ��
	void Close();//�ر���Ƶ�豸
	vector<Videostring> GetAllChannelName();//�õ������豸�����У�ͨ��/�豸������
public:
	//��д
	void SetDeviceInfo(string ipAdd, long port, string userName, string password);
	size_t GetChannelNum()const { return m_VideoDeviceChannelNum;};//�õ������豸���ܣ�ͨ��/�豸����
};
