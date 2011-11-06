#pragma once
#include "videodevice.h"
#include "CameraDS.h"
class Usbvideo :
	public VideoDevice
{
public:
	Usbvideo(void);
	virtual ~Usbvideo(void);
public:
	bool Init(HWND hWnd=NULL);//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
	bool Open(size_t deviceID,Videostring filepath=_T(""));//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
	IplImage* QueryFrame();//�õ�һ֡ͼ��
	void Close();//�ر���Ƶ�豸
	vector<Videostring> GetAllChannelName();//�õ������豸�����У�ͨ��/�豸������
private:
	CCameraDS m_pCap;
	IplImage *m_img;//��CvCapture���صĺ���������Ҫ�û��ͷ�
};
