#pragma once
#pragma warning(disable:4996)
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <queue>
#include <map>
#include <io.h>
using namespace std;
#if CV_MAJOR_VERSION==2 && CV_MINOR_VERSION==1
#ifdef _DEBUG
#pragma comment(lib,"ipcommd.lib")
#pragma comment(lib,"ipstddatad.lib")
#pragma comment(lib,"ipguid.lib")
#pragma comment(lib,"ipalgod.lib")
#pragma comment(lib,"iplearnd.lib")
#else
#pragma comment(lib,"ipcomm.lib")
#pragma comment(lib,"ipstddata.lib")
#pragma comment(lib,"ipgui.lib")
#pragma comment(lib,"ipalgo.lib")
#pragma comment(lib,"iplearn.lib")
#endif
#endif
const string gVRun="..\\1����.avi";
const string gVOneRun="..\\1��ֻ����.avi";
const string gVOneWander="..\\1��ֻ����.avi";
const string gVOneWander2="..\\1��ֻ����2.avi";
const string gVFire="..\\1���.avi";
const string gVTwoPigSideBySide="..\\2��С������.avi";
const string gVCloseToSow="..\\1�ӽ�ĸ��.avi";
const string gVSowStand2LieDown="..\\3ĸ����վ����ſ��.avi";
const string gVSowStand2LieDown2="..\\4ĸ����վ����ſ��2.avi";
const string gVOnlyStandSow="..\\1ֻ��վ����ĸ��.avi";

void sincos( int angle, float& cosval, float& sinval );

void cvPersonSkinSegment(IplImage* img, IplImage* mask);
void cvPigSkinSegment(IplImage* img, IplImage* mask);
vector<string> GetAllFileName(string folde,string suffix, bool ishavepath=false);

void cvSuperLightingCompensation(const IplImage *inimg, IplImage *outimg);
void cvMyLightingCompensation(const IplImage *inimg, IplImage *outimg);
void cvInternetLightingCompensation(const IplImage *inimg, IplImage *outimg);

CvRect GetMinRect(IplImage* srcImg, CvScalar bgColor=CV_RGB(0,0,0));//����ͼ������Χ���η�Χ
bool CutDownImage(IplImage* srcImg, IplImage **dstImg,CvScalar bgColor=CV_RGB(0,0,0));//�ü�ͼ�񣨸���ͼ�����С��Χ������ȡͼ��
CvSeq* GetMaxContour(IplImage *surMark, CvMemStorage* storage);//��ȡͼ����������
CvBox2D DrawMinAreaRect(IplImage *srcImg,CvSeq *contour,CvScalar color=CV_RGB(255,0,0));//��ͼ��srcImg�ϸ���contour����������С��Ӿ���
CvBox2D DrawMinAreaEllipse(IplImage *srcImg,CvSeq *contour,CvScalar color=CV_RGB(255,0,0));//��ͼ��srcImg�ϸ���contour�����������Բ
bool CommonArea(CvRect rectT1,CvRect rectT2,float threshold);//�ж��������εĹ�ͬ�����Ƿ����threshold