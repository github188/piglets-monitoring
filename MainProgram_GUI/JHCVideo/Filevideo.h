#pragma once
#include "videodevice.h"

class Filevideo :
	public VideoDevice//Ҫpublic�̳У���Ȼ�޷���ָ�븳ֵ������(VideoDevice��)ָ��
{
public:
	Filevideo(void);
	virtual ~Filevideo(void);
public://��д���麯��
	bool Init(HWND hWnd=NULL);//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
	bool Open(size_t deviceID,Videostring filepath=_T(""));//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
	IplImage* QueryFrame();//�õ�һ֡ͼ��
	void Close();//�ر���Ƶ�豸
	vector<Videostring> GetAllChannelName();//�õ������豸�����У�ͨ��/�豸������
private:
	CvCapture *m_pCap;
	IplImage *m_img;//��CvCapture���صĺ���������Ҫ�û��ͷ�
};
