/*************************************************
**
** �ļ�����DSStream.h
**
** �����ߣ�10Moons xiyi
**
** ����ʱ�䣺2001.02.25
**
** �����£�2001.07.31
**
** ˵����DSStream.dll API �����������ļ�, ver 5.2
**
**************************************************/

#if !defined(DSSTREAM_H_2000_10_18)
#define DSSTREAM_H_2000_10_18

//����ö���豸
//�漰������
//	DSStream_EnumVideoCaptureDev()
//	DSStream_EnumAudioCaptureDev()
#define MAX_DEVICE_NAME_LEN	80
#define MAX_DEVICE_NUM		32

//���ڿ�����Ƶ����״̬
//�漰������
//	DSStream_GetStreamStatus()
//	DSStream_SetStreamStatus()
#define RUN		0
#define PAUSE	1
#define STOP	2

//�ֱ���� Capture Pin �� Preview Pin
//ǰ��������Ⱦ¼����������������Ⱦ��ʾ��
//�漰������
//	DSStream_GetVideoInfo()
//	DSStream_SetVideoInfo()
//	DSStream_ConnectPin()
//	DSStream_DeconnectPin()
#define CAPTURE	1
#define PREVIEW	2

//�ֱ��ʾ����ʹ����Ƶ�豸����Ƶ�豸������
//�漰������
//	DSStream_WhoIsCapturingAudio()
#define AUDIO_DEVICE_NO_USED	-1
#define AUDIO_DEVICE_NO_EXIST	-2

//��Ƶ�������ͣ�RGB555��RGB24��YV12
//�漰��
//	�ṹ VIDEOSTREAMINFO
typedef enum
{
	VideoSubType_None = -1,
	VideoSubType_RGB555 = 0,
	VideoSubType_RGB24,
	VideoSubType_YUY2,
	VideoSubType_YVU9,
	VideoSubType_YV12,
} VideoSubType;

//��Ƶ���ĸ�����Ϣ
//�漰������
//	DSStream_GetVideoInfo()
//	DSStream_SetVideoInfo()
typedef struct
{
	VideoSubType		subtype;
	RECT				rcSource;
	RECT				rcTarget;
	DWORD				dwBitRate;
	DWORD				dwBitErrorRate;
	LONGLONG			AvgTimePerFrame;//֡��
	BITMAPINFOHEADER	bmiHeader;		//���ߡ���ɫλ�ʵ�
} VIDEOSTREAMINFO;

//����ϵͳ�Ի������ø���������Ƶ����
//�漰������
//	DSStream_IsPropertyExist()
//	DSStream_DisplayPropertyDialogs()
typedef enum
{
	PropertyDlg_VideoCaptureFilter = 0,
	PropertyDlg_VideoCapturePin,	// *
	PropertyDlg_VideoPreviewPin,	// *
	PropertyDlg_VideoCrossbar,
	PropertyDlg_AudioCaptureFilter,
	PropertyDlg_AudioCapturePin,
	PropertyDlg_AudioCrossbar,
	PropertyDlg_TVAudioFilter,
	PropertyDlg_TVTuner,
	PropertyDlg_VfwCaptureFormat,	// *
	PropertyDlg_VfwCaptureSource,
	PropertyDlg_VfwCaptureDisplay,
} PropertyDialog;

//����DirectShow COM �ӿں������ø�����Ƶͼ������
//�漰������
//	DSStream_GetVideoPropertyValue()
//	DSStream_SetVideoPropertyValue()
typedef enum
{
	VideoProperty_Brightness = 0,	//����
	VideoProperty_Contrast,			//�Աȶ�
	VideoProperty_Hue,				//ɫ��
	VideoProperty_Saturation,		//���Ͷ�
	VideoProperty_Gamma,			//GammaУ��
	VideoProperty_ColorEnable,		
	VideoProperty_WhiteBalance,		//��ƽ��
	VideoProperty_BacklightCompensation,
} VideoProperty;

//��Ƶͼ�����Եĸ���ֵ
//�漰������
//	DSStream_GetVideoPropertyValue()
typedef struct 
{
	long lValue;		//��ǰֵ
	long lMin;			//��Сֵ
	long lMax;			//���ֵ
	long lStepDelta;	//��С����ֵ
	long lDefault;		//Ĭ��ֵ
	long lCapsFlags;	//���
} VIDEOPROPERTYRANGE;

//������Ƶ�˿�
//�漰������
//	DSStream_GetVideoStandard()
//	DSStream_SetVideoStandard()
typedef enum
{
	VideoStandard_None		= 0x00000000,
	VideoStandard_NTSC_M	= 0x00000001, 
	VideoStandard_NTSC_M_J	= 0x00000002,  
	VideoStandard_NTSC_433	= 0x00000004,
	VideoStandard_PAL_B		= 0x00000010,
	VideoStandard_PAL_D		= 0x00000020,
	VideoStandard_PAL_H		= 0x00000080,
	VideoStandard_PAL_I		= 0x00000100,
	VideoStandard_PAL_M		= 0x00000200,
	VideoStandard_PAL_N		= 0x00000400,
	VideoStandard_PAL_60	= 0x00000800,
	VideoStandard_SECAM_B	= 0x00001000,
	VideoStandard_SECAM_D	= 0x00002000,
	VideoStandard_SECAM_G	= 0x00004000,
	VideoStandard_SECAM_H	= 0x00008000,
	VideoStandard_SECAM_K	= 0x00010000,
	VideoStandard_SECAM_K1	= 0x00020000,
	VideoStandard_SECAM_L	= 0x00040000,
	VideoStandard_SECAM_L1	= 0x00080000,
} VideoStandard;

//��Ƶ״̬�ı��¼�����Ĵ���:
//����ΪLPARAM�ĵ������ֽڴ������ڣ�LPARAM�ĸ������ֽڱ�ʾ���ţ���
//����Card ID = (LPARAM>>16) & 0x0000FFFFL��Errod code = (LPARAM & 0x0000FFFFL)
//�漰������
//	DSStream_SetNotifyWindow()
//	DSStream_ManageNotifyMessage()
typedef enum
{
	Error_FirstNotifyMsg = 0x1234,	//�յ�������󣬱������ DSStream_ManageNotifyMessage()
	Error_DiskFull,				//����������wparam=0
	Error_VideoSizeChange,		//��Ƶ���ߴ�ı䣺height=HIWORD(wparam), width=LOWORD(wparam)
	Error_StreamStatusChange,	//��Ƶ��״̬�ı䣺wparam=0
	Error_CaptureError,			//¼���쳣��wparam=HRESULT: error value
} DSStreamError;

//��Ƶѹ��Filter����Ϣ
//�漰������
//	DSStream_EnumVideoCompressor()
typedef struct VIDEOCOMPRESSORINFO
{
	char szName[80];
} VideoCompressorInfo;

//��ʼ���ͷ���ʼ��
HRESULT DSStream_Initialize();
void	DSStream_Uninitialize();

//ö��ϵͳ�д��ڵ���Ƶ����Ƶ��׽�豸
HRESULT	DSStream_EnumVideoCaptureDev(
		char szDevName[MAX_DEVICE_NUM][MAX_DEVICE_NAME_LEN],
		int *pDevNum);
HRESULT	DSStream_EnumAudioCaptureDev(
		char szDevName[MAX_DEVICE_NUM][MAX_DEVICE_NAME_LEN],
		int *pDevNum);

//���ӺͶϿ���Ƶ��׽�豸
//	iVideoDevID����Ƶ��׽�豸����ţ���0Ϊ������
//	bOverlay��TURE-Overlay, FALSE-Preview
HRESULT	DSStream_ConnectDevice(int iCardID, BOOL bOverlay, HWND hParentWnd = NULL);
HRESULT DSStream_DisconnectDevice(int iCardID);

//��ù��ڶ࿨������
HRESULT DSStream_GetCardNumber(int * pCardNum);
HRESULT DSStream_IsConnected(int iCardID, BOOL * bConnected);

//˭���������ʹ��Ȩ
void DSStream_WhoIsCapturingAudio(int * pCardID);

//��Ƶ��ʾ���ڵĸ�����
HRESULT DSStream_SetOwnerWnd(int iCardID, HWND hParentWnd);
HRESULT DSStream_GetOwnerWnd(int iCardID, HWND* phParentWnd);

//��Ƶ��ʾ�����ڸ������е�λ��
HRESULT DSStream_SetWindowPos(int iCardID, RECT rc);
HRESULT DSStream_GetWindowPos(int iCardID, RECT* prc);

//¼��ʱ��.avi�ļ���
void DSStream_SetCaptureFile(int iCardID, LPCTSTR szFileName);
//��ʼ¼��
void DSStream_StartCapture(int iCardID);
//ֹͣ¼��
void DSStream_StopCapture(int iCardID);
//�Ƿ�����¼��
void DSStream_IsCapturing(int iCardID, BOOL* bIsCapturing);

//���ú���Ƶ��ʾ���ڵļ��̺������Ϣ���������� hWnd
HRESULT DSStream_SetMessageDrain(int iCardID, HWND hWnd);

//���ú�ÿ��DSStreamError����ʱ������ hWnd �����յ���Ϣ lMsg��LPARAM = DSStreamError��
HRESULT DSStream_SetNotifyWindow(int iCardID, HWND hWnd, long lMsg);
//������ǰ�������¼���������Ϣ lMsg ������ hWnd
void DSStream_ManageNotifyMessage(int iCardID);

//ȫ��
HRESULT DSStream_FullScreen(int iCardID, BOOL bFullSrc);
HRESULT DSStream_IsFullSrcMode(int iCardID, BOOL* bIsFull);

//��Ƶ������Ϣ
//	pin��CAPTURE Pin �� PREVIEW Pin
HRESULT DSStream_GetVideoInfo(int iCardID, VIDEOSTREAMINFO * pVSI, int pin);
HRESULT DSStream_SetVideoInfo(int iCardID, VIDEOSTREAMINFO vsi, int pin);

//����ϵͳ�Ի������ø���������Ƶ����
//	id�����Դ���
//	hParentWnd��ϵͳ�Ի���ĸ�����
//	szCaption��ϵͳ�Ի���ı���
HRESULT DSStream_DisplayPropertyDialogs(int iCardID, PropertyDialog id, HWND hParentWnd, LPCTSTR szCaption=NULL);
//����������Ƶ�����Ƿ�ɱ�����
HRESULT DSStream_IsPropertyExist(int iCardID, PropertyDialog id, BOOL* bReturn);

//������Ƶͼ������
HRESULT DSStream_GetVideoPropertyValue(int iCardID, VideoProperty id, VIDEOPROPERTYRANGE* pVPR);
HRESULT DSStream_SetVideoPropertyValue(int iCardID, VideoProperty id, long value);
//ר������·��������·ͬ��ʱ���ڵ�·ͼ�����Ƶ����
HRESULT DSStream_GetVideoPropertyValue_Ex(int iCardID, long lWay, VideoProperty id, long * pValue);
HRESULT DSStream_SetVideoPropertyValue_Ex(int iCardID, long lWay, VideoProperty id, long lValue);

//��Ƶ�˿�
HRESULT DSStream_RouteInPinToOutPin(int iCardID, int idInPin, int idOutPin);
HRESULT DSStream_WhatInPinRouteToOutPin(int iCardID, int idOutPin, long* pInPin);

//��Ƶ��ʽ
//	VideoStandard����ǰ��ʽ
//	pAvailable�������豸֧�ֵ�������ʽ
HRESULT DSStream_GetVideoStandard(int iCardID, VideoStandard* pVideoStandard, long* pAvailable);
HRESULT DSStream_SetVideoStandard(int iCardID, VideoStandard vs);

//���գ�JPG
//	szFileName��.jpg �ļ���ȫ·����
//	iQuality��ѹ��������10-100��
HRESULT DSStream_SaveToJpgFile(int iCardID, LPCTSTR szFileName, int iQuality);
//���գ�BMP
HRESULT DSStream_SaveToBmpFile(int iCardID, LPCTSTR szFileName);
//�õ���ǰͼ��� DIB ���ݡ��� pBuffer ���� NULL��pSize �õ�����ͼ������Ҫ���ڴ��С
HRESULT DSStream_GetCurrentDib(int iCardID, BYTE* pBuffer, long* pSize);

//��ǰ����ʾ״̬
HRESULT DSStream_GetStreamStatus(int iCardID, int* pStatus);
HRESULT DSStream_SetStreamStatus(int iCardID, int status);

//¼��ʱ�Ƿ�ʹ���Զ����֡�ʣ�duFrameRate = 0-30��
void DSStream_SetFrameRateOnCapture(int iCardID, BOOL bUseFrameRate, double duFrameRate);
void DSStream_IsUseFrameRate(int iCardID, BOOL* pYesOrNo);

//¼��ʱ�Ƿ�׽��Ƶ
void DSStream_IsCaptureAudio(int iCardID, BOOL* bCaptureAudio);
void DSStream_SetCaptureAudio(int iCardID, BOOL* bCaptureAudio);

//�Ͽ�������ĳ��Pin�����ӵ�����Filter
void DSStream_DisconnectPin(int iCardID, int pin);
void DSStream_ConnectPin(int iCardID, int pin);

//�����ã���ӡ����Filter������״�����ļ� szFile ��
void DSStream_ListAllFilter(int iCardID, LPCTSTR szFile);

//�쿴 PREVIEW Pin �Ƿ����
HRESULT DSStream_IsPreviewPinExisted(int iCardID, BOOL* pbExisted);

//��ȡDIB������
extern "C"
{
typedef void (__stdcall *VideoStreamProc) (const BYTE* pDIBHead, const BYTE* pDIBits, LPVOID pParam);
}
HRESULT DSStream_GetVideoStream(int iCardID, VideoStreamProc proc, LPVOID pParam);

//����Logo��Time��Date����ʾ
//	���ñ�ʾLogo��BMP�ļ�������Ϊ24λ�������½ǵ�һ��Ϊ͸��ɫ
HRESULT DSStream_SetLogoFile(int iCardID, LPCTSTR szFilename);
//	��ʾ������Logo
HRESULT DSStream_ShowLogo(int iCardID, BOOL bShow, int x, int y);
//	��ʾ������Time
HRESULT DSStream_ShowTime(int iCardID, BOOL bShow, int x, int y);
//	��ʾ������Date
HRESULT DSStream_ShowDate(int iCardID, BOOL bShow, int x, int y);
//	�Ƿ�����DSStream_GetVideoStream()�õ�������������ʾLogo��Time��Date
//	Ӱ��ĺ�����
//		DSStream_GetVideoStream()
HRESULT DSStream_ShowLogoTimeOnUserStream(int iCardID, BOOL bShow);

//ר���ڴ��������·����������ڣ��� 0 Ϊ����
//idCamera >= 32 ʱ���� idWay ·��ͼ��
HRESULT DSStream_ChooseCamera_Ex(int iCardID, BYTE idWay, BYTE idCamera);

//�õ���Ƶѹ��Filter����Ϣ
HRESULT DSStream_EnumVideoCompressor(VideoCompressorInfo * pInfo, int * piVidCompNum);
//ѡ����Ƶѹ���㷨
HRESULT DSStream_ChooseVideoCompressor(LPCTSTR szCompName);
//������ǰʹ�õ���ʲôѹ���㷨
HRESULT DSStream_GetCurrentVideoCompressor(LPSTR pszCompName);
//��Ƶѹ��������(0--100)
HRESULT DSStream_GetVideoCompressorQuality(int * piQuality);
HRESULT DSStream_SetVideoCompressorQuality(int iQuality);

//��⵱ǰ����Ƶ����˿����Ƿ����ź�����
HRESULT DSStream_IsVideoSignalLocked(int iCardID, BOOL * bIsSignalLocked);

#endif //!defined(DSSTREAM_H_2000_10_18)