#include "KalmanBlob.h"
CvRect box2D2rect(CvBox2D &box,CvSize *psize/*=0*/)
{
	CvPoint2D32f point[4];
	cvBoxPoints(box, point);
	CvRect rc;
	rc.x=std::min(point[0].x,std::min(point[1].x,std::min(point[2].x,point[3].x)));
	rc.y=std::min(point[0].y,std::min(point[1].y,std::min(point[2].y,point[3].y)));
	rc.width=std::max(point[0].x,std::max(point[1].x,std::max(point[2].x,point[3].x)));
	rc.height=std::max(point[0].y,std::max(point[1].y,std::max(point[2].y,point[3].y)));
	if(psize){
		rc.x=rc.x<0 ? 0 : rc.x;
		rc.y=rc.y<0 ? 0 : rc.y;
		rc.width=rc.width>psize->width ? psize->width : rc.width;
		rc.height=rc.height>psize->height ? psize->height : rc.height;
	}
	rc.width -=rc.x;
	rc.height-=rc.y;
	return rc;
}

KalmanBlob::KalmanBlob( void )
	:statePre(NULL)
	,statePost(NULL)
	,transitionMatrix(NULL)
	,controlMatrix(NULL)
	,measurementMatrix(NULL)
	,processNoiseCov(NULL)
	,measurementNoiseCov(NULL)
	,errorCovPre(NULL)
	,gain(NULL)
	,errorCovPost(NULL)
	,temp1(NULL)
	,temp2(NULL)
	,temp3(NULL)
	,temp4(NULL)
	,temp5(NULL)
{

}

KalmanBlob::KalmanBlob( int dynamParams, int measureParams, int controlParams/*=0*/ )
	:statePre(NULL)
	,statePost(NULL)
	,transitionMatrix(NULL)
	,controlMatrix(NULL)
	,measurementMatrix(NULL)
	,processNoiseCov(NULL)
	,measurementNoiseCov(NULL)
	,errorCovPre(NULL)
	,gain(NULL)
	,errorCovPost(NULL)
	,temp1(NULL)
	,temp2(NULL)
	,temp3(NULL)
	,temp4(NULL)
	,temp5(NULL)
{
	init(dynamParams, measureParams, controlParams);
}

KalmanBlob::~KalmanBlob( void )
{
	releaseAll();
}

void KalmanBlob::init(int DP, int MP, int CP)
{
	assert( DP > 0 && MP > 0 );
	CP = std::max(CP, 0);

	releaseAll();
	statePre=cvCreateMat(DP,1,CV_32FC1);
	cvZero(statePre);
	statePost=cvCreateMat(DP,1,CV_32FC1);
	cvZero(statePost);
	transitionMatrix=cvCreateMat(DP,DP,CV_32FC1);
	cvSetIdentity(transitionMatrix,cvScalarAll(1));
	processNoiseCov=cvCreateMat(DP,DP,CV_32FC1);
	cvSetIdentity(processNoiseCov, cvRealScalar(1e-5) );/*过程噪声*/
	measurementMatrix=cvCreateMat(MP,DP,CV_32FC1);
	cvSetIdentity(measurementMatrix, cvRealScalar(1) );/*观测矩阵*/
	measurementNoiseCov=cvCreateMat(MP,MP,CV_32FC1);
	cvSetIdentity(measurementNoiseCov, cvRealScalar(1e-1) );/*观测噪声*/

	errorCovPre=cvCreateMat(DP,DP,CV_32FC1);
	cvZero(errorCovPre);
	errorCovPost=cvCreateMat(DP,DP,CV_32FC1);
	cvSetIdentity(errorCovPost, cvRealScalar(1) );/*后验误差协方差*/
	gain=cvCreateMat(DP,MP,CV_32FC1);
	cvZero(gain);

	if( CP > 0 ){
		controlMatrix=cvCreateMat(DP,CP,CV_32FC1);
		cvZero(controlMatrix);
	}
	temp1=cvCreateMat(DP,DP,CV_32FC1);
	temp2=cvCreateMat(MP,DP,CV_32FC1);
	temp3=cvCreateMat(MP,MP,CV_32FC1);
	temp4=cvCreateMat(MP,DP,CV_32FC1);
	temp5=cvCreateMat(MP,1,CV_32FC1);
}

const CvMat* KalmanBlob::predict( const CvMat* control/*=0*/ )
{
	// update the state: x'(k) = A*x(k)
	cvMatMulAdd( transitionMatrix, statePost, 0, statePre );

	if( control && controlMatrix)
		/* x'(k) = x'(k) + B*u(k) */
		cvMatMulAdd(controlMatrix, control, statePre, statePre);

	// update error covariance matrices: temp1 = A*P(k)
	cvMatMulAdd( transitionMatrix, errorCovPost, 0,temp1 );

	// P'(k) = temp1*At + Q
	cvGEMM( temp1, transitionMatrix, 1, processNoiseCov, 1,errorCovPre, CV_GEMM_B_T );

	return statePre;

}

const CvMat* KalmanBlob::correct( const CvMat* measurement )
{
	// temp2 = H*P'(k)
	cvMatMulAdd(measurementMatrix, errorCovPre, 0, temp2 );

	// temp3 = temp2*Ht + R
	cvGEMM(temp2, measurementMatrix, 1,measurementNoiseCov, 1, temp3, CV_GEMM_B_T );

	// temp4 = inv(temp3)*temp2 = Kt(k)
	cvSolve( temp3,temp2, temp4, CV_SVD );

	// K(k)
	cvTranspose( temp4,gain );

	// temp5 = z(k) - H*x'(k)
	cvGEMM( measurementMatrix, statePre, -1, measurement, 1, temp5 );

	// x(k) = x'(k) + K(k)*temp5
	cvMatMulAdd( gain,temp5, statePre, statePost);

	// P(k) = P'(k) - K(k)*temp2
	cvGEMM( gain,temp2, -1,errorCovPre, 1,errorCovPost, 0 );

	return statePost;

}

void KalmanBlob::releaseAll()
{
	if(statePre)cvReleaseMat(&statePre);
	if(statePost)cvReleaseMat(&statePost);
	if(transitionMatrix)cvReleaseMat(&transitionMatrix);
	if(processNoiseCov)cvReleaseMat(&processNoiseCov);
	if(measurementMatrix)cvReleaseMat(&measurementMatrix);
	if(measurementNoiseCov)cvReleaseMat(&measurementNoiseCov);
	if(controlMatrix)cvReleaseMat(&controlMatrix);
	if(errorCovPre)cvReleaseMat(&errorCovPre);
	if(errorCovPost)cvReleaseMat(&errorCovPost);
	if(gain)cvReleaseMat(&gain);
	if(temp1)cvReleaseMat(&temp1);
	if(temp2)cvReleaseMat(&temp2);
	if(temp3)cvReleaseMat(&temp3);
	if(temp4)cvReleaseMat(&temp4);
	if(temp5)cvReleaseMat(&temp5);
}
