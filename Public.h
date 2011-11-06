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
const string gVRun="..\\1奔跑.avi";
const string gVOneRun="..\\1单只奔跑.avi";
const string gVOneWander="..\\1单只漫步.avi";
const string gVOneWander2="..\\1单只漫步2.avi";
const string gVFire="..\\1打架.avi";
const string gVTwoPigSideBySide="..\\2二小猪并排走.avi";
const string gVCloseToSow="..\\1接近母猪.avi";
const string gVSowStand2LieDown="..\\3母猪由站立到趴卧.avi";
const string gVSowStand2LieDown2="..\\4母猪由站立到趴卧2.avi";
const string gVOnlyStandSow="..\\1只有站立的母猪.avi";

void sincos( int angle, float& cosval, float& sinval );

void cvPersonSkinSegment(IplImage* img, IplImage* mask);
void cvPigSkinSegment(IplImage* img, IplImage* mask);
vector<string> GetAllFileName(string folde,string suffix, bool ishavepath=false);

void cvSuperLightingCompensation(const IplImage *inimg, IplImage *outimg);
void cvMyLightingCompensation(const IplImage *inimg, IplImage *outimg);
void cvInternetLightingCompensation(const IplImage *inimg, IplImage *outimg);

CvRect GetMinRect(IplImage* srcImg, CvScalar bgColor=CV_RGB(0,0,0));//返回图像的外包围矩形范围
bool CutDownImage(IplImage* srcImg, IplImage **dstImg,CvScalar bgColor=CV_RGB(0,0,0));//裁减图像（根据图像的最小包围矩形提取图像）
CvSeq* GetMaxContour(IplImage *surMark, CvMemStorage* storage);//获取图像的最大轮廓
CvBox2D DrawMinAreaRect(IplImage *srcImg,CvSeq *contour,CvScalar color=CV_RGB(255,0,0));//在图像srcImg上根据contour轮廓画上最小外接矩形
CvBox2D DrawMinAreaEllipse(IplImage *srcImg,CvSeq *contour,CvScalar color=CV_RGB(255,0,0));//在图像srcImg上根据contour轮廓画拟合椭圆
bool CommonArea(CvRect rectT1,CvRect rectT2,float threshold);//判断两个矩形的共同部分是否大于threshold