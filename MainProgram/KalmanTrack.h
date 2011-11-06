#pragma once
#include "PhataTrackBlobs.h"
#include "KalmanBlob.h"
#include <vector>
using std::vector;
typedef struct _tagKalmanTrack{
	int  trackid; 
	CBlob blob;
	unsigned int lifetime; ///< Indicates how much frames the object has been in scene.
	unsigned int active; ///< Indicates number of frames that has been active from last inactive period.
	unsigned int inactive; ///< Indicates number of frames that has been missing.
	KalmanBlob *kalmanblob;
}KalmanTrackInfo;//目标信息
typedef std::vector<KalmanTrackInfo*> KalmanTrackInfo_Vector;//目标链

class KalmanTrack: public PhataTrackBlobs
{
public:
	KalmanTrack(void);
	~KalmanTrack(void);

public:
	virtual bool DoInit(IplImage *ProtoFrame);
	virtual bool GetBlobsImg(findbolbs_func func,IplImage *srcImg,IplImage *blobImg,void* userdata);
	virtual bool UpdateTrack(const double thDistance, const unsigned int thInactive, 
		const unsigned int thActive=0);
	virtual bool DrawTrack(IplImage *DrawImg,unsigned short mode=0x000f,CvFont *font=0);
private:
	KalmanTrackInfo_Vector m_kalmans;
};
