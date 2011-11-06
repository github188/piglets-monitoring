#include "Track.h"
const int stateNum=8;
const int measureNum=stateNum/2;
const float T=0.5;
#if stateNum==10
float transitionMat[stateNum][stateNum] ={//transition matrix(x,y,width,height,angle,dx,dy,dw,dh,da)
	1,0,0,0,0,T,0,0,0,0,
	0,1,0,0,0,0,T,0,0,0,
	0,0,1,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,T, 
	0,0,0,0,0,1,0,0,0,0, 
	0,0,0,0,0,0,1,0,0,0, 
	0,0,0,0,0,0,0,1,0,0, 
	0,0,0,0,0,0,0,0,1,0, 
	0,0,0,0,0,0,0,0,0,1, 
};
#else if stateNum==8
float transitionMat[stateNum][stateNum] ={//transition matrix(x,y,width,height,dx,dy,dw,dh)
	1,0,0,0,T,0,0,0,
	0,1,0,0,0,T,0,0,
	0,0,1,0,0,0,T,0,
	0,0,0,1,0,0,0,T,
	0,0,0,0,1,0,0,0, 
	0,0,0,0,0,1,0,0, 
	0,0,0,0,0,0,1,0, 
	0,0,0,0,0,0,0,1,
};
#endif
//公共面积计算，面积重合率计算
//参数：相比较的两个矩形
//返回：返回最大重合率
double CommonArea(CvRect rectT1,CvRect rectT2)
{
	if (rectT1.x>rectT2.x+rectT2.width||rectT2.x>rectT1.x+rectT1.width){
		return 0.;
	}
	if (rectT1.y>rectT2.y+rectT2.height||rectT2.y>rectT1.y+rectT1.height){
		return 0.;
	}
	int maxLeftTopX,maxLeftTopY,minRightButtonX,minRightButtonY;
	float commonArea,rectT1Area,rectT2Area;
	float rate1,rate2;
	if (rectT1.x-rectT2.x>0){
		maxLeftTopX=rectT1.x;
	}else{
		maxLeftTopX=rectT2.x;
	}
	if (rectT1.y-rectT2.y>0){
		maxLeftTopY=rectT1.y;
	}else{
		maxLeftTopY=rectT2.y;
	}
	if (rectT1.x+rectT1.width>rectT2.x+rectT2.width){
		minRightButtonX=rectT2.x+rectT2.width;
	}else{
		minRightButtonX=rectT1.x+rectT1.width;
	}
	if (rectT1.y+rectT1.height>rectT2.y+rectT2.height){
		minRightButtonY=rectT2.y+rectT2.height;
	}else{
		minRightButtonY=rectT1.y+rectT1.height;
	}
	commonArea=(float)(minRightButtonX-maxLeftTopX)*(minRightButtonY-maxLeftTopY);
	rectT1Area=(float)rectT1.height*rectT1.width;
	rectT2Area=(float)rectT2.height*rectT2.width;
	rate1=commonArea/rectT1Area;
	rate2=commonArea/rectT2Area;
	return rate1 >rate2 ? rate1 : rate2;
}

double DistanceBetweenTrackAndBlob( CBlob& blob, CTrack& track )
{
	CvRect blobrc=blob.GetBoundingBox();
	CvRect trackrc=track.m_rcBoundingBox;
	double dcommArea=CommonArea(blobrc,trackrc);
	double dhis[3];
	for (int i=0; i<3; ++i){
		blob.GetHistogram(i)->CompareHist(track.GetHistogram(i),CV_COMP_BHATTACHARYYA,dhis[i]);
		dcommArea+=1-dhis[i];
	}
	dcommArea/=4;
	return dcommArea;
}
CTrack::CTrack()
	:CBlob()
	,m_kalmanblob(new KalmanBlob(stateNum,measureNum,0))
	,m_lifetime(0)
	,m_active(0)
	,m_inactive(0)
{
	memcpy( m_kalmanblob->transitionMatrix->data.fl,transitionMat, sizeof(transitionMat));
}

CTrack::CTrack(CBlob &blob,int id,TRACKSTATE state)
	:CBlob(blob)
	,m_trackid(id)
	,m_trackState(state)
	,m_kalmanblob(new KalmanBlob(stateNum,measureNum,0))
	,m_lifetime(0)
	,m_active(0)
	,m_inactive(0)
{
	memcpy( m_kalmanblob->transitionMatrix->data.fl,transitionMat, sizeof(transitionMat));
	m_rcBoundingBox=blob.GetBoundingBox();
	m_box=blob.GetEllipse();
	if (stateNum==10)
	{
		m_kalmanblob->statePost->data.fl[0]=m_box.center.x;
		m_kalmanblob->statePost->data.fl[1]=m_box.center.y;
		m_kalmanblob->statePost->data.fl[2]=m_box.size.width;
		m_kalmanblob->statePost->data.fl[3]=m_box.size.height;
		m_kalmanblob->statePost->data.fl[4]=m_box.angle;
	}else if (stateNum==8)
	{
		m_kalmanblob->statePost->data.fl[0]=m_rcBoundingBox.x;
		m_kalmanblob->statePost->data.fl[1]=m_rcBoundingBox.y;
		m_kalmanblob->statePost->data.fl[2]=m_rcBoundingBox.width;
		m_kalmanblob->statePost->data.fl[3]=m_rcBoundingBox.height;
	}
}

CTrack::CTrack( CTrack &srcTrack )
	:m_kalmanblob(new KalmanBlob(stateNum,measureNum,0))
	,m_lifetime(0)
	,m_active(0)
	,m_inactive(0)
{
	memcpy( m_kalmanblob->transitionMatrix->data.fl,transitionMat, sizeof(transitionMat));
	*this=srcTrack;
}

CTrack::CTrack( CTrack *srcTrack )
	:m_kalmanblob(new KalmanBlob(stateNum,measureNum,0))
	,m_lifetime(0)
	,m_active(0)
	,m_inactive(0)
{
	memcpy( m_kalmanblob->transitionMatrix->data.fl,transitionMat, sizeof(transitionMat));
	if (srcTrack!=NULL)
	{
		*this=*srcTrack;
	}
}
CTrack::CTrack( CBlob &srcBlob )
	:m_kalmanblob(new KalmanBlob(stateNum,measureNum,0))
	,m_lifetime(0)
	,m_active(0)
	,m_inactive(0)
{
	*this=srcBlob;
}

CTrack::CTrack( CBlob *srcBlob )
	:m_kalmanblob(new KalmanBlob(stateNum,measureNum,0))
	,m_lifetime(0)
	,m_active(0)
	,m_inactive(0)
{
	if (srcBlob!=NULL)
	{
		*this=*srcBlob;
	}
}

CTrack::~CTrack(void)
{
}


CTrack& CTrack::operator=( const CTrack &srcTrack )
{
	if(this != &srcTrack){
		this->CBlob::operator=(srcTrack);
		this->m_lifetime=srcTrack.m_lifetime;
		this->m_active=srcTrack.m_active;
		this->m_inactive=srcTrack.m_inactive;
		this->m_rcBoundingBox=srcTrack.m_rcBoundingBox;
		this->m_box=srcTrack.m_box;
		this->m_kalmanblob=srcTrack.m_kalmanblob;
		this->SetState(srcTrack.GetState());
		this->SetID(srcTrack.GetID());
	}
	return *this;
}
CTrack& CTrack::operator=( const CBlob &srcBlob )
{
	if(this != &srcBlob){
		CBlob tmpBlob(srcBlob);
		this->CBlob::operator=(tmpBlob);
		m_rcBoundingBox=tmpBlob.GetBoundingBox();
		m_box=tmpBlob.GetEllipse();
		if (stateNum==10)
		{
			m_kalmanblob->statePost->data.fl[0]=m_box.center.x;
			m_kalmanblob->statePost->data.fl[1]=m_box.center.y;
			m_kalmanblob->statePost->data.fl[2]=m_box.size.width;
			m_kalmanblob->statePost->data.fl[3]=m_box.size.height;
			m_kalmanblob->statePost->data.fl[4]=m_box.angle;
		}else if (stateNum==8)
		{
			m_kalmanblob->statePost->data.fl[0]=m_rcBoundingBox.x;
			m_kalmanblob->statePost->data.fl[1]=m_rcBoundingBox.y;
			m_kalmanblob->statePost->data.fl[2]=m_rcBoundingBox.width;
			m_kalmanblob->statePost->data.fl[3]=m_rcBoundingBox.height;
		}
	}
	return *this;
}

void CTrack::SetState( TRACKSTATE trackState )
{
	m_trackState=trackState;
	switch(trackState){
	case TRACKSTATE_ACTIVE:
		this->m_lifetime++;
		this->m_active++;
		this->m_inactive=0;
		break;
	case TRACKSTATE_MISS:
		//this->m_lifetime++;
		this->m_active=0;
		this->m_inactive++;
		break;
	case TRACKSTATE_MERGE:
		//this->m_lifetime++;
		this->m_active=0;
		this->m_inactive++;
		break;
	case TRACKSTATE_SEPARATE:
		//this->m_lifetime++;
		this->m_active=0;
		this->m_inactive++;
		break;
	default:
		break;
	}
}

void CTrack::UpdateBlob( CBlob &blob,bool isUseKalman/*=false*/)
{
	this->CBlob::operator=(blob);
	if(isUseKalman){
		const CvMat*predict=m_kalmanblob->predict();//预测结果
		m_box=blob.GetEllipse();
		m_rcBoundingBox=blob.GetBoundingBox();
		float fmeasure[measureNum];
		CvMat blobMat=cvMat(measureNum,1,CV_32FC1,fmeasure);
		if (measureNum==5)
		{
			fmeasure[0]=m_box.center.x;
			fmeasure[1]=m_box.center.y;
			fmeasure[2]=m_box.size.width;
			fmeasure[3]=m_box.size.height;
			fmeasure[4]=m_box.angle;
			m_kalmanblob->correct(&blobMat);//加上测量结果得到最优估算
			m_box.center.x=predict->data.fl[0];
			m_box.center.y=predict->data.fl[1];
			m_box.size.width=predict->data.fl[2];
			m_box.size.height=predict->data.fl[3];
			m_box.angle=predict->data.fl[4];
			m_rcBoundingBox=box2D2rect(m_box);
		}else if(measureNum==4)
		{
			fmeasure[0]=m_rcBoundingBox.x;
			fmeasure[1]=m_rcBoundingBox.y;
			fmeasure[2]=m_rcBoundingBox.width;
			fmeasure[3]=m_rcBoundingBox.height;
			predict=m_kalmanblob->correct(&blobMat);//加上测量结果得到最优估算
			m_rcBoundingBox.x=predict->data.fl[0];
			m_rcBoundingBox.y=predict->data.fl[1];
			m_rcBoundingBox.width=predict->data.fl[2];
			m_rcBoundingBox.height=predict->data.fl[3];
		}
	}else{
		m_rcBoundingBox=blob.GetBoundingBox();
		m_box=blob.GetEllipse();
		const CvMat*predict=m_kalmanblob->predict();//预测结果
		float fmeasure[measureNum];
		CvMat blobMat=cvMat(measureNum,1,CV_32FC1,fmeasure);
		if (measureNum==5)
		{
			fmeasure[0]=m_box.center.x;
			fmeasure[1]=m_box.center.y;
			fmeasure[2]=m_box.size.width;
			fmeasure[3]=m_box.size.height;
			fmeasure[4]=m_box.angle;
			m_kalmanblob->correct(&blobMat);//加上测量结果得到最优估算
		}else if(measureNum==4)
		{
			fmeasure[0]=m_rcBoundingBox.x;
			fmeasure[1]=m_rcBoundingBox.y;
			fmeasure[2]=m_rcBoundingBox.width;
			fmeasure[3]=m_rcBoundingBox.height;
			m_kalmanblob->correct(&blobMat);//加上测量结果得到最优估算
		}
	}
}
