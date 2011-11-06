#include "FindPigObject.h"
//找出图像surMark中的最大轮廓
CvSeq* GetMaxContour(IplImage *surMark, CvMemStorage* storage)
{
	if (!CV_IS_IMAGE(surMark) || surMark->nChannels!=1 || surMark->depth !=IPL_DEPTH_8U)
	{
		return NULL;
	}
	CvSeq* contour = 0;
	CvSeq* maxcontour=0;
	double dMaxArea=0;
	IplImage *tmpImg=cvCloneImage(surMark);//cvFindContours会改变图像的内容
	cvFindContours( tmpImg, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	cvReleaseImage(&tmpImg);
	for( ; contour != 0; contour = contour->h_next )
	{
		CvRect rc=((CvContour*)contour)->rect;
		if (rc.width*rc.height>dMaxArea)
		{
			dMaxArea=rc.width*rc.height;
			maxcontour=contour;
		}
	}
	return maxcontour;
}

//对比矩形共有面积是否在阈值范围内
bool CommonArea(CvRect rectT1,CvRect rectT2,float threshold)
{
	if (rectT1.x>rectT2.x+rectT2.width||rectT2.x>rectT1.x+rectT1.width)
	{
		return false;
	}
	if (rectT1.y>rectT2.y+rectT2.height||rectT2.y>rectT1.y+rectT1.height)
	{
		return false;
	}
	int maxLeftTopX,maxLeftTopY,minRightButtonX,minRightButtonY;
	float commonArea,rectT1Area,rectT2Area;
	float rate1,rate2;
	maxLeftTopX=std::max(rectT1.x,rectT2.x);
	maxLeftTopY=std::max(rectT1.y,rectT2.y);
	minRightButtonX=std::min(rectT1.x+rectT1.width,rectT2.x+rectT2.width);
	minRightButtonY=std::min(rectT1.y+rectT1.height,rectT2.y+rectT2.height);

	commonArea=(float)(minRightButtonX-maxLeftTopX)*(minRightButtonY-maxLeftTopY);
	rectT1Area=(float)rectT1.height*rectT1.width;
	rectT2Area=(float)rectT2.height*rectT2.width;
	rate1=commonArea/rectT1Area;
	rate2=commonArea/rectT2Area;
	return (rate1>threshold || rate2>threshold);
}

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

//由于高斯背景建模没返回背景，所以使用平均背景法自动生成背景
bool GetAvgBackgroudImg(const IplImage *pFrame, IplImage *bgImg,double alpha)
{  
	static bool isFirst=true;
	static IplImage *pAccImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_32F,3);
	if(isFirst){
		cvSetZero(pAccImg);
		isFirst=false;
	}
	if (!pFrame || !bgImg || !pAccImg ||
		pAccImg->width != pFrame->width || pAccImg->width != bgImg->width ||
		pAccImg->height != pFrame->height || pAccImg->height != bgImg->height|| 
		pFrame->nChannels!=3 || pFrame->nChannels!=3){
		return false;
	}
	cvRunningAvg(pFrame,pAccImg,alpha);
	cvConvertScale(pAccImg,bgImg,1.,0.);
	return true;
}

////Improving shadow suppression in moving object detection with HSV color information里阴影检测的算法
//TS，TH分别表示H分量、S分量的阈值，可通过实验来确定。因数阴影点的V分量值通常小于非阴影点相应的V分量值，
//所以β取值应该小于1，而α的取值则需要考虑场景光线的强弱，光线越强，α取值越小
bool ShadowDetect(const IplImage *srcImg,const IplImage *bgImg,const IplImage *mask,
	const double Alpha, const double Beta, const double TS, const double TH,IplImage *dstImg)
{
	if (!srcImg || !bgImg || !dstImg || (mask && mask==dstImg)
		|| srcImg->width != bgImg->width ||  (mask && srcImg->width !=mask->width) || srcImg->width != dstImg->width
		|| srcImg->height!=bgImg->height || (mask && srcImg->height!=mask->height) || srcImg->height!=dstImg->height
		|| srcImg->nChannels!=3 || bgImg->nChannels!=3 || (mask && mask->nChannels!=1) || dstImg->nChannels!=1)
	{
		return false;
	}
	cvSetZero(dstImg);
	IplImage *channelsImg=cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,3);
	IplImage *IhImg=cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,1);
	IplImage *IsImg=cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,1);
	IplImage *IvImg=cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,1);
	IplImage *BhImg=cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,1);
	IplImage *BsImg=cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,1);
	IplImage *BvImg=cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,1);
	cvCvtColor(srcImg,channelsImg,CV_RGB2HSV);
	cvSplit(channelsImg,IhImg,IsImg,IvImg,NULL);
	cvCvtColor(bgImg,channelsImg,CV_RGB2HSV);
	cvSplit(channelsImg,BhImg,BsImg,BvImg,NULL);
	uchar Ih,Is,Iv,Bh,Bs,Bv;
	for (int i=0; i<srcImg->height; ++i){
		for (int j=0; j<srcImg->width; ++j){
			Ih=((uchar*)IhImg->imageData + i *IhImg->widthStep)[j];
			Is=((uchar*)IsImg->imageData + i *IsImg->widthStep)[j];
			Iv=((uchar*)IvImg->imageData + i *IvImg->widthStep)[j];
			Bh=((uchar*)BhImg->imageData + i *BhImg->widthStep)[j];
			Bs=((uchar*)BsImg->imageData + i *BsImg->widthStep)[j];
			Bv=((uchar*)BvImg->imageData + i *BvImg->widthStep)[j];
			if (mask){
				uchar maskPiex=((uchar*)mask->imageData + i *mask->widthStep)[j];
				if (maskPiex){
					if (Iv*1./Bv>Alpha && Iv*1./Bv<Beta
						&& abs(Is-Bs)<=TS && abs(Ih-Bh)<=TH){
							((uchar*)dstImg->imageData + i *dstImg->widthStep)[j]=255;
					}
				}
			}else{
				if (Iv*1./Bv>Alpha && Iv*1./Bv<Beta
					&& abs(Is-Bs)<=TS && abs(Ih-Bh)<=TH){
						((uchar*)dstImg->imageData + i *dstImg->widthStep)[j]=255;
				}
			}
		}
	}
	cvReleaseImage(&channelsImg);
	cvReleaseImage(&IhImg);
	cvReleaseImage(&IsImg);
	cvReleaseImage(&IvImg);
	cvReleaseImage(&BhImg);
	cvReleaseImage(&BsImg);
	cvReleaseImage(&BvImg);
	return true;
}

bool FindPigBlobs(IplImage *srcImg, IplImage *maskImg,void* userdata)
{
	assert(CV_IS_IMAGE(srcImg) && CV_IS_IMAGE(maskImg) && srcImg->nChannels==3 && maskImg->nChannels==1 
		&& srcImg->width==maskImg->width && srcImg->height==maskImg->height && userdata);

	findpigblobs_userdata* param = (findpigblobs_userdata*)userdata;
	CvGaussBGModel *bg_model=param->bg_model;
	int *table=param->LUPTable;
	int rows=param->rows;
	int cols=param->cols;
	CvMemStorage *storage=param->storage;
	int& nModelFrames=param->modelframes;
	bool isRemoveShadow=param->isRemoveShadow;
	nModelFrames=nModelFrames< 0 ? 0 : nModelFrames;
	IplImage*mask=cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,1);
	IplImage *tmpImg = cvCreateImage(cvSize(srcImg->width, srcImg->height), srcImg->depth, srcImg->nChannels);
	IplImage *imgs[4];
	for(int i=0; i<4; ++i)
	{
		imgs[i]= cvCreateImage(cvSize(srcImg->width, srcImg->height), srcImg->depth, 1);
	}
	cvZero(maskImg);
	cvCvtColor(srcImg,tmpImg,CV_BGR2YCrCb);
	cvSplit(tmpImg,imgs[0], imgs[1], imgs[2], 0);
	cvCvtColor(srcImg,tmpImg,CV_BGR2HSV);
	cvSplit(tmpImg, 0, imgs[3], 0, 0);

	cvLookUpTable(table,rows,cols,5,imgs,mask);
	cvErode(mask,mask,NULL,1);
	cvDilate(mask,mask,0,4);
	cvClearMemStorage(storage);
	CvSeq *contour=GetMaxContour(mask,storage);
	if (!contour){
		return false;
	}
	cvDrawContours(maskImg,contour,cvScalarAll(255),cvScalarAll(255),-1,CV_FILLED, 8 );

	CvRect maxcontourRc=((CvContour*)contour)->rect;
	cvUpdateBGStatModel( srcImg,(CvBGStatModel *)bg_model, -1.0 );
	if (isRemoveShadow){
		bool b=GetAvgBackgroudImg(srcImg,tmpImg,0.1);//平均背景法生成背景,tmpImg为背景
		assert(b);
	}

	if (nModelFrames>20){//20帧之后才产生背景
		nModelFrames=21;
		if (isRemoveShadow){
			bool b=ShadowDetect(srcImg,tmpImg,bg_model->foreground,0.5,1,20,20,mask);//检测阴影
			assert(b);
			cvSub(bg_model->foreground,mask,mask);//得到去除阴影后的二值图
		}else{
			cvCopy(bg_model->foreground,mask);
		}
		cvErode(mask,mask);
		cvDilate(mask,mask,0,5);
		cvClearMemStorage(storage);
		cvFindContours( mask, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
		for( ; contour != 0; contour = contour->h_next ){
			CvRect rc=((CvContour*)contour)->rect;
			if(!CommonArea(rc,maxcontourRc,0.3) && rc.width*rc.height>mask->imageSize*0.008
				&& rc.x+rc.width>40 && rc.y+rc.height>40)//删除位于保温室的误检测
			{
				cvDrawContours(maskImg,contour,cvScalarAll(255),cvScalarAll(255),-1,CV_FILLED, 8 );
			}
		}
	}else{
		nModelFrames++;
	}

	cvReleaseImage(&mask);
	cvReleaseImage(&tmpImg);
	for(int i=0; i<4; ++i)
	{
		cvReleaseImage(&imgs[i]);
	}
	return true; 
}
