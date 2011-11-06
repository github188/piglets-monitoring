///////////////////////卡尔曼滤波器跟踪猪只
#include "..\Public.h"
#include "..\MainProgram\KalmanBlob.h"
#include <iostream>
using namespace std;

const int stateNum=8;
const int measureNum=stateNum/2;
const float T=0.5;
float A10[10][10] ={//transition matrix(x,y,width,height,angle,dx,dy,dw,dh,da)
	1,0,0,0,0,T,0,0,0,0,
	0,1,0,0,0,0,T,0,0,0,
	0,0,1,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,T, 
	0,0,0,0,0,1,0,0,0,0, 
	0,0,0,0,0,0,1,0,0,0, 
	0,0,0,0,0,0,0,1,0,0, 
	0,0,0,0,0,0,0,0,1,0, 
	0,0,0,0,0,0,0,0,0,1, 
};
float A8[8][8] ={//transition matrix(x,y,width,height,dx,dy,dw,dh)
	1,0,0,0,T,0,0,0,
	0,1,0,0,0,T,0,0,
	0,0,1,0,0,0,0,0,
	0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0, 
	0,0,0,0,0,1,0,0, 
	0,0,0,0,0,0,1,0, 
	0,0,0,0,0,0,0,1,
};

int main (void)
{
	// Initialize Kalman filter object, window, number generator, etc
	cvNamedWindow( "Kalman", 1 );//创建窗口，当为的时候，表示窗口大小自动设定
	IplImage *img=cvCreateImage(cvSize(500,500),IPL_DEPTH_8U,3);
	if(stateNum==10)
	{
		KalmanBlob kalman(10,5,0);
		float blob[5]={5,5,10,20,20};
		CvMat blobMat=cvMat(measureNum,1,CV_32FC1,blob);
		int count=0;
		//for (int i=0; i<measureNum; ++i)
		//{
		//	kalman.statePost->data.fl[i]=blob[i];
		//	kalman.statePost->data.fl[i+measureNum]=0;
		//}
		memcpy( kalman.transitionMatrix->data.fl,A10, sizeof(A10));
		while( 1 ){
			count++;
			cvZero(img);
			const CvMat*predict=kalman.predict();//预测结果
			CvBox2D box;
			box.center.x=predict->data.fl[0];
			box.center.y=predict->data.fl[1];
			box.size.width=predict->data.fl[2];
			box.size.height=predict->data.fl[3];
			box.angle=predict->data.fl[4];
			CvPoint2D32f box_vtx[4];
			CvPoint pt0, pt;
			cvBoxPoints( box, box_vtx );
			pt0.x = cvRound(box_vtx[3].x);
			pt0.y = cvRound(box_vtx[3].y);
			for(int i = 0; i < 4; i++ )
			{
				pt.x = cvRound(box_vtx[i].x);
				pt.y = cvRound(box_vtx[i].y);
				cvLine(img, pt0, pt,CV_RGB(255,0,0), 1, CV_AA, 0);
				pt0 = pt;
			}
			
			blob[0]+=1;
			blob[1]+=1;
			if (count%4==0){
				blob[2]+=2;
				blob[3]+=2;
				blob[4]+=20;

			}
			if(count%8==0){
				blob[2]-=4;
				blob[3]-=4;
				blob[4]-=40;
			}
			box.center.x=blob[0];
			box.center.y=blob[1];
			box.size.width=blob[2];
			box.size.height=blob[3];
			box.angle=blob[4];
			cvBoxPoints( box, box_vtx );
			pt0.x = cvRound(box_vtx[3].x);
			pt0.y = cvRound(box_vtx[3].y);
			for(int i = 0; i < 4; i++ )
			{
				pt.x = cvRound(box_vtx[i].x);
				pt.y = cvRound(box_vtx[i].y);
				cvLine(img, pt0, pt,CV_RGB(0,255,0), 1, CV_AA, 0);
				pt0 = pt;
			}
			predict=kalman.correct(&blobMat);//加上测量结果得到最优估算
			cvShowImage( "Kalman", img );
			if(cvWaitKey(100) == 27) 
				break;
		}
	}else
	{
		KalmanBlob kalman(8,4,0);
		float blob[4]={5,5,10,10};
		CvMat blobMat=cvMat(4,1,CV_32FC1,blob);
		int count=0;
		//for (int i=0; i<4; ++i)
		//{
		//	kalman.statePost->data.fl[i]=blob[i];
		//	kalman.statePost->data.fl[i+4]=0;
		//}
		memcpy( kalman.transitionMatrix->data.fl,A8, sizeof(A8));
		// Initialize other Kalman parameters
		cvSetIdentity( kalman.measurementMatrix, cvRealScalar(1) );/*观测矩阵*/
		cvSetIdentity( kalman.processNoiseCov, cvRealScalar(1e-5) );/*过程噪声*/
		cvSetIdentity( kalman.measurementNoiseCov, cvRealScalar(1e-1) );/*观测噪声*/
		cvSetIdentity( kalman.errorCovPost, cvRealScalar(1) );/*后验误差协方差*/
		while( 1 ){
			count++;
			cvZero(img);
			const CvMat*predict=kalman.predict();//预测结果
			CvRect rc;
			rc.x=predict->data.fl[0];
			rc.y=predict->data.fl[1];
			rc.width=predict->data.fl[2];
			rc.height=predict->data.fl[3];
			cvRectangleR(img,rc,CV_RGB(255,0,0));
			blob[0]+=1;
			blob[1]+=1;
			if (count%4==0){
				blob[2]+=2;
				blob[3]+=2;
				blob[0]+=5;

			}
			if(count%8==0){
				blob[2]-=4;
				blob[3]-=4;
				blob[0]-=10;
			}
			rc.x=blob[0];
			rc.y=blob[1];
			rc.width=blob[2];
			rc.height=blob[3];
			cvRectangleR(img,rc,CV_RGB(0,255,0));
			predict=kalman.correct(&blobMat);//加上测量结果得到最优估算
			cvShowImage( "Kalman", img );
			if(cvWaitKey(100) == 27) 
				break;
		}
	}
	cvReleaseImage(&img);/*释放图像*/
	cvDestroyAllWindows();/*释放所有窗口*/
	return 0;
}
