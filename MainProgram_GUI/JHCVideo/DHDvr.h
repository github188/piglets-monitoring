#pragma once
#include "videodevice.h"
#include "DaHua/dhassistant.h"
#include "DaHua/dhnetsdk.h"
//��DVR�֧࣬�����д�����Ӳ��¼�����������Ƶ���������������������������������ز�Ʒ
//�豸��Ϣ�б�ṹ
typedef struct _DHDeviceInfo {
	long   lID;         //�豸iD,Ϊ������ʾ������
	string DeviceName;//�豸����,Ϊ������ʾ������
	string IPAdd;     //�豸IP��ַ�ַ�
	long   lPort;       //��¼�˿ں�
	string UserNanme; //��¼�û���
	string Pwd;       //��¼����
	NET_DEVICEINFO DeviceInfo;//�豸����
}DHDeviceInfo,*PDHDeviceInfo;
class DHDvr :
	public VideoDevice
{
public:
	DHDvr(void);
	virtual ~DHDvr(void);
	DHDeviceInfo devInfo;
	long m_lUserID;//��¼�豸�󷵻ص��û�ID
	long m_lRealPlay;//Ԥ������ֵ
	long m_lPort;////���ڱ��������Ĳ��ſ�port��(����������ͨ����Ҳ�豸��ͨ�������)
	size_t m_VideoDeviceChannelNum;
public:
	bool Init(HWND hWnd=NULL);//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
	bool Open(size_t deviceID,Videostring filepath=_T(""));//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
	IplImage* QueryFrame();//�õ�һ֡ͼ��
	void Close();//�ر���Ƶ�豸
	vector<Videostring> GetAllChannelName();//�õ������豸�����У�ͨ��/�豸������
	HWND m_hwnd;
public:
	//��д
	void SetDeviceInfo(string ipAdd, long port, string userName, string password);
	size_t GetChannelNum()const { return m_VideoDeviceChannelNum;};//�õ������豸���ܣ�ͨ��/�豸����
};
