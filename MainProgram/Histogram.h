
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
	// Method:    CompareHist--�Ƚ���hist�ľ���
	// FullName:  Histogram::CompareHist
	// Returns:   bool �ɹ�����true,ʧ�ܷ���false
	// Parameter: const Histogram * hist1 Ҫ��֮�Ƚϵ�Histogram
	// Parameter: int method �������ķ�������������
	//  CV_COMP_CORREL 
	//	CV_COMP_CHISQR 
	//	CV_COMP_INTERSECT 
	//	CV_COMP_BHATTACHARYYA 
	// Parameter: double & distance ����ֵ������ֵΪtrueʱ��ֵ����
	//************************************
	bool CompareHist( const Histogram* hist, int method, double &distance);
private:
	//************************************
	// Method:    createHistogram--����ͼ������ֱ��ͼ
	// FullName:  Histogram::createHistogram
	// Returns:   void
	// Parameter: IplImage * frame--ͼ��
	// Parameter: CvRect roiRc--ͼ���roi����
	// Parameter: IplImage * mask--��ģ��Ĭ��Ϊ�գ���ʾ���������ͼ���ֱ��ͼ
	//************************************
	void createHistogram(IplImage* frame, CvRect roiRc,IplImage* mask=0);
	//************************************
	// Method:    ReleaseHis--�ͷ��ڴ�
	// FullName:  Histogram::ReleaseHis
	// Returns:   void
	//************************************
	void ReleaseHis();
private:
	CvHistogram *hist_;
	IplImage* roiImg_;
};
#endif
