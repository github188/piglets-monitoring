
#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__
#include "cv.h"
class Histogram {
public:
	Histogram(void);
	Histogram(IplImage *srcImg,CvRect roiRc,IplImage* mask=0);
	Histogram(Histogram &srchis);
	Histogram(Histogram *srchis);
	Histogram& operator=(const Histogram &srchis);
	~Histogram(void);
	//************************************
	// Method:    CompareHist--比较与hist的距离
	// FullName:  Histogram::CompareHist
	// Returns:   bool 成功返回true,失败返回false
	// Parameter: const Histogram * hist1 要与之比较的Histogram
	// Parameter: int method 计算距离的方法，参数如下
	//  CV_COMP_CORREL 
	//	CV_COMP_CHISQR 
	//	CV_COMP_INTERSECT 
	//	CV_COMP_BHATTACHARYYA 
	// Parameter: double & distance 距离值，返回值为true时该值可用
	//************************************
	bool CompareHist( const Histogram* hist, int method, double &distance);
private:
	//************************************
	// Method:    createHistogram--根据图像生成直方图
	// FullName:  Histogram::createHistogram
	// Returns:   void
	// Parameter: IplImage * frame--图像
	// Parameter: CvRect roiRc--图像的roi区域
	// Parameter: IplImage * mask--掩模，默认为空，表示计算机整副图像的直方图
	//************************************
	void createHistogram(IplImage* frame, CvRect roiRc,IplImage* mask=0);
	//************************************
	// Method:    ReleaseHis--释放内存
	// FullName:  Histogram::ReleaseHis
	// Returns:   void
	//************************************
	void ReleaseHis();
private:
	CvHistogram *hist_;
	IplImage* roiImg_;
};
#endif
