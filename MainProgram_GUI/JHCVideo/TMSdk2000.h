#pragma once
#include "videodevice.h"
//����SDK2000 Dvr card,��2·����4·������
class TMSdk2000 :
	public VideoDevice
{
public:
	TMSdk2000(void);
	virtual ~TMSdk2000(void);
	size_t m_VideoCardDevNum;
	void DestorySdk2000DefaultWnd();
public:
	bool Init(HWND hWnd=NULL);//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
	bool Open(size_t deviceID,Videostring filepath=_T(""));//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
	IplImage* QueryFrame();//�õ�һ֡ͼ��
	void Close();//�ر���Ƶ�豸
	vector<Videostring> GetAllChannelName();//�õ������豸�����У�ͨ��/�豸������
public:
	//��д
	size_t GetChannelNum()const { return m_VideoCardDevNum;};//�õ������豸���ܣ�ͨ��/�豸����
private:
	//��ǰ�Ƿ�����·ͬ��ģʽ
	bool Is4WayCard(int iCardID);
};
