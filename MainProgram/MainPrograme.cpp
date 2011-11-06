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

#include "MBBTrack.h"
#include "..\MainProgram_GUI\FindPigObject.h"
bool InitFindPigParam(IplImage *img,findpigblobs_userdata &userdata)
{
	/////�����ֻ
	string ImgPath="..\\ConsoleTest\\ĸ���ͼ�ļ�";
	if (ImgPath.at(ImgPath.length()-1)!='\\')
	{
		ImgPath+='\\';
	}

	string LUPName;
	LUPName=ImgPath+"ĸ��Y_CrCbSMinMaxIndex���ұ�.dat";
	int row,col;
	int *table=0;
	CreateLUT(LUPName,&table,row,col);

	CvGaussBGStatModelParams params;
	params.win_size      = CV_BGFG_MOG_WINDOW_SIZE*0.5;
	params.bg_threshold  = CV_BGFG_MOG_BACKGROUND_THRESHOLD;
	params.std_threshold = CV_BGFG_MOG_STD_THRESHOLD;
	params.weight_init   = CV_BGFG_MOG_WEIGHT_INIT;
	params.variance_init = CV_BGFG_MOG_SIGMA_INIT*CV_BGFG_MOG_SIGMA_INIT;
	params.minArea       = CV_BGFG_MOG_MINAREA*10;
	params.n_gauss       = CV_BGFG_MOG_NGAUSSIANS-2;

	CvGaussBGModel *m_bg_model = (CvGaussBGModel*)cvCreateGaussianBGModel(img,&params );
	CvMemStorage *storage=cvCreateMemStorage();
	userdata.bg_model=m_bg_model;
	userdata.storage=storage;
	userdata.LUPTable=table;
	userdata.rows=row;
	userdata.cols=col;
	userdata.modelframes=0;
	userdata.isRemoveShadow=true;
	return true;
}
void ReleasePigParam(findpigblobs_userdata &userdata)
{
	ReleaseLUT(&userdata.LUPTable);
	cvReleaseMemStorage(&userdata.storage);
	cvReleaseBGStatModel((CvBGStatModel**)&userdata.bg_model);
}


int main (void)
{
	CvCapture* capture = 0;
	MBBTrack mbbtracks;
	capture = cvCaptureFromAVI(gVOneWander.c_str());
	if (!capture)
	{
		return -1;
	}

	IplImage *pframe=cvQueryFrame(capture);
	findpigblobs_userdata userdata;
	InitFindPigParam(pframe,userdata);
	IplImage*Blobmask=cvCreateImage(cvGetSize(pframe),IPL_DEPTH_8U,1);
	IplImage*tmpImg=cvCreateImage(cvGetSize(pframe),IPL_DEPTH_8U,3);
	mbbtracks.DoInit(pframe);
	while(pframe=cvQueryFrame(capture))
	{
		//cvLightingCompensation(pframe,pframe);
		mbbtracks.GetBlobsImg(FindPigBlobs,pframe,Blobmask,&userdata);
		cvShowImage("mbbtracks mask",Blobmask);

		mbbtracks.UpdateTrack(0.65,20);
		cvCopyImage(pframe,tmpImg);
		mbbtracks.DrawTrack(tmpImg,CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX /*| CV_TRACK_RENDER_TO_STD*/);		
		cvShowImage("ԭͼ��-mbbtrack���",tmpImg);
		cvSaveImage("mbbtrack���.jpg",tmpImg);
		if(cvWaitKey(5)==27) break;
	}
	ReleasePigParam(userdata);
	cvReleaseImage(&Blobmask);
	cvReleaseImage(&tmpImg);
	cvReleaseCapture(&capture);
	cvDestroyAllWindows();
	return 0;
}