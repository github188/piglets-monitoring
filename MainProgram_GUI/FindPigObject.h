#pragma once
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <string>
using std::string;
//找出图像surMark中的最大轮廓
CvSeq* GetMaxContour(IplImage *surMark, CvMemStorage* storage);

//对比矩形共有面积是否在阈值范围内
bool CommonArea(CvRect rectT1,CvRect rectT2,float threshold);

//table查找表，rows查找表行数,cols查找表列数,usecols所使用到的列数，不能大于cols
//pframe的大小为(usecols+1)/2，pframe[0]为主分量，pframe[1]...pframe[(usecols+1)/2]为辅分量
//根据查找表检测猪只
void cvLookUpTable(int *table,const int Rows,const int Cols,const int usecols,IplImage** pframe, IplImage* mask);
bool CreateLUT(string LUTPath,int **table,int &row, int &col);
void ReleaseLUT(int **table);
typedef struct _tagfindpigblobs_userdata{
	CvGaussBGModel *bg_model;
	CvMemStorage *storage;
	int *LUPTable;
	int rows;
	int cols;
	int modelframes;//建模帧数，背景建模20帧后才开始检测
	bool isRemoveShadow;//是否使用阴影检测算法去除阴影
}findpigblobs_userdata;

bool FindPigBlobs(IplImage *srcImg, IplImage *maskImg,void* userdata);

bool GetAvgBackgroudImg(const IplImage *pFrame, IplImage *bgImg,double alpha);//由于高斯背景建模没返回背景，所以使用平均背景法自动生成背景
////Improving shadow suppression in moving object detection with HSV color information里的阴影检测算法
//TS，TH分别表示H分量、S分量的阈值，可通过实验来确定。因数阴影点的V分量值通常小于非阴影点相应的V分量值，
//所以β取值应该小于1，而α的取值则需要考虑场景光线的强弱，光线越强，α取值越小
bool ShadowDetect(const IplImage *srcImg,const IplImage *bgImg,const IplImage *mask, const double Alpha, 
	const double Beta, const double TS, const double TH,IplImage *dstImg);