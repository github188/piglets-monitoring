#pragma once
#include "videodevice.h"
//����Dvr card,оƬSa7130,֧��10MOONS SDK3000/2500ϵͳ��Ʒ
class TMSdk2500And3000 :
	public VideoDevice
{
public:
	TMSdk2500And3000(void);
	virtual ~TMSdk2500And3000(void);
	size_t m_VideoCardChannelNum;
	size_t OpenChannelID;
public:
	bool Init(HWND hWnd=NULL);//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
	bool Open(size_t deviceID,Videostring filepath=_T(""));//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
	IplImage* QueryFrame();//�õ�һ֡ͼ��
	void Close();//�ر���Ƶ�豸
	vector<Videostring> GetAllChannelName();//�õ������豸�����У�ͨ��/�豸������
public:
	//��д
	size_t GetChannelNum()const { return m_VideoCardChannelNum;};//�õ������豸���ܣ�ͨ��/�豸����
};
