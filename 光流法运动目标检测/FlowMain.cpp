#include "..\Public.h"
//************************************
// Method:    GetOptimalClustersNum
// FullName:  GetOptimalClustersNum
// Access:    public 
// Returns:   int--最佳聚类数
// Qualifier:
// Parameter: const CvPoint * point--所要求取聚类的点集
// Parameter: const int samplecount--点集的数量
//************************************
int GetOptimalClustersNum(CvMat *sampleMat,CvMat *clusterMat)//const CvPoint *curpoint,const CvPoint *prepoint,const int samplecount)
{
	double compactness=0;
	double bestcompactness=0;
	int bestcluster=-1;//最佳聚类数
	int samplecount=sampleMat->rows;
	for (int clustercount=1; clustercount<=samplecount; ++clustercount )//寻找最好的聚类数bestcluster
	{
		//cout<<"end "<<clustercount<<" "<<compactness<<endl;
		cvKMeans2( sampleMat, clustercount, clusterMat,
			cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0 ),
			5, 0, 0, 0, &compactness );
		//cout<<"begin "<<clustercount<<" "<<compactness<<endl;
		if (clustercount==1)
		{
			bestcompactness=compactness*clustercount*5;
		}
		if (compactness*clustercount*5<bestcompactness)
		{
			bestcompactness=compactness*clustercount*5;
		}else if (clustercount!=1)
		{
			bestcluster=clustercount;
			break;
		}
	}
	bestcluster= (bestcluster==-1 ? samplecount : bestcluster-1);
	return bestcluster;
}
//************************************
// Method:    GetClusterInfo
// FullName:  GetClusterInfo
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const CvPoint * point--所要求取聚类的点集
// Parameter: const int samplecount--point中包含的样本点数量
// Parameter: const int bestcluster--最佳聚类数目
// Parameter: int * label--point中每个样本对应的类别标识(长度为samplecount) 
// Parameter: CvPoint * centers--每个类别的聚类中心(长度为bestcluster)
// Parameter: CvBox2D * box--每个类别的最小外接矩形(长度为bestcluster)
//************************************
void GetClusterInfo(CvMat *sampleMat,CvMat *clusterMat,const int bestcluster,int *label,CvPoint *centers,CvBox2D *box)
{
	int nCols=sampleMat->cols;
	CvMat* centerMat=cvCreateMat(bestcluster,nCols,CV_32FC1);
	cvKMeans2( sampleMat, bestcluster, clusterMat,
		cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0 ),
		10, 0, 0, centerMat, 0);
	////////得到聚类中心点
	CvPoint ipt;
	for(int i = 0; i < bestcluster; ++i )
	{
		ipt.x = (int)centerMat->data.fl[i*nCols  ];
		ipt.y = (int)centerMat->data.fl[i*nCols+1];
		centers[i]=ipt;
	}
	////////////得到各聚类的最小外接矩形
	CvMemStorage** storage =(CvMemStorage**)cvAlloc(bestcluster*sizeof(CvMemStorage*));
	CvSeq** ptseq =(CvSeq**)cvAlloc(bestcluster*sizeof(CvSeq*));
	for(int i=0; i<bestcluster; ++i)
	{
		storage[i]=cvCreateMemStorage(0);
		ptseq[i]=cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),sizeof(CvPoint), storage[i]);
	}
	int samplecount=sampleMat->rows;
	for(int i = 0; i < samplecount; ++i )
	{
		label[i] = clusterMat->data.i[i];//得到类别标识
		ipt.x = (int)sampleMat->data.fl[i*nCols  ];
		ipt.y = (int)sampleMat->data.fl[i*nCols+1];
		cvSeqPush(ptseq[label[i]],&ipt);
	}
	for( int i = 0; i <bestcluster; ++i )
	{
		if (ptseq[i]->total==0)//序列中无元素
		{
			box[i].size.height=0;
			box[i].size.width=0;
			box[i].center.x=0;
			box[i].center.y=0;
			box[i].angle=0;
		}else
		{
			box[i]=cvMinAreaRect2(ptseq[i],0);//得到最小面积的包围矩形 
		}	
	}
	//释放内存空间
	for(int i=0; i<bestcluster; ++i)
	{
		if(storage[i])
		{
			cvReleaseMemStorage(&storage[i]);
		}
	}
	cvFree(&storage);
	cvFree(&ptseq);
	cvReleaseMat(&centerMat);
}
void DrawImg(IplImage *img,const int bestcluster,const int *label,const CvPoint *centers,const CvBox2D *box)
{
	if (!img || !label || !centers || !box)
	{
		return;
	}
	for(int i = 0; i <bestcluster; i++ )
	{
		cvCircle( img, centers[i], 5, CV_RGB(0,0,255), CV_FILLED, CV_AA, 0 );//画聚类中心点
		CvPoint2D32f box_vtx[4];
		cvBoxPoints( box[i], box_vtx );
		CvPoint pt0,pt;
		pt0.x = box_vtx[3].x;
		pt0.y = box_vtx[3].y;
		for(int j = 0; j < 4; j++ )
		{
			pt.x = box_vtx[j].x;
			pt.y = box_vtx[j].y;
			cvLine(img, pt0, pt, CV_RGB(255, 0, 0), 1, CV_AA, 0);//画外接矩形
			pt0 = pt;
		}
	}
}
int main( int argc, char** argv )
{
	IplImage *image = 0, *grey = 0, *prev_grey = 0, *pyramid = 0, *prev_pyramid = 0, *swap_temp;
	int win_size = 10;
	const int MAX_COUNT = 500;
	CvPoint2D32f* points[2] = {0,0}, *swap_points;
	char* status = 0;
	float*errors=0;
	int count = 0;
	int flags = 0;
	CvCapture* capture = 0;

	int sample_count=0;//样本点数
	vector<CvPoint2D32f> tmpCurpointvec;
	vector<CvPoint2D32f> tmpPrepointvec;
	capture = cvCaptureFromAVI("c:\\MOV00225.avi");
	//capture=cvCaptureFromCAM(1);
	if( !capture )
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		return -1;
	}
	bool binit=true;
	for(;;)
	{
		IplImage* frame = 0;
		int i=0, c;
		frame = cvQueryFrame( capture );
		frame = cvQueryFrame( capture );
		frame = cvQueryFrame( capture );
		if( !frame )
			break;
		if( !image )
		{
			/* allocate all the buffers */
			image = cvCreateImage( cvGetSize(frame),IPL_DEPTH_8U, 3 );
			image->origin = frame->origin;
			grey = cvCreateImage( cvGetSize(frame),IPL_DEPTH_8U, 1 );
			prev_grey = cvCreateImage( cvGetSize(frame),IPL_DEPTH_8U, 1 );
			pyramid = cvCreateImage(cvSize(frame->width+8,frame->height/3),IPL_DEPTH_8U, 1 );
			prev_pyramid = cvCreateImage(cvSize(frame->width+8,frame->height/3),IPL_DEPTH_8U, 1 );
			points[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
			points[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
			status = (char*)cvAlloc(MAX_COUNT);
			errors=(float*)cvAlloc(MAX_COUNT*sizeof(float));
			flags = 0;
		}
		else
		{
			cvSaveImage("原图像.jpg",frame);
			cvCopy( frame, image, 0 );
			cvCvtColor( image, grey, CV_BGR2GRAY );
			cvCalcOpticalFlowPyrLK( prev_grey, grey, prev_pyramid, pyramid,
				points[0], points[1], count, cvSize(win_size,win_size), 3, status, errors,
				cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags );
			flags |= CV_LKFLOW_PYR_A_READY;
			tmpCurpointvec.clear();
			tmpPrepointvec.clear();
			for( i = 0; i < count; i++ )
			{
				if( !status[i])// || errors[i]>500)
					continue;
				CvPoint2D32f p0 = points[0][i];
				CvPoint2D32f p1 = points[1][i];
				if((p0.x-p1.x)*(p0.x-p1.x)+(p0.y-p1.y)*(p0.y-p1.y)<20 || (p0.x-p1.x)*(p0.x-p1.x)+(p0.y-p1.y)*(p0.y-p1.y)>1000
					) continue;
				tmpCurpointvec.push_back(p1);
				tmpPrepointvec.push_back(p0);
				cvLine( image, cvPointFrom32f(p0), cvPointFrom32f(p1), CV_RGB(255,0,0),1 );
				cvCircle( image, cvPointFrom32f(p1), 2, CV_RGB(255,0,0), -1, 8,0);
			}
			cvShowImage("光流图",image);
			cvSaveImage("光流图.jpg",image);
			sample_count=tmpCurpointvec.size();
			if (sample_count>2)
			{
				int nCols=5;
				CvMat* pointMat = cvCreateMat( sample_count, nCols, CV_32FC1 );
				CvMat* clustersMat = cvCreateMat( sample_count, 1, CV_32SC1 );
				for(int i = 0; i < sample_count; ++i )
				{
					pointMat->data.fl[i*nCols  ]=tmpCurpointvec.at(i).x;
					pointMat->data.fl[i*nCols+1]=tmpCurpointvec.at(i).y;
					pointMat->data.fl[i*nCols+2]=tmpPrepointvec.at(i).x;
					pointMat->data.fl[i*nCols+3]=tmpPrepointvec.at(i).y;
					float tmpfloat=(tmpCurpointvec.at(i).y-tmpPrepointvec.at(i).y)==0 ? 0 : (tmpCurpointvec.at(i).x-tmpPrepointvec.at(i).x)/(tmpCurpointvec.at(i).y-tmpPrepointvec.at(i).y);
					pointMat->data.fl[i*nCols+4]=atan(tmpfloat);
				}
				int bestcluster=GetOptimalClustersNum(pointMat,clustersMat);
				int *label=(int*)cvAlloc(sample_count*sizeof(int));
				CvPoint *centerPoint=(CvPoint*)cvAlloc(bestcluster*sizeof(CvPoint));
				CvBox2D *box=(CvBox2D*)cvAlloc(bestcluster*sizeof(CvBox2D));
				GetClusterInfo(pointMat,clustersMat,bestcluster,label,centerPoint,box);
				DrawImg(image,bestcluster,label,centerPoint,box);
				cvReleaseMat(&pointMat);
				cvReleaseMat(&clustersMat);
				cvFree(&label);
				cvFree(&centerPoint);
				cvFree(&box);		
			}
		}
		IplImage *tmpImg=cvCloneImage(frame);
		IplImage* eig = cvCreateImage( cvGetSize(grey), IPL_DEPTH_32F, 1 );
		IplImage* temp = cvCreateImage( cvGetSize(grey), IPL_DEPTH_32F, 1 );
		double quality = 0.01;
		double min_distance = 10;

		count = MAX_COUNT;
		cvGoodFeaturesToTrack( grey, eig, temp, points[1], &count,
			quality, min_distance, 0, 3, 0, 0.04 );
		cvFindCornerSubPix( grey, points[1], count,
			cvSize(win_size,win_size), cvSize(-1,-1),
			cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
		for (int i=0; i<count; i++)
		{
			cvCircle( tmpImg, cvPointFrom32f(points[1][i]), 3, CV_RGB(0,255,0), -1, 8,0);
		}
		cvShowImage("角点",tmpImg);
		cvSaveImage("角点.jpg",tmpImg);
		cvReleaseImage(&tmpImg);
		cvReleaseImage( &eig );
		cvReleaseImage( &temp );

		CV_SWAP( prev_grey, grey, swap_temp );
		CV_SWAP( prev_pyramid, pyramid, swap_temp );
		CV_SWAP( points[0], points[1], swap_points );
		cvShowImage("光流聚类图", image );
		cvSaveImage("光流聚类图.jpg", image );

		c = cvWaitKey(10);
		if( (char)c == 27 )
			break;
	}

	cvReleaseCapture( &capture );
	cvDestroyAllWindows();
	cvReleaseImage(&image);
	cvReleaseImage(&grey);
	cvReleaseImage(&prev_grey);
	cvReleaseImage(&pyramid);
	cvReleaseImage(&prev_pyramid);
	cvFree(&points[0]);
	cvFree(&points[1]);
	cvFree(&status);
	cvFree(&errors);
	return 0;
}
