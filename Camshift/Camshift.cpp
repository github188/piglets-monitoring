#include "..\Public.h"
void ScaleInitArray(const CvArr* Harr,const CvArr*Sarr,const CvArr* Varr,CvArr* dstarr,
	uchar Hlow, uchar Hhight,uchar Slow,uchar Shight,
	uchar Vlow, uchar Vhight,uchar init)
{
	CvMat Htub, *H;
	CvMat Stub, *S;
	CvMat Vtub, *V;
	CvMat dstub, *dst;
	H = cvGetMat( Harr, &Htub );
	S = cvGetMat( Sarr, &Stub );
	V = cvGetMat( Varr, &Vtub );
	dst = cvGetMat( dstarr, &dstub );
	for(int i=0; i<H->rows; ++i)
		for(int j=0; j<H->cols; ++j)
		{
			uchar huegray=((uchar*)(H->data.ptr + (size_t)H->step*i))[j];
			uchar satgray=((uchar*)(S->data.ptr + (size_t)S->step*i))[j];
			uchar vgray=((uchar*)(V->data.ptr + (size_t)V->step*i))[j];
			if(huegray >= Hlow && huegray <=Hhight && satgray>=Slow && satgray<=Shight 
				&& vgray>=Vlow && vgray <= Vhight)
			{
				((uchar*)(dst->data.ptr + (size_t)dst->step*i))[j]=(uchar)init;
			}
			else
			{
				((uchar*)(dst->data.ptr + (size_t)dst->step*i))[j]=(uchar)0;
			}
		}

}
int main(int argc,char **argv)
{
	IplImage*       tmp_frame = NULL;
	CvCapture*      cap = NULL;
	cap = cvCaptureFromFile(gVRun.c_str());
	if( !cap )
	{
		printf("can not open camera or video file\n");
		return -1;
	}
	tmp_frame = cvQueryFrame(cap);
	if(!tmp_frame)
	{
		printf("can not read data from the video source\n");
		return -1;
	}
	IplImage *hsv = cvCreateImage( cvGetSize(tmp_frame), IPL_DEPTH_8U,3 );
	IplImage *Cr = cvCreateImage( cvGetSize(tmp_frame), IPL_DEPTH_8U, 1);
	IplImage *S = cvCreateImage( cvGetSize(tmp_frame), IPL_DEPTH_8U, 1);
	IplImage *V = cvCreateImage( cvGetSize(tmp_frame),IPL_DEPTH_8U, 1);
	IplImage *dst=cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	cvNamedWindow("Image");
	cvNamedWindow("bw");
	while (tmp_frame=cvQueryFrame(cap))
	{
		cvCvtColor( tmp_frame, hsv, CV_BGR2YCrCb );		
		cvSplit( hsv, 0, Cr, 0, 0 );

		cvCvtColor( tmp_frame, hsv, CV_BGR2HSV );	
		cvSplit( hsv, 0, S, V, 0 );

		cvSaveImage("EqualH.jpg",Cr);
		cvSaveImage("EqualS.jpg",S);
		cvSaveImage("EqualV.jpg",V);
		ScaleInitArray(Cr,S,V,dst,125,255,0,25,140,255,255);
		//ScaleInitArray(Cr,S,V,dst,0,105,0,20,190,255,255);
		cvErode(dst,dst);
		cvDilate(dst,dst);
		cvShowImage("Image",tmp_frame);
		cvShowImage("bw",dst);
		if(cvWaitKey(10)==27) break;
	}
	cvReleaseImage(&hsv);
	cvReleaseImage(&Cr);
	cvReleaseImage(&S);
	cvReleaseImage(&V);
	cvReleaseImage(&dst);
	cvReleaseCapture(&cap);
	cvDestroyAllWindows();
	return 0;

}