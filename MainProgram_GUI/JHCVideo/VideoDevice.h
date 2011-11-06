#pragma once
#include <string>
#include <vector>
#include <queue>
#include <afxmt.h>
#include "cv.h"
#include "highgui.h"
using std::string;
using std::wstring;
using std::vector;
using std::queue;
#if CV_MAJOR_VERSION==2 && CV_MINOR_VERSION==1
	#ifdef _DEBUG
#pragma comment(lib,"ipcommd.lib")
#pragma comment(lib,"ipstddatad.lib")
#pragma comment(lib,"ipguid.lib")
#pragma comment(lib,"ipalgod.lib")
#pragma comment(lib,"iplearnd.lib")
#else
#pragma comment(lib,"ipcomm.lib")
#pragma comment(lib,"ipstddata.lib")
#pragma comment(lib,"ipgui.lib")
#pragma comment(lib,"ipalgo.lib")
#pragma comment(lib,"iplearn.lib")
	#endif
#elif  CV_MAJOR_VERSION==2 && CV_MINOR_VERSION==0
	#ifdef _DEBUG
	#pragma comment(lib,"cv200d.lib")
	#pragma comment(lib,"cxcore200d.lib")
	#pragma comment(lib,"highgui200d.lib")
	#pragma comment(lib,"cvaux200d.lib")
	#else
	#pragma comment(lib,"cv200.lib")
	#pragma comment(lib,"cxcore200.lib")
	#pragma comment(lib,"highgui200.lib")
	#pragma comment(lib,"cvaux200.lib")
	#endif
#endif

#ifdef _UNICODE
typedef wstring  Videostring; 
#else
typedef string   Videostring;
#endif
// �ٽ�������
extern CCriticalSection g_cs_imgqueue;
extern CCriticalSection g_cs_analysis;

class VideoDevice
{
public:
	VideoDevice(bool isUsingCallBack=true);
	virtual ~VideoDevice(void);
	bool IsCaptureing()const { return isCapture;};//�豸�Ƿ����ڲ�����Ƶ
	bool IsUsingCallBack()const { return usingCallback;};//�豸�Ƿ�ʹ�ûص���������ͼ������
public://��̬����
	static void PushIntoImgQueue(IplImage *m_pframe);
	static size_t GetQueueSize();
	static IplImage* GetFrontImgQueue();
	static void PopImgQueue();
	static void ClearImgQueue();
	static bool DrawImgToHDC( HDC hDCDst, CRect rect,IplImage *img );//��ͼ��img��ʾ�ڿؼ�hDCDst��
public://���麯��
	virtual bool Init(HWND hWnd=NULL)=0;//��ʼ����Ƶ�豸,hWnd(��ѡ)Ϊ�豸��overlay���ھ��
	virtual bool Open(size_t deviceID,Videostring filepath=_T(""))=0;//����Ƶ�豸,filepathΪ����Ƶ�ļ�ʱ����Ҫ
	virtual IplImage* QueryFrame()=0;//�õ�һ֡ͼ��,ʹ�ûص��������豸����Ҫ����Щ����
	virtual void Close()=0;//�ر���Ƶ�豸
	virtual vector<Videostring> GetAllChannelName()=0;//�õ������豸�����У�ͨ��/�豸������
public://�麯��������д
	virtual void SetDeviceInfo(string ipAdd, long port, string userName, string password){return;};
	virtual size_t GetChannelNum()const { return channelName.size();};//�õ������豸���ܣ�ͨ��/�豸����
protected:
	vector<Videostring> channelName;//Ĭ��Ϊ��
	bool isCapture;//=false;
	bool usingCallback;//=true;�Ƿ�ʹ�ûص������ɼ�ͼ�����У�Ĭ���ǣ�ֻ���ļ���USB�ӿ���Ϊ�ǣ�
protected://��̬����
	static CvvImage m_ShowImg;
	static queue<IplImage*>img_queue;//�洢ͼ��Ķ���
};
//ȫ�ֺ���
std::string ws2s(const std::wstring& ws);//��wstring�����ַ���ת��Ϊstring����
std::wstring s2ws(const std::string& s);//��string�����ַ���ת��Ϊwstring����