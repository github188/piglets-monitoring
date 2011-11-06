#pragma once
#include "cv.h"
#include "KalmanBlob.h"
#include "CountedPtr.h"
#include "../GoogleCode_video-tracker/opencv_cvBlobsLibV8.3/Blob.h"
typedef CountedPtr<KalmanBlob> KalmanBlobPtr;
typedef enum _trackstate{
	TRACKSTATE_ACTIVE=1,
	TRACKSTATE_MISS,
	TRACKSTATE_MERGE,
	TRACKSTATE_SEPARATE,
}TRACKSTATE;

class CTrack:public CBlob
{
public:
	CTrack(void);
	CTrack(CBlob &blob,int id,TRACKSTATE state);
	CTrack(CTrack &srcTrack);
	CTrack(CTrack *srcTrack);
	CTrack(CBlob &srcBlob);
	CTrack(CBlob *srcBlob);
	CTrack& operator=(const CTrack &srcTrack);
	CTrack& operator=(const CBlob &srcBlob);
	virtual ~CTrack(void);
private:
	TRACKSTATE m_trackState;
	int  m_trackid; ///< Track identification number.
public:
	unsigned int m_lifetime; ///< Indicates how much frames the object has been in scene.
	unsigned int m_active; ///< Indicates number of frames that has been active from last inactive period.
	unsigned int m_inactive; ///< Indicates number of frames that has been missing.
	CvRect m_rcBoundingBox;//最小包围矩形
	CvBox2D m_box;//最小外接矩形
	KalmanBlobPtr m_kalmanblob;
public:
	TRACKSTATE GetState() const { return m_trackState;};
	void SetState(TRACKSTATE trackState);
	int GetID() const { return m_trackid;};
	void SetID(int id) { m_trackid=id;};
	CvPoint GetCentorid()const { 
		return cvPoint(m_rcBoundingBox.x+m_rcBoundingBox.width/2,m_rcBoundingBox.y+m_rcBoundingBox.height/2);
	};
	void UpdateBlob(CBlob &blob,bool isUseKalman=false);
};
double DistanceBetweenTrackAndBlob( CBlob& blob, CTrack& track);
double CommonArea(CvRect rectT1,CvRect rectT2);
