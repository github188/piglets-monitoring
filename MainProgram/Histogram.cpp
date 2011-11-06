
#include "Histogram.h"
using std::min;
using std::max;
using std::abs;
#include <iostream>
#include <windows.h>
using namespace std;
int HistogramBins = 256;
float HistogramRange1[2]={0,255};
float *HistogramRange[1]={&HistogramRange1[0]};

Histogram::~Histogram(void) {
   ReleaseHis();
}
Histogram::Histogram(void)
:hist_(0)
,roiImg_(0)
{

}
void Histogram::createHistogram(IplImage* frame,CvRect roiRc,IplImage* mask/*=0*/) {
	assert( (!CV_IS_IMAGE(mask) && CV_IS_IMAGE(frame) &&  frame->nChannels==1) || 
		(CV_IS_IMAGE(frame) && CV_IS_IMAGE(mask) && frame->width==mask->width
		&& frame->height==mask->height && frame->nChannels==1 && mask->nChannels==1) );
	ReleaseHis();
	hist_=cvCreateHist(1,&HistogramBins,CV_HIST_ARRAY,HistogramRange);
	IplImage *roiMask=0;
	if (frame->width==roiRc.width && frame->height==roiRc.height && roiRc.x==roiRc.y==0)
	{
		roiImg_=cvCloneImage(frame);
		roiMask=mask;
	} 
	else
	{
		roiImg_=cvCreateImage(cvSize(roiRc.width,roiRc.height),IPL_DEPTH_8U,1);
		CvRect rc=cvGetImageROI(frame);
		cvSetImageROI(frame,roiRc);
		cvCopyImage(frame,roiImg_);
		cvSetImageROI(frame,rc);
		if(mask){
			roiMask=cvCreateImage(cvSize(roiRc.width,roiRc.height),IPL_DEPTH_8U,1);
			rc=cvGetImageROI(mask);
			cvSetImageROI(mask,roiRc);
			cvCopyImage(mask,roiMask);
			cvSetImageROI(mask,rc);
		}else{
			roiMask=mask;
		}
	}
	cvCalcHist(&roiImg_,hist_,0,roiMask);
	cvNormalizeHist(hist_,1);//归一化直方图 
    if(roiMask!=mask)cvReleaseImage(&roiMask);
}

Histogram::Histogram( IplImage *srcImg,CvRect roiRc,IplImage* mask/*=0*/ )
	:hist_(0)
	,roiImg_(0)
{
	createHistogram(srcImg,roiRc,mask);
}

Histogram::Histogram( Histogram &srchis )
	:hist_(0)
	,roiImg_(0)
{
	*this = srchis;
}

Histogram::Histogram( Histogram *srchis )
	:hist_(0)
	,roiImg_(0)
{
	if (srchis != NULL )
	{
		*this = *srchis;
	}

}

Histogram& Histogram::operator=( const Histogram &srchis )
{
	if(this != &srchis && srchis.hist_){
		cvCopyHist(srchis.hist_,&this->hist_);
		if(this->roiImg_)cvReleaseImage(&roiImg_);
		if(srchis.roiImg_)this->roiImg_=cvCloneImage(srchis.roiImg_);
	}
	return *this;
}

void Histogram::ReleaseHis()
{
	if (hist_) cvReleaseHist(&hist_);
	if(this->roiImg_)cvReleaseImage(&this->roiImg_);
}

bool Histogram::CompareHist( const Histogram* hist, int method, double &distance )
{
	if (CV_IS_HIST(this->hist_) && CV_IS_HIST(hist->hist_))
	{
		distance=cvCompareHist(this->hist_, hist->hist_, method );
		return true;
	}
	return false;
}
