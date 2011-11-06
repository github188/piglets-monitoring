/*************************************************
**
** 文件名：DSStream.h
**
** 创建者：10Moons xiyi
**
** 创建时间：2001.02.25
**
** 最后更新：2001.07.31
**
** 说明：DSStream.dll API 函数的声明文件, ver 5.2
**
**************************************************/

#if !defined(DSSTREAM_H_2000_10_18)
#define DSSTREAM_H_2000_10_18

//用于枚举设备
//涉及函数：
//	DSStream_EnumVideoCaptureDev()
//	DSStream_EnumAudioCaptureDev()
#define MAX_DEVICE_NAME_LEN	80
#define MAX_DEVICE_NUM		32

//用于控制视频流的状态
//涉及函数：
//	DSStream_GetStreamStatus()
//	DSStream_SetStreamStatus()
#define RUN		0
#define PAUSE	1
#define STOP	2

//分别代表 Capture Pin 和 Preview Pin
//前者用于渲染录像流，后者用于渲染显示流
//涉及函数：
//	DSStream_GetVideoInfo()
//	DSStream_SetVideoInfo()
//	DSStream_ConnectPin()
//	DSStream_DeconnectPin()
#define CAPTURE	1
#define PREVIEW	2

//分别表示无人使用音频设备和音频设备不存在
//涉及函数：
//	DSStream_WhoIsCapturingAudio()
#define AUDIO_DEVICE_NO_USED	-1
#define AUDIO_DEVICE_NO_EXIST	-2

//视频的子类型：RGB555、RGB24、YV12
//涉及：
//	结构 VIDEOSTREAMINFO
typedef enum
{
	VideoSubType_None = -1,
	VideoSubType_RGB555 = 0,
	VideoSubType_RGB24,
	VideoSubType_YUY2,
	VideoSubType_YVU9,
	VideoSubType_YV12,
} VideoSubType;

//视频流的各种信息
//涉及函数：
//	DSStream_GetVideoInfo()
//	DSStream_SetVideoInfo()
typedef struct
{
	VideoSubType		subtype;
	RECT				rcSource;
	RECT				rcTarget;
	DWORD				dwBitRate;
	DWORD				dwBitErrorRate;
	LONGLONG			AvgTimePerFrame;//帧率
	BITMAPINFOHEADER	bmiHeader;		//宽、高、颜色位率等
} VIDEOSTREAMINFO;

//调用系统对话框设置各种音、视频属性
//涉及函数：
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

//调用DirectShow COM 接口函数设置各种视频图像属性
//涉及函数：
//	DSStream_GetVideoPropertyValue()
//	DSStream_SetVideoPropertyValue()
typedef enum
{
	VideoProperty_Brightness = 0,	//亮度
	VideoProperty_Contrast,			//对比度
	VideoProperty_Hue,				//色度
	VideoProperty_Saturation,		//饱和度
	VideoProperty_Gamma,			//Gamma校验
	VideoProperty_ColorEnable,		
	VideoProperty_WhiteBalance,		//白平衡
	VideoProperty_BacklightCompensation,
} VideoProperty;

//视频图像属性的各个值
//涉及函数：
//	DSStream_GetVideoPropertyValue()
typedef struct 
{
	long lValue;		//当前值
	long lMin;			//最小值
	long lMax;			//最大值
	long lStepDelta;	//最小步进值
	long lDefault;		//默认值
	long lCapsFlags;	//风格
} VIDEOPROPERTYRANGE;

//各种视频端口
//涉及函数：
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

//视频状态改变或录像错误的代号:
//它作为LPARAM的低两个字节传给窗口（LPARAM的高两个字节表示卡号），
//即：Card ID = (LPARAM>>16) & 0x0000FFFFL，Errod code = (LPARAM & 0x0000FFFFL)
//涉及函数：
//	DSStream_SetNotifyWindow()
//	DSStream_ManageNotifyMessage()
typedef enum
{
	Error_FirstNotifyMsg = 0x1234,	//收到这个错误，必须调用 DSStream_ManageNotifyMessage()
	Error_DiskFull,				//磁盘已满：wparam=0
	Error_VideoSizeChange,		//视频流尺寸改变：height=HIWORD(wparam), width=LOWORD(wparam)
	Error_StreamStatusChange,	//视频流状态改变：wparam=0
	Error_CaptureError,			//录像异常：wparam=HRESULT: error value
} DSStreamError;

//视频压缩Filter的信息
//涉及函数：
//	DSStream_EnumVideoCompressor()
typedef struct VIDEOCOMPRESSORINFO
{
	char szName[80];
} VideoCompressorInfo;

//初始化和反初始化
HRESULT DSStream_Initialize();
void	DSStream_Uninitialize();

//枚举系统中存在的视频、音频捕捉设备
HRESULT	DSStream_EnumVideoCaptureDev(
		char szDevName[MAX_DEVICE_NUM][MAX_DEVICE_NAME_LEN],
		int *pDevNum);
HRESULT	DSStream_EnumAudioCaptureDev(
		char szDevName[MAX_DEVICE_NUM][MAX_DEVICE_NAME_LEN],
		int *pDevNum);

//连接和断开视频捕捉设备
//	iVideoDevID：视频捕捉设备的序号（以0为基数）
//	bOverlay：TURE-Overlay, FALSE-Preview
HRESULT	DSStream_ConnectDevice(int iCardID, BOOL bOverlay, HWND hParentWnd = NULL);
HRESULT DSStream_DisconnectDevice(int iCardID);

//获得关于多卡的数据
HRESULT DSStream_GetCardNumber(int * pCardNum);
HRESULT DSStream_IsConnected(int iCardID, BOOL * bConnected);

//谁获得声卡的使用权
void DSStream_WhoIsCapturingAudio(int * pCardID);

//视频显示窗口的父窗口
HRESULT DSStream_SetOwnerWnd(int iCardID, HWND hParentWnd);
HRESULT DSStream_GetOwnerWnd(int iCardID, HWND* phParentWnd);

//视频显示窗口在父窗口中的位置
HRESULT DSStream_SetWindowPos(int iCardID, RECT rc);
HRESULT DSStream_GetWindowPos(int iCardID, RECT* prc);

//录像时的.avi文件名
void DSStream_SetCaptureFile(int iCardID, LPCTSTR szFileName);
//开始录像
void DSStream_StartCapture(int iCardID);
//停止录像
void DSStream_StopCapture(int iCardID);
//是否正在录像
void DSStream_IsCapturing(int iCardID, BOOL* bIsCapturing);

//设置后，视频显示窗口的键盘和鼠标消息将发往窗口 hWnd
HRESULT DSStream_SetMessageDrain(int iCardID, HWND hWnd);

//设置后，每当DSStreamError产生时，窗口 hWnd 都将收到消息 lMsg（LPARAM = DSStreamError）
HRESULT DSStream_SetNotifyWindow(int iCardID, HWND hWnd, long lMsg);
//分析当前产生的事件，发送消息 lMsg 到窗口 hWnd
void DSStream_ManageNotifyMessage(int iCardID);

//全屏
HRESULT DSStream_FullScreen(int iCardID, BOOL bFullSrc);
HRESULT DSStream_IsFullSrcMode(int iCardID, BOOL* bIsFull);

//视频流的信息
//	pin：CAPTURE Pin 或 PREVIEW Pin
HRESULT DSStream_GetVideoInfo(int iCardID, VIDEOSTREAMINFO * pVSI, int pin);
HRESULT DSStream_SetVideoInfo(int iCardID, VIDEOSTREAMINFO vsi, int pin);

//调用系统对话框设置各种音、视频属性
//	id：属性代号
//	hParentWnd：系统对话框的父窗口
//	szCaption：系统对话框的标题
HRESULT DSStream_DisplayPropertyDialogs(int iCardID, PropertyDialog id, HWND hParentWnd, LPCTSTR szCaption=NULL);
//各种音、视频属性是否可被调用
HRESULT DSStream_IsPropertyExist(int iCardID, PropertyDialog id, BOOL* bReturn);

//各种视频图像属性
HRESULT DSStream_GetVideoPropertyValue(int iCardID, VideoProperty id, VIDEOPROPERTYRANGE* pVPR);
HRESULT DSStream_SetVideoPropertyValue(int iCardID, VideoProperty id, long value);
//专用于四路卡：在四路同屏时调节单路图像的视频属性
HRESULT DSStream_GetVideoPropertyValue_Ex(int iCardID, long lWay, VideoProperty id, long * pValue);
HRESULT DSStream_SetVideoPropertyValue_Ex(int iCardID, long lWay, VideoProperty id, long lValue);

//视频端口
HRESULT DSStream_RouteInPinToOutPin(int iCardID, int idInPin, int idOutPin);
HRESULT DSStream_WhatInPinRouteToOutPin(int iCardID, int idOutPin, long* pInPin);

//视频制式
//	VideoStandard：当前制式
//	pAvailable：返回设备支持的所有制式
HRESULT DSStream_GetVideoStandard(int iCardID, VideoStandard* pVideoStandard, long* pAvailable);
HRESULT DSStream_SetVideoStandard(int iCardID, VideoStandard vs);

//快照：JPG
//	szFileName：.jpg 文件的全路径名
//	iQuality：压缩质量（10-100）
HRESULT DSStream_SaveToJpgFile(int iCardID, LPCTSTR szFileName, int iQuality);
//快照：BMP
HRESULT DSStream_SaveToBmpFile(int iCardID, LPCTSTR szFileName);
//得到当前图像的 DIB 数据。若 pBuffer 等于 NULL，pSize 得到保存图像所需要的内存大小
HRESULT DSStream_GetCurrentDib(int iCardID, BYTE* pBuffer, long* pSize);

//当前的显示状态
HRESULT DSStream_GetStreamStatus(int iCardID, int* pStatus);
HRESULT DSStream_SetStreamStatus(int iCardID, int status);

//录像时是否使用自定义的帧率（duFrameRate = 0-30）
void DSStream_SetFrameRateOnCapture(int iCardID, BOOL bUseFrameRate, double duFrameRate);
void DSStream_IsUseFrameRate(int iCardID, BOOL* pYesOrNo);

//录像时是否捕捉音频
void DSStream_IsCaptureAudio(int iCardID, BOOL* bCaptureAudio);
void DSStream_SetCaptureAudio(int iCardID, BOOL* bCaptureAudio);

//断开、连接某个Pin上连接的所有Filter
void DSStream_DisconnectPin(int iCardID, int pin);
void DSStream_ConnectPin(int iCardID, int pin);

//调试用：打印所有Filter的连接状况到文件 szFile 中
void DSStream_ListAllFilter(int iCardID, LPCTSTR szFile);

//察看 PREVIEW Pin 是否存在
HRESULT DSStream_IsPreviewPinExisted(int iCardID, BOOL* pbExisted);

//获取DIB数据流
extern "C"
{
typedef void (__stdcall *VideoStreamProc) (const BYTE* pDIBHead, const BYTE* pDIBits, LPVOID pParam);
}
HRESULT DSStream_GetVideoStream(int iCardID, VideoStreamProc proc, LPVOID pParam);

//设置Logo、Time、Date的显示
//	设置表示Logo的BMP文件，必须为24位，最左下角的一点为透明色
HRESULT DSStream_SetLogoFile(int iCardID, LPCTSTR szFilename);
//	显示或隐藏Logo
HRESULT DSStream_ShowLogo(int iCardID, BOOL bShow, int x, int y);
//	显示或隐藏Time
HRESULT DSStream_ShowTime(int iCardID, BOOL bShow, int x, int y);
//	显示或隐藏Date
HRESULT DSStream_ShowDate(int iCardID, BOOL bShow, int x, int y);
//	是否在由DSStream_GetVideoStream()得到的数据流中显示Logo、Time、Date
//	影响的函数：
//		DSStream_GetVideoStream()
HRESULT DSStream_ShowLogoTimeOnUserStream(int iCardID, BOOL bShow);

//专用于带矩阵的四路卡：矩阵调节，以 0 为基数
//idCamera >= 32 时，第 idWay 路无图像。
HRESULT DSStream_ChooseCamera_Ex(int iCardID, BYTE idWay, BYTE idCamera);

//得到视频压缩Filter的信息
HRESULT DSStream_EnumVideoCompressor(VideoCompressorInfo * pInfo, int * piVidCompNum);
//选择视频压缩算法
HRESULT DSStream_ChooseVideoCompressor(LPCTSTR szCompName);
//看看当前使用的是什么压缩算法
HRESULT DSStream_GetCurrentVideoCompressor(LPSTR pszCompName);
//视频压缩的质量(0--100)
HRESULT DSStream_GetVideoCompressorQuality(int * piQuality);
HRESULT DSStream_SetVideoCompressorQuality(int iQuality);

//检测当前的视频输入端口上是否有信号输入
HRESULT DSStream_IsVideoSignalLocked(int iCardID, BOOL * bIsSignalLocked);

#endif //!defined(DSSTREAM_H_2000_10_18)