#pragma once
#include "phatatrackblobs.h"
class MBBTrack :
	public PhataTrackBlobs
{
public:
	MBBTrack(void);
	virtual ~MBBTrack(void);
	CvFont *m_pdefaultFont;
public:
	virtual bool DoInit(IplImage *ProtoFrame);
	virtual bool GetBlobsImg(findbolbs_func func,IplImage *srcImg,IplImage *blobImg,void* userdata);
	virtual bool UpdateTrack(const double thDistance, const unsigned int thInactive, 
		const unsigned int thActive=0);
	virtual bool DrawTrack(IplImage *DrawImg,unsigned short mode=0x000f,CvFont *font=0);
private:
	void updateTrackState(int indexTrack,TRACKSTATE State);
	bool processNVSN(int *blob_track,int nblob,int ntrack);
	long m_maxID;//下一个添加的目标的ID值
};

