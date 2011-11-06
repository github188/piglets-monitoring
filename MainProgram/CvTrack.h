#pragma once
#include "PhataTrackBlobs.h"
#include <list>
using std::list;

class CvTrack :	public PhataTrackBlobs
{
public:
	CvTrack(void);
	virtual ~CvTrack(void);
	CvFont *m_pdefaultFont;
public:
	virtual bool DoInit(IplImage *ProtoFrame);
	virtual bool GetBlobsImg(findbolbs_func func,IplImage *srcImg,IplImage *blobImg,void* userdata);
	virtual bool UpdateTrack(const double thDistance, const unsigned int thInactive, 
		const unsigned int thActive=0);
	virtual bool DrawTrack(IplImage *DrawImg,unsigned short mode=0x000f,CvFont *font=0);

private:
	void getClusterForBlob(unsigned int blobPos, unsigned int *close, unsigned int nBlobs, unsigned int nTracks, 
		Blob_vector const &blobs, list<CBlob*> &bb, list<CTrack*> &tt);
	void getClusterForTrack(unsigned int trackPos, unsigned int *close, unsigned int nBlobs, unsigned int nTracks, 
		Blob_vector const &blobs, list<CBlob*> &bb, list<CTrack*> &tt);
};

