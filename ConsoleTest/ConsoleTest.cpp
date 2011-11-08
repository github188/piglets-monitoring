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


#define MYTYPE 4//1--截图小程序
                //2--统计猪只颜色图像中的Cr与Cb分量的范围
                //3--基于查找表的测试猪只视频
                //4--opencv自带肤色检测例子
//#include "..\Public.h"
#if MYTYPE==1
///////截图小程序(根据鼠标选择区域截图视频中的感兴趣区域保存为图像：保存于指定目录)
#include <iostream>
#include <algorithm>
using namespace std;
IplImage *pframe=NULL;//每一帧图像
CvRect grc;//选择的区域
bool isDrawing=false;
string savePath=".\\母猪截图文件\\";
void on_mouse( int event, int x, int y, int flags, void* frame)
{
	IplImage *img=(IplImage*)frame;
	if( !img )
		return;
	IplImage *drawImg=NULL;	
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		isDrawing=true;
		if (x<0) x=0;
		if (y<0) y=0;
		if (x>pframe->width) x=pframe->width;
		if (y>pframe->height)y=pframe->height;
		grc=cvRect(x,y,0,0);
		break;
	case  CV_EVENT_LBUTTONUP:
		isDrawing=false;
	
		if (x<0) x=0;
		if (y<0) y=0;
		if (x>pframe->width) x=pframe->width;
		if (y>pframe->height)y=pframe->height;		
		grc.width=abs(x-grc.x);
		grc.height=abs(y-grc.y);		
		grc.x= grc.x > x ? x : grc.x;
		grc.y= grc.y > y ? y : grc.y;
		if (grc.width<3 ||grc.width<3)
		{
			return;
		}
		drawImg=cvCloneImage(img);
		cvRectangleR(drawImg,grc,CV_RGB(255,0,0));
		cvShowImage( "Video", drawImg );
		{
			cvSetImageROI(img,grc);
			IplImage *curImg=cvCreateImage(cvSize(grc.width,grc.height),img->depth,img->nChannels);
			cvCopyImage(img,curImg);
			cvShowImage("1",curImg);
			//保存截图文件
			vector<string> typefilenames=GetAllFileName(savePath,"*.jpg");
			vector<string>::const_iterator constit=typefilenames.begin();
			vector<int> fileNum;
			for (;constit!=typefilenames.end(); ++constit)//查找目的文件夹已经指定字符图像数量
			{
				string name=(*constit);
				name=name.substr(0,name.find_last_of('.'));
				int num=atoi(name.c_str());
				fileNum.push_back(num);
			}
			int index=fileNum.empty()? -1 : *max_element(fileNum.begin(),fileNum.end());//从已有编号开始
			stringstream strstream;
			strstream<<savePath<<++index<<".jpg";
			string saveimgname=strstream.str();
			cvSaveImage(saveimgname.c_str(),curImg);
			//完成保存截图文件
			cvResetImageROI(img);
			cvReleaseImage(&curImg);
		}
		cvReleaseImage(&drawImg);
		break;
	case  CV_EVENT_MOUSEMOVE:
		if (isDrawing)
		{
			if (x<0) x=0;
			if (y<0) y=0;
			if (x>pframe->width) x=pframe->width;
			if (y>pframe->height)y=pframe->height;
			grc.width=abs(x-grc.x);
			grc.height=abs(y-grc.y);
			CvRect tmpRC=grc;
			tmpRC.x= tmpRC.x > x ? x : tmpRC.x;
			tmpRC.y= tmpRC.y > y ? y : tmpRC.y;
			if (tmpRC.width<3 ||tmpRC.width<3)
			{
				return;
			}
			drawImg=cvCloneImage(img);
			cvRectangleR(drawImg,tmpRC,CV_RGB(255,0,0));
			cvShowImage( "Video", drawImg );
			cvReleaseImage(&drawImg);
		}
		break;
	default:
		break;
	}
	if(drawImg) cvReleaseImage(&drawImg);
}

int main( int argc, char** argv )
{ 
	if (savePath.at(savePath.length()-1)!='\\')
	{
		savePath+='\\';
	}

	CvCapture* capture = 0;
	vector<string> filenames=GetAllFileName(".","*.avi",true);
	cvNamedWindow("Video");
	for (int i=0; i<filenames.size(); ++i)
	{
		capture = cvCaptureFromAVI(filenames.at(i).c_str());
		pframe=cvQueryFrame(capture);
		cvLightingCompensation(pframe,pframe);
		if(!pframe) return -1;
		cvSetMouseCallback("Video",on_mouse,pframe);
		if(!capture) return -1;
		while(pframe=cvQueryFrame(capture))
		{
			cvShowImage("Video",pframe);
			if(cvWaitKey(0)==27) break;
		}
		cvReleaseCapture(&capture);
	}
	cvDestroyAllWindows();
	return 0;
} 

#elif MYTYPE==2
///////统计猪只颜色图像中的Cr/Cb分量的椭圆范围
#include "..\Public.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
using namespace std;
int main( int argc, char** argv )
{ 
	string ImgPath=".\\母猪截图文件\\";
	if (ImgPath.at(ImgPath.length()-1)!='\\')
	{
		ImgPath+='\\';
	}
	vector<string> filenames=GetAllFileName(ImgPath,"*.jpg",true);
	IplImage *pframe=NULL;//每一帧图像
	CvMat* staticMat= cvCreateMat(256,256,CV_64FC1);
	cvSetZero(staticMat);

	int   step  = staticMat->step/sizeof(double);
	double *pMatdata = staticMat->data.db;
	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_PLAIN,1, 1, 0, 1, CV_AA );
	const int COMPONENT_NUM=2;
	for (int picIndex=0; picIndex<filenames.size(); ++picIndex)
	{
		pframe=cvLoadImage(filenames.at(picIndex).c_str());
		if(!pframe)continue;
		IplImage *components[COMPONENT_NUM];
		for (int i=0; i<COMPONENT_NUM; ++i)
		{
			components[i]=cvCreateImage(cvGetSize(pframe),IPL_DEPTH_8U,1);
		}
		cvCvtColor(pframe,pframe,CV_BGR2YCrCb);
		cvSplit(pframe,0,components[0],components[1],0);
		cvReleaseImage(&pframe);

		for (int i=0; i<components[0]->height; ++i){
			for (int j=0; j<components[0]->width; ++j){
					int cr=((uchar*)components[0]->imageData+ i*components[0]->widthStep)[j];
					int cb=((uchar*)components[1]->imageData+ i*components[1]->widthStep)[j];
					(pMatdata+cr*step)[cb]+=1;
			}	
		}
		for (int i=0; i<COMPONENT_NUM; ++i)
		{
			cvReleaseImage(&components[i]);
		}
	}

	FILE *file;
	if (!(file=fopen(".\\母猪截图文件\\CbCr平面参数.txt","w")))
	{
		return false;
	}
	double dmaxPiex=0;
	for (int i=255; i>=0; --i){//图像坐标轴为左下，所以改为左上		
		for (int j=0; j<=255; ++j){
			double& dTmp=(pMatdata+i*step)[j];
			dTmp=(dTmp<(filenames.size()*0.5)) ? 0 : dTmp;
			dmaxPiex=dTmp > dmaxPiex ? dTmp : dmaxPiex;
			fprintf(file,"%f  ",dTmp);
		}
		fprintf(file,"\r");	
	}
	fclose(file);

	IplImage *grayImg=cvCreateImage(cvSize(256,256),IPL_DEPTH_8U,1);
	cvConvertScale(staticMat,grayImg,255/dmaxPiex,0);
	cvThreshold(grayImg,grayImg,0,255,CV_THRESH_BINARY);
	CvRect minRect=GetMinRect(grayImg);
	cout<<"Cb分量最小值："<<minRect.x<<"  Cb分量最大值："<<minRect.width+minRect.x<<endl
		<<"Cr分量最小值："<<minRect.y<<"  Cr分量最大值："<<minRect.height+minRect.y<<endl;
	if (minRect.width*minRect.height>0)//范围不为空
	{
		IplImage *cutImg=cvCreateImage(cvSize(minRect.width,minRect.height),IPL_DEPTH_8U,1);
		cvSetImageROI(grayImg,minRect);
		cvCopyImage(grayImg,cutImg);
		cvResetImageROI(grayImg);

		IplImage *ResizeImg=cvCreateImage(cvSize(256,256),IPL_DEPTH_8U,1);
		cvResize(cutImg,ResizeImg);
		cvThreshold(ResizeImg,ResizeImg,1,255,CV_THRESH_BINARY);

		IplImage *axesImg=cvCreateImage(cvSize(320,320),IPL_DEPTH_8U,1);
		cvSet(axesImg,cvScalarAll(0));
		cvFlip(ResizeImg,0,0);//沿x轴翻转
		cvSetImageROI(axesImg,cvRect(40,40,256,256));
		cvCopyImage(ResizeImg,axesImg);
		cvResetImageROI(axesImg);

		IplImage *showImg=cvCreateImage(cvGetSize(axesImg),IPL_DEPTH_8U,3);
		CvMemStorage *storage=cvCreateMemStorage(0);
		CvSeq *maxcontour=GetMaxContour(axesImg,storage);
		cvSubRS(axesImg,cvScalarAll(255),axesImg);//反转黑白，现在底色为白
		cvCvtColor(axesImg,showImg,CV_GRAY2BGR);
		DrawMinAreaEllipse(showImg,maxcontour);
		cvReleaseMemStorage(&storage);

		cvSaveImage("水平翻转后的CrCb平面图.jpg",showImg);
		cvShowImage("椭圆拟合图",showImg);
		cvShowImage("放大椭圆拟合图",ResizeImg);
		cvWaitKey(0);
		cvReleaseImage(&cutImg);
		cvReleaseImage(&showImg);
		cvReleaseImage(&ResizeImg);
		cvReleaseImage(&axesImg);
	}
	cvReleaseImage(&grayImg);
	cvReleaseMat(&staticMat);
	cvDestroyAllWindows();
	return 0;
}
#elif MYTYPE==3
#include "..\\Public.h"
#include <sstream>
using namespace std;

//table查找表，rows查找表行数,cols查找表列数,usecols所使用到的列数，不能大于cols
//pframe的大小为(usecols+1)/2，pframe[0]为主分量，pframe[1]...pframe[(usecols+1)/2]为辅分量
//根据查找表检测猪只
void cvLookUpTable(int *table,const int Rows,const int Cols,const int usecols,IplImage** pframe, IplImage* mask){
	assert(Cols>=usecols);
	const int IMGNUM=(usecols-1)/2;
	IplImage **imgs=new IplImage*[IMGNUM];
	IplImage *grayImg=pframe[0];
	IplImage *img=grayImg;
	for (int i=0; i<IMGNUM; ++i)
	{
		imgs[i] =pframe[i+1];
	}
	cvSetZero(mask);
	int *param=new int[usecols-1];
	int *componPiex=new int[IMGNUM];
	for (int i=0; i<img->height; ++i){
		for (int j=0; j<img->width; ++j){
			uchar grayPiex=((unsigned char *)grayImg->imageData+ i* grayImg->widthStep)[j];
			for (int m=0; m<Rows; ++m)
			{
				if (grayPiex==table[m*Cols+0])
				{
					for (int n=0; n<usecols-1; ++n)
					{
						param[n]=table[m*Cols+n+1];
					}
					for (int imgnumIndex=0; imgnumIndex<IMGNUM; ++imgnumIndex){
						componPiex[imgnumIndex]=((unsigned char *)imgs[imgnumIndex]->imageData+ i* imgs[imgnumIndex]->widthStep)[j];
					}
					bool isObjectPiex=true;
					for (int n=0; n<IMGNUM; ++n)
					{
						if (!(componPiex[n]>param[n*2] && componPiex[n] <param[n*2+1])){
							isObjectPiex=false;
							break;
						}
					}
					if (isObjectPiex==true)
					{
						((unsigned char *)mask->imageData+ i* mask->widthStep)[j]=255;
					}
					break;//for (int m=0; m<Rows; ++m)
				}				
			}
		}
	}
	if (imgs){
		delete[] imgs;
		imgs=NULL;
	}
	if (componPiex){
		delete[] componPiex;
		componPiex=NULL;
	}
	if(param) {
		delete []param;
		param=NULL;
	}
}
bool CreateLUT(string LUTPath,int **table,int &row, int &col)
{
	FILE *file=0;
	if (!(file=fopen(LUTPath.c_str(),"r")))
	{
		return false;
	}
	fscanf(file,"%d %d",&row,&col);
	*table=new int[row*col];
	for (int i=0; i<row; ++i){
		for (int j=0; j<col; ++j){		
			float tmp;
			fscanf(file,"%f",&tmp);
			(*table)[i*col+j]=tmp;
		}
	}
	fclose(file);
	return true;
}
void ReleaseLUT(int **table)
{
	if(!table) return;
	if (*table)
	{
		int *tmpTable=*table;
		*table=0;
		delete []tmpTable;
	}
}
//调用如下
int main( int argc, char** argv )
{ 
	string ImgPath=".\\母猪截图文件\\";
	if (ImgPath.at(ImgPath.length()-1)!='\\')
	{
		ImgPath+='\\';
	}

	string LUPName;
	LUPName=ImgPath+"母猪Y_CrCbSMinMaxIndex查找表.dat";
	int row,col;
	int *table=0;
	CvCapture* capture = 0;

	CreateLUT(LUPName,&table,row,col);
	capture = cvCaptureFromAVI(gVRun.c_str());
	if (!capture)
	{
		return -1;
	}
	CvGaussBGStatModelParams params;
	params.win_size      = CV_BGFG_MOG_WINDOW_SIZE*4;
	params.bg_threshold  = CV_BGFG_MOG_BACKGROUND_THRESHOLD;
	params.std_threshold = CV_BGFG_MOG_STD_THRESHOLD;
	params.weight_init   = CV_BGFG_MOG_WEIGHT_INIT;
	params.variance_init = CV_BGFG_MOG_SIGMA_INIT*CV_BGFG_MOG_SIGMA_INIT;
	params.minArea       = CV_BGFG_MOG_MINAREA;
	params.n_gauss       = CV_BGFG_MOG_NGAUSSIANS;


	IplImage *pframe=cvQueryFrame(capture);
	CvGaussBGModel *m_bg_model = (CvGaussBGModel*)cvCreateGaussianBGModel(pframe,&params );

	IplImage*mask=cvCreateImage(cvGetSize(pframe),IPL_DEPTH_8U,1);
	CvMemStorage *storage=cvCreateMemStorage();
	pframe=cvQueryFrame(capture);
	IplImage *tmpImg = cvCreateImage(cvSize(pframe->width, pframe->height), pframe->depth, pframe->nChannels);
	IplImage *imgs[4];
	for(int i=0; i<4; ++i)
	{
		imgs[i]= cvCreateImage(cvSize(pframe->width, pframe->height), pframe->depth, 1);
	}
	while(pframe=cvQueryFrame(capture))
	{
		//cvLightingCompensation(pframe,pframe);
		cvSaveImage("原图像.jpg",pframe);
		cvCvtColor(pframe,tmpImg,CV_BGR2YCrCb);
		cvSplit(tmpImg,imgs[0], imgs[1], imgs[2], 0);
		cvCvtColor(pframe,tmpImg,CV_BGR2HSV);
		cvSplit(tmpImg, 0, imgs[3], 0, 0);	
		cvCvtColor(tmpImg,tmpImg,CV_HSV2BGR);

		cvLookUpTable(table,row,col,7,imgs,mask);
		cvShowImage("原始查找表法",mask);
		cvSaveImage("原始查找表法.jpg",mask);
		cvDilate(mask,mask);
		cvErode(mask,mask);
		cvDilate(mask,mask,0,1);	
		cvClearMemStorage(storage);
		CvSeq *contour=GetMaxContour(mask,storage);
		if (!contour){
			break;
		}
		CvRect maxcontourRc=((CvContour*)contour)->rect;
		DrawMinAreaRect(tmpImg,contour);
		cvShowImage("去噪后查找表法",mask);
		cvSaveImage("去噪后查找表法.jpg",mask);

		cvUpdateBGStatModel( pframe,(CvBGStatModel *)m_bg_model, -1.0 );
		cvShowImage("原始高斯法",m_bg_model->foreground);
		cvSaveImage("原始高斯法.jpg",m_bg_model->foreground);
		cvCopy(m_bg_model->foreground,mask);
		cvErode(mask,mask);
		cvDilate(mask,mask);
		cvShowImage("去噪后的高斯法",mask);
		cvSaveImage("去噪后的高斯法.jpg",mask);

		double dMaxArea=0;
		cvClearMemStorage(storage);
		cvFindContours( mask, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
		for( ; contour != 0; contour = contour->h_next )
		{
			CvRect rc=((CvContour*)contour)->rect;
			if(!CommonArea(rc,maxcontourRc,0.3) && rc.width*rc.height>mask->imageSize*0.008)
			{
				DrawMinAreaRect(tmpImg,contour);
			}
		}

		cvShowImage("原图像-标志结果",tmpImg);
		cvSaveImage("原图像-标志结果.jpg",tmpImg);
		if(cvWaitKey(0)==27) break;
	}
	for(int i=0; i<4; ++i)
	{
		cvReleaseImage(&imgs[i]);
	}	
	cvReleaseBGStatModel((CvBGStatModel**)&m_bg_model);
	cvReleaseImage(&tmpImg);
	cvReleaseImage(&mask);
	cvReleaseMemStorage(&storage);
	cvReleaseCapture(&capture);
	cvDestroyAllWindows();
	ReleaseLUT(&table);
	return 0;
}
#elif MYTYPE==4
#include "E:\硕士毕业论文\SuckingPigletsMonitoring\GaussBG\BackGroundModel.h"
#include "E:\硕士毕业论文\SuckingPigletsMonitoring\MainProgram_GUI\FindPigObject.h"
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

#include <time.h>
#define CV_NO_BACKWARD_COMPATIBILITY


// various tracking parameters (in seconds)
const double MHI_DURATION = 1;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
// number of cyclic frame buffer used for motion detection
// (should, probably, depend on FPS)
const int N = 4;

// ring image buffer
IplImage **buf = 0;
int last = 0;

// temporary images
IplImage *mhi = 0; // MHI
IplImage *orient = 0; // orientation
IplImage *mask = 0; // valid orientation mask
IplImage *segmask = 0; // motion segmentation map
CvMemStorage* storage = 0; // temporary storage

// parameters:
//  img - input video frame
//  dst - resultant motion picture
//  args - optional parameters
void  update_mhi( IplImage* img, IplImage* dst, int diff_threshold )
{
	double timestamp = (double)clock()/CLOCKS_PER_SEC; // get current time in seconds
	CvSize size = cvSize(img->width,img->height); // get current frame size
	int i, idx1 = last, idx2;
	IplImage* silh;
	CvSeq* seq;
	CvRect comp_rect;
	double count;
	double angle;
	CvPoint center;
	double magnitude;
	CvScalar color;

	// allocate images at the beginning or
	// reallocate them if the frame size is changed
	if( !mhi || mhi->width != size.width || mhi->height != size.height ) {
		if( buf == 0 ) {
			buf = (IplImage**)malloc(N*sizeof(buf[0]));
			memset( buf, 0, N*sizeof(buf[0]));
		}

		for( i = 0; i < N; i++ ) {
			cvReleaseImage( &buf[i] );
			buf[i] = cvCreateImage( size, IPL_DEPTH_8U, 1 );
			cvZero( buf[i] );
		}
		cvReleaseImage( &mhi );
		cvReleaseImage( &orient );
		cvReleaseImage( &segmask );
		cvReleaseImage( &mask );

		mhi = cvCreateImage( size, IPL_DEPTH_32F, 1 );
		cvZero( mhi ); // clear MHI at the beginning
		orient = cvCreateImage( size, IPL_DEPTH_32F, 1 );
		segmask = cvCreateImage( size, IPL_DEPTH_32F, 1 );
		mask = cvCreateImage( size, IPL_DEPTH_8U, 1 );
	}

	cvCvtColor( img, buf[last], CV_BGR2GRAY ); // convert frame to grayscale

	idx2 = (last + 1) % N; // index of (last - (N-1))th frame
	last = idx2;

	silh = buf[idx2];
	cvAbsDiff( buf[idx1], buf[idx2], silh ); // get difference between frames

	cvThreshold( silh, silh, diff_threshold, 1, CV_THRESH_BINARY ); // and threshold it
	cvUpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION ); // update MHI

	// convert MHI to blue 8u image
	cvCvtScale( mhi, mask, 255./MHI_DURATION,
		(MHI_DURATION - timestamp)*255./MHI_DURATION );
	cvZero( dst );
	cvMerge( mask, 0, 0, 0, dst );

	// calculate motion gradient orientation and valid orientation mask
	cvCalcMotionGradient( mhi, mask, orient, MAX_TIME_DELTA, MIN_TIME_DELTA, 3 );

	if( !storage )
		storage = cvCreateMemStorage(0);
	else
		cvClearMemStorage(storage);

	// segment motion: get sequence of motion components
	// segmask is marked motion components map. It is not used further
	seq = cvSegmentMotion( mhi, segmask, storage, timestamp, MAX_TIME_DELTA );

	// iterate through the motion components,
	// One more iteration (i == -1) corresponds to the whole image (global motion)
	for( i = -1; i < seq->total; i++ ) {

		if( i < 0 ) { // case of the whole image
			comp_rect = cvRect( 0, 0, size.width, size.height );
			color = CV_RGB(255,255,255);
			magnitude = 100;
		}
		else { // i-th motion component
			comp_rect = ((CvConnectedComp*)cvGetSeqElem( seq, i ))->rect;
			if( comp_rect.width + comp_rect.height < 100 ) // reject very small components
				continue;
			color = CV_RGB(255,0,0);
			magnitude = 30;
		}

		// select component ROI
		cvSetImageROI( silh, comp_rect );
		cvSetImageROI( mhi, comp_rect );
		cvSetImageROI( orient, comp_rect );
		cvSetImageROI( mask, comp_rect );

		// calculate orientation
		angle = cvCalcGlobalOrientation( orient, mask, mhi, timestamp, MHI_DURATION);
		angle = 360.0 - angle;  // adjust for images with top-left origin

		count = cvNorm( silh, 0, CV_L1, 0 ); // calculate number of points within silhouette ROI

		cvResetImageROI( mhi );
		cvResetImageROI( orient );
		cvResetImageROI( mask );
		cvResetImageROI( silh );

		// check for the case of little motion
		if( count < comp_rect.width*comp_rect.height * 0.05 )
			continue;

		// draw a clock with arrow indicating the direction
		center = cvPoint( (comp_rect.x + comp_rect.width/2),
			(comp_rect.y + comp_rect.height/2) );

		cvCircle( dst, center, cvRound(magnitude*1.2), color, 3, CV_AA, 0 );
		cvLine( dst, center, cvPoint( cvRound( center.x + magnitude*cos(angle*CV_PI/180)),
			cvRound( center.y - magnitude*sin(angle*CV_PI/180))), color, 3, CV_AA, 0 );
	}
}


int main(int argc, char** argv)
{
	IplImage* motion = 0;
	CvCapture* capture = 0;


		capture = cvCaptureFromFile("c:\\MOV00225.avi");
	if( capture )
	{
		cvNamedWindow( "Motion", 1 );
		IplImage* image = cvQueryFrame( capture );
		CvVideoWriter *write=cvCreateVideoWriter("C:\\1.avi",CV_FOURCC('X', 'V', 'I', 'D'),25,cvGetSize(image),1);

		for(;;)
		{
			image = cvQueryFrame( capture );
			if( !image )
				break;

			if( !motion )
			{
				motion = cvCreateImage( cvSize(image->width,image->height), 8, 3 );
				cvZero( motion );
				motion->origin = image->origin;
			}

			update_mhi( image, motion, 30 );
			cvShowImage( "Motion", motion );
		cvWriteFrame(write,motion);

			if( cvWaitKey(10) >= 0 )
				break;
		}
		cvReleaseVideoWriter(&write);
		cvReleaseCapture( &capture );
		cvDestroyWindow( "Motion" );
	}

	return 0;
}


#endif // MYTYPE
