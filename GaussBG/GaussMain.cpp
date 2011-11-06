#include "..\Public.h"
#include "BackGroundModel.h"

int main(int argc, char** argv)
{
	const int METHODNUM=3;
	IplImage*       tmp_frame = NULL;
	CvCapture*      cap = NULL;
	bool update_bg_model = true;
	cap = cvCaptureFromFile("c:\\MOV00225.avi");
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
	BackGroundModel *bgm[METHODNUM];
	bgm[0]=new BackgroundSubtraction();
	bgm[1]=new AvgBGM();
	bgm[2]=new GMM();
	int fr=0; 
	bool bfirstframe=true;
	while(tmp_frame = cvQueryFrame(cap))
	{
		if(bfirstframe)
		{
			for (int i=0; i<METHODNUM; ++i)
			{
				bgm[i]->CreateBGM(tmp_frame);
			}
			bfirstframe=false;
			continue;
		}
		for (int i=0; i<METHODNUM; ++i)
		{
			bgm[i]->UpdateBGM(tmp_frame);
		}
		printf( "%d. %.1f\n", fr++, bgm[2]->GetDurTime());
		cvShowImage("原视频", tmp_frame);
		cvSaveImage("原视频.jpg", tmp_frame);
		for (int i=0; i<METHODNUM; ++i)
		{
			string name=bgm[i]->GetWinName()+"_BG.jpg";
			cvShowImage(name.c_str(), bgm[i]->GetBGFrame());
			cvSaveImage(name.c_str(),bgm[i]->GetBGFrame());
			name=bgm[i]->GetWinName()+"_FG.jpg";
			cvShowImage(name.c_str(), bgm[i]->GetFGFrame());
			cvSaveImage(name.c_str(),bgm[i]->GetFGFrame());
		}
		if(cvWaitKey(5) == 27 ) break;
	}
	for (int i=0; i<METHODNUM; ++i)
	{
		delete bgm[i];
	}
	cvReleaseCapture(&cap);
	cvDestroyAllWindows();
	return 0;
}
