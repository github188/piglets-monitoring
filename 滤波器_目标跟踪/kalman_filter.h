#pragma once
#ifndef KALMAN_FILTER
#define KALMAN_FILTER

#include "highgui.h"
#include "cv.h"
#include "cxcore.h"
#include <stdio.h>
#include <math.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;


#ifdef D_BOOK_EXAMPLE

#define phi2xy(mat,img) \
	cvPoint( cvRound(img->width/2 + img->width/3*cos(mat->data.fl[0])), \
			cvRound( img->height/2 - img->width/3*sin(mat->data.fl[0])) )


#define STATE_DIM 2
#define MEASURE_DIM 1

#else

#define phi2xy(mat,img) \
	cvPoint( cvRound(img->width/2 + img->width/3*cos(mat->data.fl[0])), \
			cvRound( img->height/2 - img->width/3*cos(mat->data.fl[1])) )

	//State: x, y, vx, vy
	//Measurement: x, y
	//Control: NONE	
#define STATE_DIM 10
#define MEASURE_DIM 5

#endif

const float T=0.5;
float A[STATE_DIM][STATE_DIM] ={//transition matrix
	1,0,T,0,0,0,0,0,0,0,
	0,1,0,T,0,0,0,0,0,0,
	0,0,1,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,0,0,0,
	0,0,0,0,1,0,T,0,0,0, 
	0,0,0,0,0,1,0,T,0,0, 
	0,0,0,0,0,0,1,0,0,0, 
	0,0,0,0,0,0,0,1,0,0, 
	0,0,0,0,0,0,0,0,1,T, 
	0,0,0,0,0,0,0,0,0,1, 
};
float H[MEASURE_DIM][STATE_DIM] ={//transition matrix
	1,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0, 
	0,0,0,0,0,1,0,0,0,0, 
	0,0,0,0,0,0,0,0,1,0, 
};
//Real state, Process Noise
CvMat *X_k = NULL;
CvMat* W_k = NULL;
CvRandState Rng;

//Must call to use Kalman filter
CvKalman* init_kalman_filter(float frame_rate) {
	// Initialize, create Kalman Filter object, window, random number
	// generator etc.
	
	cvRandInit( &Rng, 0, 1, -1, CV_RAND_UNI );
	//imgKMTest = cvCreateImage( cvSize(300,300), 8, 3 );

	CvKalman* Kalman = cvCreateKalman( STATE_DIM, MEASURE_DIM, 0 );

	// Initialize with random guess for state
	X_k = cvCreateMat( STATE_DIM, 1, CV_32FC1 );
	cvRandSetRange( &Rng, 0, 0.1, 0 );
	Rng.disttype = CV_RAND_NORMAL;
	cvRand( &Rng, X_k );

	// process noise
	W_k = cvCreateMat( STATE_DIM, 1, CV_32FC1 );

	//Create F matrix:
	// 1,0,dt,0, 
	// 0,1,0,dt,
	// 0,0,1,0, 
	// 0,0,0,1
	//float F[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	//F[2] = frame_rate;
	//F[7] = frame_rate;

	memcpy( Kalman->transition_matrix->data.fl, A, sizeof(A));
	// Initialize other Kalman filter parameters.
	//
	cvSetIdentity( Kalman->measurement_matrix, cvRealScalar(1) );
	cvSetIdentity( Kalman->process_noise_cov, cvRealScalar(1e-5) );
	cvSetIdentity( Kalman->measurement_noise_cov, cvRealScalar(1e-1) );
	cvSetIdentity( Kalman->error_cov_post, cvRealScalar(1));
	// choose random initial state
	cvRand( &Rng, Kalman->state_post );
	return Kalman;
}

//Makesure observed_mat is the following format
//CvMat* z_k = cvCreateMat( MEASURE_DIM, 1, CV_32FC1 );
int reset_counter = 0;
void do_kalman_filter (CvKalman* Kalman,CvMat *observed_mat, IplImage *original_image)
{
	// predict point position
	const CvMat* y_k = cvKalmanPredict( Kalman, 0 );

	// plot points (eg convert to planar coordinates and draw)
	//
	//cvZero( imgKMTest );
	float w = original_image->width;


	cvCircle( original_image, cvPoint( *(float*)(observed_mat->data.ptr) , *(float*)(observed_mat->data.ptr+4) ), 4, CV_RGB(0xff,0xff,0x00) ); // observed state
	cvCircle( original_image, cvPoint( *(float*)(y_k->data.ptr) , *(float*)(y_k->data.ptr+4) ) , 4, CV_RGB(0xff,0x00,0x00), 2 ); // “predicted?state
	//adjust Kalman filter state
	//cvKalmanCorrect( Kalman, observed_mat );
	//cvCircle( original_image, cvPoint( *(float*)(Kalman->state_post->data.ptr) ,*(float*)(Kalman->state_post->data.ptr+4) ), 4, CVX_BLUE ); // correct s

	if(*(float*)(observed_mat->data.ptr) == 0.0 && *(float*)(observed_mat->data.ptr+4) == 0.0)
	{
		reset_counter ++;		
		cvSetIdentity( Kalman->measurement_noise_cov, cvRealScalar(99999999) );
	}
	else if (reset_counter < 10)
	{
		reset_counter = 0;
		cvSetIdentity( Kalman->measurement_noise_cov, cvRealScalar(1e-1) );
		//cvRand( &Rng, observed_mat );
	}
	else
	{
		reset_counter = 0;
		cvSetIdentity( Kalman->measurement_matrix,    cvRealScalar(1) );
		cvSetIdentity( Kalman->process_noise_cov,     cvRealScalar(1e-5) );
		cvSetIdentity( Kalman->measurement_noise_cov, cvRealScalar(1e-1) );
		cvSetIdentity( Kalman->error_cov_post,        cvRealScalar(1));
	}

	cvKalmanCorrect( Kalman, observed_mat );
	cvCircle( original_image, cvPoint( *(float*)(Kalman->state_post->data.ptr) ,*(float*)(Kalman->state_post->data.ptr+4) ), 4, CV_RGB(0x00,0x00,0xff) ); // correct s

	// Apply the transition matrix ‘F?(e.g., step time forward)
	// and also apply the “process?noise w_k.
	//
	cvRandSetRange(&Rng,0,sqrt(Kalman->process_noise_cov->data.fl[0]),	0 );
	cvRand( &Rng, W_k );
	cvMatMulAdd( Kalman->transition_matrix, X_k, W_k, X_k );	
}

//To use this:
//First define frame_rate
//Then call init_kalman_filter
//For every point (x,y) 
//Need to create CvMat* z_k = cvCreateMat( MEASURE_DIM, 1, CV_32FC1 );
//The if you want to track it, accoring to Sam suggestion, 
//you will need to change noise_cov, depending on good or bad image
// if(good_image){
//		cvSetIdentity( Kalman->measurement_noise_cov, cvRealScalar(1e-1) );
//		cvRand( &Rng, z_k );
// } else {
//		cvSetIdentity( Kalman->measurement_noise_cov, cvRealScalar(1212999999912) );
// }
// cvMatMulAdd( Kalman->measurement_matrix, X_k, z_k, z_k );
// do_kalman_filter(z_k);
// See test_kalman_filter for example.
/*
int main_test(int argc, char** argv) {
	float frame_rate = 1.0; // In reality it should be 1/30 if we have 30frames/second
	init_kalman_filter(frame_rate);
	test_kalman_filter();
	return 0;
}
*/

#endif
