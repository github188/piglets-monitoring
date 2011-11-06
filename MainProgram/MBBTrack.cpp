#include "MBBTrack.h"
#include "../MainProgram_GUI/cxlog.h"
#include <iostream>
using namespace std;
MBBTrack::MBBTrack(void)
	:PhataTrackBlobs()
    ,m_pdefaultFont(NULL)
	,m_maxID(0)
{
	m_pdefaultFont = new CvFont;
	cvInitFont(m_pdefaultFont, CV_FONT_HERSHEY_DUPLEX, 0.5, 0.5, 0, 1);
	// Other fonts:
	//   CV_FONT_HERSHEY_SIMPLEX, CV_FONT_HERSHEY_PLAIN,
	//   CV_FONT_HERSHEY_DUPLEX, CV_FONT_HERSHEY_COMPLEX,
	//   CV_FONT_HERSHEY_TRIPLEX, CV_FONT_HERSHEY_COMPLEX_SMALL,
	//   CV_FONT_HERSHEY_SCRIPT_SIMPLEX, CV_FONT_HERSHEY_SCRIPT_COMPLEX
}


MBBTrack::~MBBTrack(void)
{
	if(m_pdefaultFont){
		delete m_pdefaultFont;
		m_pdefaultFont=NULL;
	}
}

bool MBBTrack::DoInit( IplImage *ProtoFrame )
{
	PhataTrackBlobs::DoInit(ProtoFrame);
	return true;
}

bool MBBTrack::GetBlobsImg( findbolbs_func func,IplImage *srcImg,IplImage *blobImg,void* userdata )
{
	PhataTrackBlobs::GetBlobsImg(func,srcImg,blobImg,userdata);
	return true;
}

bool MBBTrack::UpdateTrack( const double thDistance, const unsigned int thInactive, const unsigned int thActive/*=0*/ )
{
	CV_FUNCNAME("MBBTrack::UpdateTrack");
	__CV_BEGIN__;
	int nblob=m_CurBlobs.GetContainterSize();
	int ntrack=m_CurTracks.GetContainterSize();
	bool isUseKalman=false;
	int *blob_track=new int[(nblob+1)*(ntrack+1)];//最后一行/列用于统计
	memset(blob_track,0,sizeof(int)*(nblob+1)*(ntrack+1));
	//得到各目标之间的距离关系
	for (int i=0; i<nblob; ++i){//Height
		for (int j=0; j<ntrack; ++j){//Width
			double distince=DistanceBetweenTrackAndBlob(*m_CurBlobs.GetBlob(i),*m_CurTracks.GetTrack(j));
			//cout<<i<<" "<<j<<" "<<distince<<endl;
			//LOG3(_T(" %d %d %f\r\n"),i,j,distince);
			if(distince > thDistance){
				blob_track[i*(ntrack+1) +j]=distince*1000;//千分位数
				if (m_CurTracks.GetTrack(j)->GetState()==TRACKSTATE_ACTIVE){
					blob_track[nblob*(ntrack+1)+j]++;//最后一行(tracks汇总）
					blob_track[i*(ntrack+1)+ntrack]++;//最后一列(blobs汇总）
				}
			}
		}
	}
	//调整blob_track，处理N:N的情况（断开两者的联系）
	processNVSN(blob_track,nblob,ntrack);

	////分析距离值,分5种情况（track VS blob）
	//track VS blob == 0 : 1  新目标出现or遮挡后出现
	//track VS blob == 1 : 0  目标消失、被遮挡
	//track VS blob == 1 : 1  目标正常，继续跟踪
	//track VS blob == 1 : N  目标分离：前1标志为分离，将N与合并列表中的目标匹配
	//track VS blob == N : 1  目标合并：前N标志为合并，将1与分离列表中的目标匹配
	vector<int> newObjectIndex;
	for (int j=0; j<ntrack; ++j)
	{
		CTrack *track=m_CurTracks.GetTrack(j);
		if (track->GetState()!=TRACKSTATE_ACTIVE){//只对活动的目标进行匹配
			continue;
		}
		if(blob_track[nblob*(ntrack+1)+j]==0){
			//track VS blob == 1 : 0  目标消失、被遮挡
			updateTrackState(j,TRACKSTATE_MISS);
		}else if(blob_track[nblob*(ntrack+1)+j]==1){
			//track VS blob == 1 : 1  目标正常，继续跟踪
			for (int i=0; i<nblob; ++i){
				if(blob_track[i*(ntrack+1)+j]!=0 && blob_track[i*(ntrack+1) +ntrack]==1){
					updateTrackState(j,TRACKSTATE_ACTIVE);
					track->UpdateBlob(*m_CurBlobs.GetBlob(i),isUseKalman);
					break;
				}
			}
		}else if(blob_track[nblob*(ntrack+1)+j] > 1){
			//track VS blob == 1 : N  目标分离：前1标志为分离，将N与合并列表中的目标匹配
			updateTrackState(j,TRACKSTATE_MERGE);
			for (int i=0; i<nblob; ++i){
				if(blob_track[i*(ntrack+1) +j]!=0){
					//与合并列表中的匹配，成功激活合并列表中的目标，失败则为新目标
					bool isNewTrack=true;
					for (int matchj=0; matchj<ntrack; ++matchj){
						if(blob_track[i*(ntrack+1)+matchj]>thDistance*1000 && m_CurTracks.GetTrack(matchj)->GetState()==TRACKSTATE_MERGE){
							updateTrackState(matchj,TRACKSTATE_ACTIVE);
							m_CurTracks.GetTrack(matchj)->UpdateBlob(*m_CurBlobs.GetBlob(i),isUseKalman);
							isNewTrack=false;
							break;
						}
					}
					if (isNewTrack){
						newObjectIndex.push_back(i);
					}
				}
			}
		}
	}
	for (int i=0; i<nblob; ++i){
		if (blob_track[i*(ntrack+1) +ntrack]==0)
		{//track VS blob == 0 : 1  新目标出现or遮挡后出现
			//与消失列表中的匹配，成功激活消失列表中的目标，失败则为新目标			
			bool isNewTrack=true;
			for (int matchj=0; matchj<ntrack; ++matchj){
				if(blob_track[i*(ntrack+1)+matchj]>thDistance*1000 && m_CurTracks.GetTrack(matchj)->GetState()==TRACKSTATE_MISS){
					updateTrackState(matchj,TRACKSTATE_ACTIVE);
					m_CurTracks.GetTrack(matchj)->UpdateBlob(*m_CurBlobs.GetBlob(i),isUseKalman);
					isNewTrack=false;
					break;
				}
			}
			if (isNewTrack){
				newObjectIndex.push_back(i);
			}
		}else if(blob_track[i*(ntrack+1) +ntrack]>1){
			//track VS blob == N : 1  目标合并：前N标志为合并，将1与分离列表中的目标匹配
			for (int j=0; j<ntrack; ++j){
				if(blob_track[i*(ntrack+1)+j]!=0){
					updateTrackState(j,TRACKSTATE_MERGE);
					//与分离列表中的匹配，成功激活分离列表中的目标，失败则为新目标
				}
			}
			bool isNewTrack=true;
			for (int matchj=0; matchj<ntrack; ++matchj){
				if(blob_track[i*(ntrack+1)+matchj]>thDistance*1000 && m_CurTracks.GetTrack(matchj)->GetState()==TRACKSTATE_SEPARATE){
					updateTrackState(matchj,TRACKSTATE_ACTIVE);
					m_CurTracks.GetTrack(matchj)->UpdateBlob(*m_CurBlobs.GetBlob(i),isUseKalman);
					isNewTrack=false;
					break;
				}
			}
			if (isNewTrack){
				newObjectIndex.push_back(i);
			}
		}
	}
	////添加新目标
	for (vector<int>::const_iterator it=newObjectIndex.begin(); it!=newObjectIndex.end(); ++it)
	{
		CTrack newTrack(*m_CurBlobs.GetBlob((*it)),++m_maxID,TRACKSTATE_ACTIVE);
		m_CurTracks.AddTrack(&newTrack);
	}
	////////丢弃已过时目标
	for (Track_vector_it it=m_CurTracks.m_Tracks.begin();it!=m_CurTracks.m_Tracks.end();)
	{
		CTrack *tmpTrack=*it;
		if (tmpTrack->m_inactive>thInactive && tmpTrack->GetState()==TRACKSTATE_MISS)
		{
			it=m_CurTracks.m_Tracks.erase(it);
		}else{
			++it;
		}
	}
	delete []blob_track;
	__CV_END__;
	return true;
}

bool MBBTrack::DrawTrack( IplImage *DrawImg,unsigned short mode/*=0x000f*/,CvFont *font/*=0*/ )
{
	CV_FUNCNAME("MBBTrack::DrawTrack");
	__CV_BEGIN__;
	CV_ASSERT(DrawImg&&(DrawImg->depth==IPL_DEPTH_8U)&&(DrawImg->nChannels==3));
	if ((mode&CV_TRACK_RENDER_ID)&&(!font)){
		font=m_pdefaultFont;
	}
	Track_vector& tmpTracks=m_CurTracks.m_Tracks;
	if (mode){
		for (Track_vector_constit it=tmpTracks.begin(); it!=tmpTracks.end(); ++it){
			if (mode&CV_TRACK_RENDER_ID){
				if ((*it)->GetState()==TRACKSTATE_ACTIVE &&((*it)->m_active>10 || ((*it)->m_active>0&& (*it)->m_lifetime>5))){
					stringstream buffer;
					buffer << (*it)->GetID();
					cvPutText(DrawImg, buffer.str().c_str(), cvPoint((int)(*it)->GetCentorid().x, (int)(*it)->GetCentorid().y), font, CV_RGB(0.,255.,0.));
				}
			}
			if (mode&CV_TRACK_RENDER_BOUNDING_BOX && ((*it)->m_active>10 || ((*it)->m_active>0&& (*it)->m_lifetime>5))){
				if ((*it)->GetState()==TRACKSTATE_ACTIVE)
					cvRectangleR(DrawImg, (*it)->m_rcBoundingBox, CV_RGB(0., 0., 255.));
				//else if ((*it)->GetState()==TRACKSTATE_MERGE)
				//	cvRectangleR(DrawImg, (*it)->m_rcBoundingBox, CV_RGB(255., 0., 0.));
				//else if ((*it)->GetState()==TRACKSTATE_MISS)
				//	cvRectangleR(DrawImg, (*it)->m_rcBoundingBox, CV_RGB(128, 128., 128.));
				//else if ((*it)->GetState()==TRACKSTATE_SEPARATE)
				//	cvRectangleR(DrawImg, (*it)->m_rcBoundingBox, CV_RGB( 0.,255., 0.));
			}
			if (mode&CV_TRACK_RENDER_TO_LOG)
			{
				clog << "Track " << (*it)->GetID() << endl;
				if ((*it)->m_inactive)
					clog << " - Inactive for " << (*it)->m_inactive << " frames" << endl;

				clog << " - Lifetime " << (*it)->m_lifetime << endl;
				clog << " - Active " << (*it)->m_active << endl;
				clog << " - State " << (*it)->GetState() << endl;
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
				cout << " - State " << (*it)->GetState() << endl;
				cout << " - Bounding box: (" << (*it)->m_rcBoundingBox.x << ", " << (*it)->m_rcBoundingBox.y << ") - (" 
					<< (*it)->m_rcBoundingBox.x+(*it)->m_rcBoundingBox.width << ", " 
					<< (*it)->m_rcBoundingBox.y+(*it)->m_rcBoundingBox.height << ")" << endl;
				cout << " - Centroid: (" << (*it)->GetCentorid().x << ", " << (*it)->GetCentorid().y << ")" << endl;
				cout << endl;
			}

			if (mode&CV_TRACK_RENDER_TO_CXLOG)
			{
				LOG1(_T("Track %d\r\n"),(*it)->GetID());
				if ((*it)->m_inactive)
					LOG1(_T(" - Inactive for %d frames\r\n"),(*it)->m_inactive);

				LOG1(_T(" - Lifetime %d\r\n"),(*it)->m_lifetime);
				LOG1(_T(" - Active %d\r\n"),(*it)->m_active);
				LOG1(_T(" - State %d\r\n"),(*it)->GetState());
				LOG4(_T(" - Bounding box:(%d,%d)-(%d,%d)\r\n"),(*it)->m_rcBoundingBox.x,(*it)->m_rcBoundingBox.y,
					(*it)->m_rcBoundingBox.x+(*it)->m_rcBoundingBox.width,
					(*it)->m_rcBoundingBox.y+(*it)->m_rcBoundingBox.height);
				LOG2(_T(" - Centroid:(%d,%d)\r\n"),(*it)->GetCentorid().x,(*it)->GetCentorid().y);
				LOG(_T("\r\n"));
			}
		}
	}
	__CV_END__;
	return true;
}

bool MBBTrack::processNVSN(int *blob_track,int nblob,int ntrack)
{
	bool isOK=true;
	for (int i=0; i<nblob; ++i){
		for (int j=0; j<ntrack; ++j){
			if (blob_track[i*(ntrack+1) +j]!=0 && blob_track[nblob*(ntrack+1)+j]>1 &&
				blob_track[i*(ntrack+1)+ntrack]>1)//N:N的情况
			{
				blob_track[i*(ntrack+1) +j]=0;
				blob_track[nblob*(ntrack+1)+j]--;//最后一列(tracks列）
				blob_track[i*(ntrack+1)+ntrack]--;//最后一行(blobs行）
				isOK=false;
			}
		}
	}
	if (isOK)
	{
		return true;
	}else{
		processNVSN(blob_track,nblob,ntrack);
	}
	return false;
}

void MBBTrack::updateTrackState( int indexTrack,TRACKSTATE State )
{
	m_CurTracks.GetTrack(indexTrack)->SetState(State);
}
