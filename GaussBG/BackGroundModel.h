#pragma once
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
#include <string>
using std::string;
class BackGroundModel
{
public:
	BackGroundModel(void);
	virtual ~BackGroundModel(void);
protected:
	IplImage *m_bgFrame;
	IplImage *m_fgFrame;
	string   m_winName;
	double durationTime;
public:
	virtual void *CreateBGM(IplImage *bgm)=0;
	virtual bool UpdateBGM(IplImage *frame)=0;
	IplImage *GetBGFrame()const{return m_bgFrame;};
	IplImage *GetFGFrame()const{return m_fgFrame;};
	string GetWinName()const{return m_winName;};
	double GetDurTime()const{return durationTime;};
};

class BackgroundSubtraction:public BackGroundModel
{
public:
	BackgroundSubtraction(void);
	virtual ~BackgroundSubtraction(void);
public:
	void *CreateBGM(IplImage *bgm);
	bool UpdateBGM(IplImage *frame);
};

class AvgBGM:public BackGroundModel
{
public:
	AvgBGM(void);
	virtual ~AvgBGM(void);
public:

	void *CreateBGM(IplImage *bgm);
	bool UpdateBGM(IplImage *frame);
private:
	IplImage *bgframe32;
};

class GMM:public BackGroundModel
{
public:
	GMM(void);
	virtual ~GMM(void);
public:
	void *CreateBGM(IplImage *bgm);
	bool UpdateBGM(IplImage *frame);
private:
	CvGaussBGModel *m_bg_model;
};