#pragma once
#include "cv.h"
#include <vector>
#include "PhataBlobContainter.h"
#include "PhataTrackContainter.h"
#include "KalmanBlob.h"
//定义获取监控目标的函数
//srcImg --原图像
//bolbImg--得到的目标图像（二值化，黑白图像）
typedef bool (*findbolbs_func)(IplImage *srcImg,IplImage *blobImg, void* userdata);

#define CV_TRACK_RENDER_ID            0x0001 ///< Print the ID of each track in the image. \see DrawTrack
#define CV_TRACK_RENDER_BOUNDING_BOX  0x0002 ///< Draw bounding box of each track in the image. \see DrawTrack
#define CV_TRACK_RENDER_TO_LOG        0x0004 ///< Print track info to log out. \see DrawTrack
#define CV_TRACK_RENDER_TO_STD        0x0008 ///< Print track info to log out. \see DrawTrack
#define CV_TRACK_RENDER_TO_CXLOG      0x0010 ///< Print track info to log text. \see DrawTrack


//typedef struct _tagTrack{
//	int  trackid; ///< Track identification number.
//	int  label;///< Label assigned to the blob related to this track.
//	unsigned int minx; ///< X min.
//	unsigned int maxx; ///< X max.
//	unsigned int miny; ///< Y min.
//	unsigned int maxy; ///< y max.
//	CvPoint2D64f centroid; ///< Centroid.
//	unsigned int lifetime; ///< Indicates how much frames the object has been in scene.
//	unsigned int active; ///< Indicates number of frames that has been active from last inactive period.
//	unsigned int inactive; ///< Indicates number of frames that has been missing.
//	KalmanBlob kalmanblob;
//}TrackInfo;//目标信息
//typedef std::vector<TrackInfo*> TrackInfo_Vector;//目标链

class PhataTrackBlobs
{
public:
	PhataTrackBlobs(void);
	virtual ~PhataTrackBlobs(void);
public:
	virtual bool DoInit(IplImage *ProtoFrame);
	virtual bool GetBlobsImg(findbolbs_func func,IplImage *srcImg,IplImage *blobImg,void* userdata);
	virtual bool UpdateTrack(const double thDistance, const unsigned int thInactive,
		const unsigned int thActive=0)=0;
	virtual bool DrawTrack(IplImage *DrawImg,unsigned short mode=0x000f,CvFont *font=0)=0;
public:
//由于PhataTrackContainter无定义赋值运算符，故只能返回指针
	const PhataTrackContainter* GetCurTracks(){return &m_CurTracks;};
protected:
	PhataBlobContainter m_CurBlobs;
	PhataTrackContainter m_CurTracks;
};

