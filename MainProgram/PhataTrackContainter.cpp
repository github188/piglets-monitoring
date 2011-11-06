#include "PhataTrackContainter.h"

PhataTrackContainter::PhataTrackContainter(void)
{
	m_Tracks=Track_vector();
}

PhataTrackContainter::~PhataTrackContainter(void)
{
	ClearTracks();
}

bool PhataTrackContainter::AddTrack( CTrack *track )
{
	if( track){
		m_Tracks.push_back(new CTrack(track));
		return true;
	}
	return false;
}

void PhataTrackContainter::ClearTracks()
{
	Track_vector_it ittracks = m_Tracks.begin();
	while( ittracks != m_Tracks.end() )
	{
		delete *ittracks;
		ittracks++;
	}
	m_Tracks.clear();
}

CTrack PhataTrackContainter::GetTrack( int indexTrack )const
{
	if( indexTrack < 0 || indexTrack >= GetContainterSize() )
	{
		return CTrack();
	}
	return *m_Tracks[indexTrack];
}

CTrack* PhataTrackContainter::GetTrack( int indexTrack ) 
{
	if( indexTrack < 0 || indexTrack >= GetContainterSize() )
	{
		return NULL;		
	}
	return m_Tracks[indexTrack];
}

CTrack * PhataTrackContainter::FindTrackByID( int trackID )
{
	Track_vector_constit it=m_Tracks.begin();
	Track_vector_constit itEnd=m_Tracks.end();
	while(it!=itEnd){
		if ((*it)->GetID()==trackID)
		{
			return *it;
		}
		it++;
	}
	return NULL;
}

Track_vector PhataTrackContainter::GetTrackVector( TRACKSTATE state )
{
	Track_vector tracvec;
	Track_vector_constit it=m_Tracks.begin();
	while(it!=m_Tracks.end()){
		if ((*it)->GetState()==state)
		{
			tracvec.push_back(*it);
		}
		++it;
	}
	return tracvec;
}
