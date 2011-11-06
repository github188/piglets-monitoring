#include "BackGroundModel.h"


BackGroundModel::BackGroundModel(void)
	:m_bgFrame(NULL)
	,m_fgFrame(NULL)
	,m_winName("")
{
}


BackGroundModel::~BackGroundModel(void)
{
	if (m_bgFrame)cvReleaseImage(&m_bgFrame);
	if (m_fgFrame)cvReleaseImage(&m_fgFrame);
}
//////////////////////////////////////////////////////////////////////////
BackgroundSubtraction::BackgroundSubtraction( void )
{
	m_winName="减背景法";
}

BackgroundSubtraction::~BackgroundSubtraction( void )
{

}

void *BackgroundSubtraction::CreateBGM( IplImage* bgm )
{
	if(CV_IS_IMAGE(bgm)==false) return NULL;
	if (bgm->nChannels!=3) return NULL;

	if (m_bgFrame)cvReleaseImage(&m_bgFrame);
	m_bgFrame=cvCloneImage(bgm);
	m_fgFrame=cvCreateImage(cvGetSize(m_bgFrame),IPL_DEPTH_8U,1);
	return m_bgFrame;
}

bool BackgroundSubtraction::UpdateBGM( IplImage *frame)
{
	double startTime= (double)cvGetTickCount();
	if(!m_bgFrame) return false;
	if (!frame) return false;
	IplImage *tmpFrame=cvCreateImage(cvGetSize(frame),frame->depth,frame->nChannels);
	cvAbsDiff(m_bgFrame,frame,tmpFrame);
	cvCvtColor(tmpFrame,m_fgFrame,CV_BGR2GRAY);
	cvThreshold(m_fgFrame,m_fgFrame,20,255,CV_THRESH_BINARY);
	cvReleaseImage(&tmpFrame);
	durationTime = ((double)cvGetTickCount()-startTime)/(cvGetTickFrequency()*1000.);
	return true;
}
//////////////////////////////////////////////////////////////////////////
AvgBGM::AvgBGM( void )
	:bgframe32(NULL)
{
	m_winName="平均背景法";
}

AvgBGM::~AvgBGM( void )
{
	if(bgframe32) cvReleaseImage(&bgframe32);
}

void *AvgBGM::CreateBGM( IplImage* bgm )
{
	if(CV_IS_IMAGE(bgm)==false) return NULL;
	if (bgm->nChannels!=3) return NULL;

	if (m_bgFrame)cvReleaseImage(&m_bgFrame);
	m_bgFrame=cvCloneImage(bgm);
	m_fgFrame=cvCreateImage(cvGetSize(m_bgFrame),IPL_DEPTH_8U,1);
	bgframe32=cvCreateImage(cvGetSize(m_bgFrame),IPL_DEPTH_32F,3);
	cvConvertScale(m_bgFrame,bgframe32);
	return m_bgFrame;
}

bool AvgBGM::UpdateBGM(IplImage *frame)
{
	double startTime= (double)cvGetTickCount();
	if(!m_bgFrame) return false;
	if (!frame) return false;
	if (!bgframe32) return false;
	cvRunningAvg(frame,bgframe32,0.7);
	cvConvertScale(bgframe32,m_bgFrame);

	IplImage *tmpFrame=cvCreateImage(cvGetSize(frame),frame->depth,frame->nChannels);
	cvAbsDiff(m_bgFrame,frame,tmpFrame);
	cvCvtColor(tmpFrame,m_fgFrame,CV_BGR2GRAY);
	cvThreshold(m_fgFrame,m_fgFrame,20,255,CV_THRESH_BINARY);
	cvReleaseImage(&tmpFrame);
	durationTime = ((double)cvGetTickCount()-startTime)/(cvGetTickFrequency()*1000.);
	return true;
}
//////////////////////////////////////////////////////////////////////////
GMM::GMM( void )
	:m_bg_model(NULL)
{
	m_winName="高斯混合背景建模";
}

GMM::~GMM( void )
{
	if(m_bg_model) cvReleaseBGStatModel((CvBGStatModel**)&m_bg_model);
}

void *GMM::CreateBGM( IplImage* bgm )
{
	if(CV_IS_IMAGE(bgm)==false) return NULL;
	if (bgm->nChannels!=3) return NULL;

	m_bgFrame=cvCreateImage(cvGetSize(bgm),bgm->depth,bgm->nChannels);
	m_fgFrame=cvCreateImage(cvGetSize(bgm),bgm->depth,1);

	CvGaussBGStatModelParams params;
	params.win_size      = CV_BGFG_MOG_WINDOW_SIZE*4;
	params.bg_threshold  = CV_BGFG_MOG_BACKGROUND_THRESHOLD;
	params.std_threshold = CV_BGFG_MOG_STD_THRESHOLD;
	params.weight_init   = CV_BGFG_MOG_WEIGHT_INIT;
	params.variance_init = CV_BGFG_MOG_SIGMA_INIT*CV_BGFG_MOG_SIGMA_INIT;
	params.minArea       = CV_BGFG_MOG_MINAREA;
	params.n_gauss       = CV_BGFG_MOG_NGAUSSIANS/2;
	m_bg_model = (CvGaussBGModel*)cvCreateGaussianBGModel(bgm,&params );
	return m_bg_model;
}

bool GMM::UpdateBGM( IplImage *frame)
{
	double startTime= (double)cvGetTickCount();
	if(!m_bg_model) return false;
	if (!frame) return false;

	cvUpdateBGStatModel( frame,(CvBGStatModel *)m_bg_model, -1.0 );
	cvUpdate21GaussBackground((CvGaussBGModel*)m_bg_model);
	cvCopyImage(m_bg_model->background,m_bgFrame);
	cvCopyImage(m_bg_model->foreground,m_fgFrame);
	durationTime = ((double)cvGetTickCount()-startTime)/(cvGetTickFrequency()*1000.);
    return true;
}
//////////////////////////////////////////////////////////////////////////