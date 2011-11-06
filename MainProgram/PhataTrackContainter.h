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
	//���ҵ�һ��track��m_trackid����trackID��track,�Ҳ����򷵻�NULL
	CTrack *FindTrackByID(int trackID);
	int GetContainterSize()const{ return m_Tracks.size();};
	Track_vector GetTrackVector(TRACKSTATE state);//����״̬Ϊstate��CTrcak
	Track_vector m_Tracks;
};

