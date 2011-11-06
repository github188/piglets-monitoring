#include "CvTrack.h"
#include <iostream>
using namespace std;
#include "../GoogleCode_video-tracker/opencv_cvBlobsLibV8.3/Blob.h"
#include "../GoogleCode_video-tracker/opencv_cvBlobsLibV8.3/BlobOperators.h"

// Access to matrix
#define C(blob, track) close[((blob) + (track)*(nBlobs+2))]
// Access to accumulators
#define Last_1_Blobs(label) C((label), (nTracks))
#define Last_1_Tracks(id) C((nBlobs), (id))
// Access to identifications
#define Last_Blobs(label) C((label), (nTracks)+1)
#define Last_Tracks(id) C((nBlobs)+1, (id))
// Access to registers
#define GetBlobByID(label) m_CurBlobs.FindBlobByID(Last_Blobs(label))
#define GetTrackByID(id) m_CurTracks.FindTrackByID(Last_Tracks(id))


double distantBlobTrack( CBlob *b, CTrack *t)
{
	double d1;
	CvPoint2D32f blobCentroid=cvPoint2D32f(CBlobGetXCenter()(*b),CBlobGetYCenter()(*b));
	double blobMinX=b->MinX();
	double blobMinY=b->MinY();
	double blobMaxX=b->MaxX();
	double blobMaxY=b->MaxY();
	double trackMinX=t->m_rcBoundingBox.x;
	double trackMinY=t->m_rcBoundingBox.y;
	double trackMaxX=t->m_rcBoundingBox.x+t->m_rcBoundingBox.width;
	double trackMaxY=t->m_rcBoundingBox.y+t->m_rcBoundingBox.height;
	CvPoint2D32f trackCentroid=cvPoint2D32f((trackMinX+trackMaxX)/2.,(trackMinY+trackMaxY)/2.);
	if (blobCentroid.x<trackMinX)//t的左上角在b的中点的右边
	{
		if (blobCentroid.y<trackMinY)//右下边
			d1 = MAX(trackMinX - blobCentroid.x, trackMinY - blobCentroid.y);
		else if (blobCentroid.y>trackMaxY)//右上边
			d1 = MAX(trackMinX - blobCentroid.x, blobCentroid.y - trackMaxY);
		else // if (trackMinY < b->centroid.y)&&(b->centroid.y < trackMaxY)
			d1 = trackMinX - blobCentroid.x;//右边
	}
	else if (blobCentroid.x>trackMaxX)//t的右下角在b的中点的左边
	{
		if (blobCentroid.y<trackMinY)//左下边
			d1 = MAX(blobCentroid.x - trackMaxX, trackMinY - blobCentroid.y);
		else if (blobCentroid.y>trackMaxY)//左上边
			d1 = MAX(blobCentroid.x - trackMaxX, blobCentroid.y - trackMaxY);
		else
			d1 = blobCentroid.x - trackMaxX;//左边
	}
	else // b的中心点>trackMinX && <trackMaxX
	{
		if (blobCentroid.y<trackMinY)
			d1 = trackMinY - blobCentroid.y;
		else if (blobCentroid.y>trackMaxY)
			d1 = blobCentroid.y - trackMaxY;
		else 
			return 0.;
	}

	double d2;
	if (trackCentroid.x<blobMinX)//b的左上角在t的中点的右边
	{
		if (trackCentroid.y<blobMinY)
			d2 = MAX(blobMinX - trackCentroid.x, blobMinY - trackCentroid.y);
		else if (trackCentroid.y>blobMaxY)
			d2 = MAX(blobMinX - trackCentroid.x, trackCentroid.y - blobMaxY);
		else // if (b->miny < trackCentroid.y)&&(trackCentroid.y < b->maxy)
			d2 = blobMinX - trackCentroid.x;
	}
	else if (trackCentroid.x>blobMaxX)//b的左上角在t的中点的左边
	{
		if (trackCentroid.y<blobMinY)
			d2 = MAX(trackCentroid.x - blobMaxX, blobMinY - trackCentroid.y);
		else if (trackCentroid.y>blobMaxY)
			d2 = MAX(trackCentroid.x - blobMaxX, trackCentroid.y - blobMaxY);
		else
			d2 = trackCentroid.x - blobMaxX;
	}
	else // t的中心点>b->minx && <b->maxx
	{
		if (trackCentroid.y<blobMinY)
			d2 = blobMinY - trackCentroid.y;
		else if (trackCentroid.y>blobMaxY)
			d2 = trackCentroid.y - blobMaxY;
		else 
			return 0.;
	}
	return MIN(d1, d2);
}

void CvTrack::getClusterForBlob(unsigned int blobPos, unsigned int *close, unsigned int nBlobs, unsigned int nTracks, 
	Blob_vector const &tmpBlobs, list<CBlob*> &bb, list<CTrack*> &tt)
{
	for (unsigned int j=0; j<nTracks; j++)
	{
		if (C(blobPos, j))
		{
			tt.push_back(GetTrackByID(j));

			unsigned int c = Last_1_Tracks(j);

			C(blobPos, j) = 0;
			Last_1_Blobs(blobPos)--;
			Last_1_Tracks(j)--;

			if (c>1)
			{
				getClusterForTrack(j, close, nBlobs, nTracks, tmpBlobs, bb, tt);
			}
		}
	}
}

void CvTrack::getClusterForTrack(unsigned int trackPos, unsigned int *close, unsigned int nBlobs, unsigned int nTracks, 
	Blob_vector const &tmpBlobs, list<CBlob*> &bb, list<CTrack*> &tt)
{
	for (unsigned int i=0; i<nBlobs; i++)
	{
		if (C(i, trackPos))
		{
			bb.push_back(GetBlobByID(i));

			unsigned int c = Last_1_Blobs(i);

			C(i, trackPos) = 0;
			Last_1_Blobs(i)--;
			Last_1_Tracks(trackPos)--;

			if (c>1)
			{
				getClusterForBlob(i, close, nBlobs, nTracks, tmpBlobs, bb, tt);
			}
		}
	}
}
CvTrack::CvTrack(void)
	:m_pdefaultFont(NULL)
{
	m_pdefaultFont = new CvFont;
	cvInitFont(m_pdefaultFont, CV_FONT_HERSHEY_DUPLEX, 0.5, 0.5, 0, 1);
	// Other fonts:
	//   CV_FONT_HERSHEY_SIMPLEX, CV_FONT_HERSHEY_PLAIN,
	//   CV_FONT_HERSHEY_DUPLEX, CV_FONT_HERSHEY_COMPLEX,
	//   CV_FONT_HERSHEY_TRIPLEX, CV_FONT_HERSHEY_COMPLEX_SMALL,
	//   CV_FONT_HERSHEY_SCRIPT_SIMPLEX, CV_FONT_HERSHEY_SCRIPT_COMPLEX
}

CvTrack::~CvTrack(void)
{
	if(m_pdefaultFont){
		delete m_pdefaultFont;
		m_pdefaultFont=NULL;
	}
}

bool CvTrack::DoInit( IplImage *ProtoFrame )
{
	PhataTrackBlobs::DoInit(ProtoFrame);
	return true;
}

bool CvTrack::GetBlobsImg( findbolbs_func func,IplImage *srcImg,IplImage *blobImg,void* userdata )
{
	PhataTrackBlobs::GetBlobsImg(func,srcImg,blobImg,userdata);
	return true;
}

bool CvTrack::UpdateTrack(const double thDistance, const unsigned int thInactive, const unsigned int thActive/*=0*/ )
{
	CV_FUNCNAME("CvTrack::UpdateTrack");
	__CV_BEGIN__;
	unsigned int nBlobs = m_CurBlobs.GetContainterSize();
	unsigned int nTracks = m_CurTracks.GetContainterSize();
	Blob_vector& tmpBlobs=m_CurBlobs.m_blobs;
	Track_vector& tmpTracks=m_CurTracks.m_Tracks;
	// Proximity matrix:
	// Last row/column is for ID/label.
	// Last-1 row/column is for accumulation.
	unsigned int *close = new unsigned int[(nBlobs+2)*(nTracks+2)]; // XXX Must be same type than CvLabel.
	try
	{
		// Inicialization:
		unsigned int i=0;
		for (Blob_vector_constit it = tmpBlobs.begin(); it!=tmpBlobs.end(); ++it, i++){
			Last_1_Blobs(i) = 0;
			Last_Blobs(i) = (*it)->GetID();
		}
		unsigned int maxTrackID = 0;
		unsigned int j=0;
		for (Track_vector_constit jt = tmpTracks.begin(); jt!=tmpTracks.end(); ++jt, j++){
			Last_1_Tracks(j) = 0;
			Last_Tracks(j) = (*jt)->GetID();
			if ((*jt)->GetID() > maxTrackID)
				maxTrackID = (*jt)->GetID();
		}

		// Proximity matrix calculation and "used blob" list inicialization:
		for (i=0; i<nBlobs; i++){
			for (j=0; j<nTracks; j++){
				if (C(i, j) = (distantBlobTrack(GetBlobByID(i), GetTrackByID(j)) < thDistance)){
					Last_1_Blobs(i)++;
					Last_1_Tracks(j)++;
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Detect inactive tracks
		for (j=0; j<nTracks; j++){
			unsigned int c = Last_1_Tracks(j);
			if (c==0){//此track与所有blobs都无关系，说明些track丢失
				CTrack *track = GetTrackByID(j);
				track->m_inactive++;
				track->SetID(0);
			}
		}

		// Detect new tracks
		for (i=0; i<nBlobs; i++){
			unsigned int c = Last_1_Blobs(i);
			if (c==0){//此blobs与所有tracks都无关系，说明是新的目标
				maxTrackID++;
				CBlob *blob = GetBlobByID(i);
				CTrack *track = new CTrack(*blob,maxTrackID,TRACKSTATE_ACTIVE);
				tmpTracks.push_back(track);
			}
		}
		// Clustering
		for (j=0; j<nTracks; j++){
			unsigned int c = Last_1_Tracks(j);
			if (c){
				list<CTrack*> tt; 
				tt.push_back(GetTrackByID(j));
				list<CBlob*> bb;
				getClusterForTrack(j, close, nBlobs, nTracks, tmpBlobs, bb, tt);
				// Select track
				CTrack *track;
				unsigned int area = 0;
				for (list<CTrack*>::const_iterator it=tt.begin(); it!=tt.end(); ++it){
					CTrack *t = *it;
					unsigned int a = (t->m_rcBoundingBox.width)*(t->m_rcBoundingBox.height);
					if (a>area){
						area = a;
						track = t;
					}
				}
				// Select blob
				CBlob *blob;
				area = 0;
				//cout << "Matching blobs: ";
				for (list<CBlob*>::const_iterator it=bb.begin(); it!=bb.end(); ++it){
					CBlob *b = *it;
					//cout << b->label << " ";
					if (CBlobGetArea()(*b)>area){
						area = CBlobGetArea()(*b);
						blob = b;
					}
				}
				// Update track
				*track=*blob;
				if (track->m_inactive)
					track->m_active = 0;
				track->m_inactive = 0;
				// Others to inactive
				for (list<CTrack*>::const_iterator it=tt.begin(); it!=tt.end(); ++it)
				{
					CTrack *t = *it;
					if (t!=track)
					{
						//cout << "Inactive: track=" << t->id << endl;
						//t->inactive++;
						//t->label = 0;
						t->m_inactive++;
					}
				}
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (Track_vector_it jt=tmpTracks.begin(); jt!=tmpTracks.end();){
			if ( (*jt)->m_inactive>=thInactive ||
				 ((*jt)->m_inactive && thActive && (*jt)->m_active<thActive) )
			{
				delete (*jt);
				jt=tmpTracks.erase(jt);
			}
			else
			{
				(*jt)->m_lifetime++;
				if (!(*jt)->m_inactive)
					(*jt)->m_active++;
				++jt;
			}
		}
	}
	catch (...)
	{
		delete[] close;
		throw;
	}
	delete[] close;
	__CV_END__;
	return true;
}
bool CvTrack::DrawTrack( IplImage *DrawImg,unsigned short mode/*=0x000f*/,CvFont *font/*=0*/ )
{
	CV_FUNCNAME("CvTrack::DrawTrack");
	__CV_BEGIN__;	
	CV_ASSERT(DrawImg&&(DrawImg->depth==IPL_DEPTH_8U)&&(DrawImg->nChannels==3));
	if ((mode&CV_TRACK_RENDER_ID)&&(!font)){
		font=m_pdefaultFont;
	}
	Track_vector& tmpTracks=m_CurTracks.m_Tracks;
	if (mode){
		for (Track_vector_constit it=tmpTracks.begin(); it!=tmpTracks.end(); ++it){
			if (mode&CV_TRACK_RENDER_ID){
				if (!(*it)->m_inactive){
					stringstream buffer;
					buffer << (*it)->GetID();
					cvPutText(DrawImg, buffer.str().c_str(), cvPoint((int)(*it)->GetCentorid().x, (int)(*it)->GetCentorid().y), font, CV_RGB(0.,255.,0.));
				}
			}
			if (mode&CV_TRACK_RENDER_BOUNDING_BOX){
				if ((*it)->m_inactive)
					cvRectangleR(DrawImg, (*it)->m_rcBoundingBox, CV_RGB(0., 0., 50.));
				else
					cvRectangleR(DrawImg, (*it)->m_rcBoundingBox, CV_RGB(0., 0., 255.));
			}
			if (mode&CV_TRACK_RENDER_TO_LOG)
			{
				clog << "Track " << (*it)->GetID() << endl;
				if ((*it)->m_inactive)
					clog << " - Inactive for " << (*it)->m_inactive << " frames" << endl;

				clog << " - Lifetime " << (*it)->m_lifetime << endl;
				clog << " - Active " << (*it)->m_active << endl;
				clog << " - Bounding box: (" << (*it)->m_rcBoundingBox.x << ", " << (*it)->m_rcBoundingBox.y << ") - (" 
					<< (*it)->m_rcBoundingBox.x+(*it)->m_rcBoundingBox.width << ", " 
					<< (*it)->m_rcBoundingBox.y+(*it)->m_rcBoundingBox.height << ")" << endl;
				clog << " - Centroid: (" << (*it)->GetCentorid().x << ", " << (*it)->GetCentorid().y << ")" << endl;
				clog << endl;
			}

			if (mode&CV_TRACK_RENDER_TO_STD)
			{
				cout << "Track " << (*it)->GetID() << endl;
				if ((*it)->m_inactive)
					cout << " - Inactive for " << (*it)->m_inactive << " frames" << endl;

				cout << " - Lifetime " << (*it)->m_lifetime << endl;
				cout << " - Active " << (*it)->m_active << endl;
				cout << " - Bounding box: (" << (*it)->m_rcBoundingBox.x << ", " << (*it)->m_rcBoundingBox.y << ") - (" 
					<< (*it)->m_rcBoundingBox.x+(*it)->m_rcBoundingBox.width << ", " 
					<< (*it)->m_rcBoundingBox.y+(*it)->m_rcBoundingBox.height << ")" << endl;
				cout << " - Centroid: (" << (*it)->GetCentorid().x << ", " << (*it)->GetCentorid().y << ")" << endl;
				cout << endl;
			}
		}
	}
	__CV_END__;
	return true;
}
