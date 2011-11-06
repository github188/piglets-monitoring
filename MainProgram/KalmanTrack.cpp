#include "KalmanTrack.h"

KalmanTrack::KalmanTrack(void)
{
}


KalmanTrack::~KalmanTrack(void)
{
}

bool KalmanTrack::DoInit( IplImage *ProtoFrame )
{
	PhataTrackBlobs::DoInit(ProtoFrame);
	return true;
}

bool KalmanTrack::GetBlobsImg( findbolbs_func func,IplImage *srcImg,IplImage *blobImg,void* userdata )
{
	PhataTrackBlobs::GetBlobsImg(func,srcImg,blobImg,userdata);
	return true;
}

bool KalmanTrack::UpdateTrack( const double thDistance, const unsigned int thInactive, const unsigned int thActive/*=0*/ )
{
	int nBlobs=m_CurBlobs.GetContainterSize();
	bool *isTrackHasBlob=new bool[m_kalmans.size()];
	memset(isTrackHasBlob,false,sizeof(bool)*m_kalmans.size());
	for (int i=0; i<nBlobs; ++i)
	{
		bool isBlobfindTrack=false;
		CBlob *curBlob=m_CurBlobs.GetBlob(i);
		CvRect rc=curBlob->GetBoundingBox();
		KalmanTrackInfo *findInfo=0;
		for(int j=0; j<m_kalmans.size(); ++j)
		{
			//blob找到相对应的track
			isBlobfindTrack=true;
			isTrackHasBlob[j]=true;
			findInfo=m_kalmans.at(j);
			break;
		}
		if(isBlobfindTrack){
			//kalman更新track
			//findInfo->kalmanblob->getNext(0.12);
		}else{
			//新blob,加入track;
			KalmanTrackInfo *info=new KalmanTrackInfo;
			info->active=0;
			info->blob=*curBlob;
			info->inactive=0;
			info->lifetime=0;
			info->trackid=m_kalmans.at(m_kalmans.size()-1)->trackid+1;
			info->kalmanblob=new KalmanBlob();
		}
	}

	////////查找那些track丢失了
	KalmanTrackInfo_Vector tmpVector;
	for(int j=0; j<m_kalmans.size(); ++j)
	{
		KalmanTrackInfo *info=m_kalmans.at(j);
		if(isTrackHasBlob[j]==false){
			//track丢失，进行丢失处理
				delete info->kalmanblob;
				delete info;
		}else{
			tmpVector.push_back(info);
		}
	}
	m_kalmans=tmpVector;
	delete []isTrackHasBlob;
	return true;
}

bool KalmanTrack::DrawTrack( IplImage *DrawImg,unsigned short mode/*=0x000f*/,CvFont *font/*=0*/ )
{
	return true;
}
