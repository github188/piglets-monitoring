// Implement a Kalman filter that allow to smooth and predict 
// a 3d position by estimating velocity.
// http://bilgin.esme.org/BitsBytes/KalmanFilterforDummies/tabid/196/Default.aspx
// http://www.innovatia.com/software/papers/kalman.htm
// http://www.cs.unc.edu/~welch/kalman/maybeck.html

#pragma once
#include "cv.h"
/*======================= KALMAN FILTER AS TRACKER =========================*/
/* state vector is (x,y,w,h,dx,dy,dw,dh)*/
/* mesurment is (x,y,w,h) */
/* dinamic matrix A */

class KalmanBlob
{
public:
	KalmanBlob(void);
	virtual ~KalmanBlob(void);
	KalmanBlob(int dynamParams, int measureParams, int controlParams=0);
	void init(int dynamParams, int measureParams, int controlParams=0);

	const CvMat* predict(const CvMat* control=0);
	const CvMat* correct(const CvMat* measurement);

	CvMat* statePre;           // predicted state (x'(k)):
	//    x(k)=A*x(k-1)+B*u(k)
	CvMat* statePost;          // corrected state (x(k)):
	//    x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
	CvMat* transitionMatrix;   // state transition matrix (A)
	CvMat* controlMatrix;      // control matrix (B)
	//   (it is not used if there is no control)
	CvMat* measurementMatrix;  // measurement matrix (H)
	CvMat* processNoiseCov;    // process noise covariance matrix (Q)
	CvMat* measurementNoiseCov;// measurement noise covariance matrix (R)
	CvMat* errorCovPre;        // priori error estimate covariance matrix (P'(k)):
	//    P'(k)=A*P(k-1)*At + Q)*/
	CvMat* gain;               // Kalman gain matrix (K(k)):
	//    K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)
	CvMat* errorCovPost;       // posteriori error estimate covariance matrix (P(k)):
	//    P(k)=(I-K(k)*H)*P'(k)
	CvMat* temp1;              // temporary matrices
	CvMat* temp2;
	CvMat* temp3;
	CvMat* temp4;
	CvMat* temp5;

private:
	void releaseAll();
}; /* class KalmanBlob */
//计算box的最小包围矩形（范围不超过size）
CvRect box2D2rect(CvBox2D &box,CvSize *psize=0);

//调用方法,刚发现目标调用setNext,以后发现目标就调用getNext
