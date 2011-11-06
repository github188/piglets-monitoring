#include "PhataTrackBlobs.h"


PhataTrackBlobs::PhataTrackBlobs(void)
	:m_CurTracks()
	,m_CurBlobs()
{
}

PhataTrackBlobs::~PhataTrackBlobs(void)
{
}
bool PhataTrackBlobs::DoInit(IplImage *ProtoFrame)
{
	return true;
}

bool PhataTrackBlobs::GetBlobsImg( findbolbs_func func,IplImage *srcImg,IplImage *blobImg,void* userdata )
{
	if((*func)(srcImg,blobImg,userdata)==false)
		return false;

	m_CurBlobs.ClearBlobs();
	m_CurBlobs.findBlobs(blobImg,NULL,0);
	//m_CurBlobs.Filter(m_CurBlobs,B_INCLUDE, CBlobGetArea(), B_GREATER, 200 );//删除面积小的
	//m_CurBlobs.Filter(m_CurBlobs,B_INCLUDE, CBlobGetMaxX(), B_GREATER, 40 );//删除位置保温室的误检测
	m_CurBlobs.calHis(srcImg,HIS_RGB);
	return true;
}
