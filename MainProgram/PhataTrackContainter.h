#pragma once
#include "cv.h"
#include "Track.h"
#include <vector>
#include "PhataBlobContainter.h"
using std::vector;
typedef vector<CTrack*> Track_vector;
typedef vector<CTrack*>::iterator Track_vector_it;
typedef vector<CTrack*>::const_iterator Track_vector_constit;
class PhataTrackContainter
{
public:
	PhataTrackContainter(void);
	virtual ~PhataTrackContainter(void);
public:
	bool AddTrack(CTrack *track);
	void ClearTracks();
	CTrack GetTrack(int indexTrack)const;
	CTrack*GetTrack(int indexTrack);
	//查找第一个track的m_trackid等于trackID的track,找不到则返回NULL
	CTrack *FindTrackByID(int trackID);
	int GetContainterSize()const{ return m_Tracks.size();};
	Track_vector GetTrackVector(TRACKSTATE state);//返回状态为state的CTrcak
	Track_vector m_Tracks;
};

