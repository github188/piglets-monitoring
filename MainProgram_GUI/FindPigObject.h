#pragma once
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <string>
using std::string;
//�ҳ�ͼ��surMark�е��������
CvSeq* GetMaxContour(IplImage *surMark, CvMemStorage* storage);

//�ԱȾ��ι�������Ƿ�����ֵ��Χ��
bool CommonArea(CvRect rectT1,CvRect rectT2,float threshold);

//table���ұ�rows���ұ�����,cols���ұ�����,usecols��ʹ�õ������������ܴ���cols
//pframe�Ĵ�СΪ(usecols+1)/2��pframe[0]Ϊ��������pframe[1]...pframe[(usecols+1)/2]Ϊ������
//���ݲ��ұ�����ֻ
void cvLookUpTable(int *table,const int Rows,const int Cols,const int usecols,IplImage** pframe, IplImage* mask);
bool CreateLUT(string LUTPath,int **table,int &row, int &col);
void ReleaseLUT(int **table);
typedef struct _tagfindpigblobs_userdata{
	CvGaussBGModel *bg_model;
	CvMemStorage *storage;
	int *LUPTable;
	int rows;
	int cols;
	int modelframes;//��ģ֡����������ģ20֡��ſ�ʼ���
	bool isRemoveShadow;//�Ƿ�ʹ����Ӱ����㷨ȥ����Ӱ
}findpigblobs_userdata;

bool FindPigBlobs(IplImage *srcImg, IplImage *maskImg,void* userdata);

bool GetAvgBackgroudImg(const IplImage *pFrame, IplImage *bgImg,double alpha);//���ڸ�˹������ģû���ر���������ʹ��ƽ���������Զ����ɱ���
////Improving shadow suppression in moving object detection with HSV color information�����Ӱ����㷨
//TS��TH�ֱ��ʾH������S��������ֵ����ͨ��ʵ����ȷ����������Ӱ���V����ֵͨ��С�ڷ���Ӱ����Ӧ��V����ֵ��
//���Ԧ�ȡֵӦ��С��1��������ȡֵ����Ҫ���ǳ������ߵ�ǿ��������Խǿ����ȡֵԽС
bool ShadowDetect(const IplImage *srcImg,const IplImage *bgImg,const IplImage *mask, const double Alpha, 
	const double Beta, const double TS, const double TH,IplImage *dstImg);