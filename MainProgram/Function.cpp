#include "Function.h"
void CreateMark(IplImage *srcImg,IplImage *dstImg)
{
	cvCvtColor(srcImg,dstImg,CV_RGB2GRAY);
	cvThreshold(dstImg,dstImg,253,255,CV_THRESH_BINARY);
	cvDilate(dstImg,dstImg);
	cvErode(dstImg,dstImg);
	////调用如下
	//int main(int argc, char *agrv)
	//{
	//	string path="..\\Mark.jpg";
	//	IplImage *img=cvLoadImage(path.c_str());
	//	IplImage *mark=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	//	CreateMark(img,mark);
	//	path=path.substr(0,path.find_last_of('\\')+1)+"MarkDst.jpg";
	//	cvSaveImage(path.c_str(),mark);
	//	cvShowImage("Mark",mark);
	//	cvReleaseImage(&img);
	//	cvReleaseImage(&mark);
	//	cvDestroyAllWindows();
	//	return 0;
	//}
}

//#include "..\Public.h"
//#include "Function.h"
//int main( int argc, char** argv )
//{ 
//	string path="..\\MarkDst.jpg";
//	IplImage *img=cvLoadImage(path.c_str());
//	CvCapture* capture = 0;
//	capture = cvCaptureFromAVI(TESTVIDEO1.c_str());
//	IplImage *pframe=cvQueryFrame(capture);
//	IplImage*onlybalustrade=cvCloneImage(pframe);
//	IplImage*deletebalustrade=cvCloneImage(pframe);
//	while(pframe=cvQueryFrame(capture))
//	{
//		cvAnd(pframe,img,onlybalustrade);
//		cvOr(pframe,img,deletebalustrade);
//		cvShowImage("原图像",pframe);
//		cvShowImage("有栏杆",onlybalustrade);
//		cvShowImage("去栏杆",deletebalustrade);
//		if(cvWaitKey(10)==27) break;
//
//	}
//	cvReleaseImage(&img);
//	cvReleaseImage(&onlybalustrade);
//	cvReleaseImage(&deletebalustrade);
//	cvReleaseCapture(&capture);
//	cvDestroyAllWindows();
//	return 0;
//}