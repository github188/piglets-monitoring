#include "Public.h"
static const float SinTable[] =
{ 0.0000000f, 0.0174524f, 0.0348995f, 0.0523360f, 0.0697565f, 0.0871557f,
0.1045285f, 0.1218693f, 0.1391731f, 0.1564345f, 0.1736482f, 0.1908090f,
0.2079117f, 0.2249511f, 0.2419219f, 0.2588190f, 0.2756374f, 0.2923717f,
0.3090170f, 0.3255682f, 0.3420201f, 0.3583679f, 0.3746066f, 0.3907311f,
0.4067366f, 0.4226183f, 0.4383711f, 0.4539905f, 0.4694716f, 0.4848096f,
0.5000000f, 0.5150381f, 0.5299193f, 0.5446390f, 0.5591929f, 0.5735764f,
0.5877853f, 0.6018150f, 0.6156615f, 0.6293204f, 0.6427876f, 0.6560590f,
0.6691306f, 0.6819984f, 0.6946584f, 0.7071068f, 0.7193398f, 0.7313537f,
0.7431448f, 0.7547096f, 0.7660444f, 0.7771460f, 0.7880108f, 0.7986355f,
0.8090170f, 0.8191520f, 0.8290376f, 0.8386706f, 0.8480481f, 0.8571673f,
0.8660254f, 0.8746197f, 0.8829476f, 0.8910065f, 0.8987940f, 0.9063078f,
0.9135455f, 0.9205049f, 0.9271839f, 0.9335804f, 0.9396926f, 0.9455186f,
0.9510565f, 0.9563048f, 0.9612617f, 0.9659258f, 0.9702957f, 0.9743701f,
0.9781476f, 0.9816272f, 0.9848078f, 0.9876883f, 0.9902681f, 0.9925462f,
0.9945219f, 0.9961947f, 0.9975641f, 0.9986295f, 0.9993908f, 0.9998477f,
1.0000000f, 0.9998477f, 0.9993908f, 0.9986295f, 0.9975641f, 0.9961947f,
0.9945219f, 0.9925462f, 0.9902681f, 0.9876883f, 0.9848078f, 0.9816272f,
0.9781476f, 0.9743701f, 0.9702957f, 0.9659258f, 0.9612617f, 0.9563048f,
0.9510565f, 0.9455186f, 0.9396926f, 0.9335804f, 0.9271839f, 0.9205049f,
0.9135455f, 0.9063078f, 0.8987940f, 0.8910065f, 0.8829476f, 0.8746197f,
0.8660254f, 0.8571673f, 0.8480481f, 0.8386706f, 0.8290376f, 0.8191520f,
0.8090170f, 0.7986355f, 0.7880108f, 0.7771460f, 0.7660444f, 0.7547096f,
0.7431448f, 0.7313537f, 0.7193398f, 0.7071068f, 0.6946584f, 0.6819984f,
0.6691306f, 0.6560590f, 0.6427876f, 0.6293204f, 0.6156615f, 0.6018150f,
0.5877853f, 0.5735764f, 0.5591929f, 0.5446390f, 0.5299193f, 0.5150381f,
0.5000000f, 0.4848096f, 0.4694716f, 0.4539905f, 0.4383711f, 0.4226183f,
0.4067366f, 0.3907311f, 0.3746066f, 0.3583679f, 0.3420201f, 0.3255682f,
0.3090170f, 0.2923717f, 0.2756374f, 0.2588190f, 0.2419219f, 0.2249511f,
0.2079117f, 0.1908090f, 0.1736482f, 0.1564345f, 0.1391731f, 0.1218693f,
0.1045285f, 0.0871557f, 0.0697565f, 0.0523360f, 0.0348995f, 0.0174524f,
0.0000000f, -0.0174524f, -0.0348995f, -0.0523360f, -0.0697565f, -0.0871557f,
-0.1045285f, -0.1218693f, -0.1391731f, -0.1564345f, -0.1736482f, -0.1908090f,
-0.2079117f, -0.2249511f, -0.2419219f, -0.2588190f, -0.2756374f, -0.2923717f,
-0.3090170f, -0.3255682f, -0.3420201f, -0.3583679f, -0.3746066f, -0.3907311f,
-0.4067366f, -0.4226183f, -0.4383711f, -0.4539905f, -0.4694716f, -0.4848096f,
-0.5000000f, -0.5150381f, -0.5299193f, -0.5446390f, -0.5591929f, -0.5735764f,
-0.5877853f, -0.6018150f, -0.6156615f, -0.6293204f, -0.6427876f, -0.6560590f,
-0.6691306f, -0.6819984f, -0.6946584f, -0.7071068f, -0.7193398f, -0.7313537f,
-0.7431448f, -0.7547096f, -0.7660444f, -0.7771460f, -0.7880108f, -0.7986355f,
-0.8090170f, -0.8191520f, -0.8290376f, -0.8386706f, -0.8480481f, -0.8571673f,
-0.8660254f, -0.8746197f, -0.8829476f, -0.8910065f, -0.8987940f, -0.9063078f,
-0.9135455f, -0.9205049f, -0.9271839f, -0.9335804f, -0.9396926f, -0.9455186f,
-0.9510565f, -0.9563048f, -0.9612617f, -0.9659258f, -0.9702957f, -0.9743701f,
-0.9781476f, -0.9816272f, -0.9848078f, -0.9876883f, -0.9902681f, -0.9925462f,
-0.9945219f, -0.9961947f, -0.9975641f, -0.9986295f, -0.9993908f, -0.9998477f,
-1.0000000f, -0.9998477f, -0.9993908f, -0.9986295f, -0.9975641f, -0.9961947f,
-0.9945219f, -0.9925462f, -0.9902681f, -0.9876883f, -0.9848078f, -0.9816272f,
-0.9781476f, -0.9743701f, -0.9702957f, -0.9659258f, -0.9612617f, -0.9563048f,
-0.9510565f, -0.9455186f, -0.9396926f, -0.9335804f, -0.9271839f, -0.9205049f,
-0.9135455f, -0.9063078f, -0.8987940f, -0.8910065f, -0.8829476f, -0.8746197f,
-0.8660254f, -0.8571673f, -0.8480481f, -0.8386706f, -0.8290376f, -0.8191520f,
-0.8090170f, -0.7986355f, -0.7880108f, -0.7771460f, -0.7660444f, -0.7547096f,
-0.7431448f, -0.7313537f, -0.7193398f, -0.7071068f, -0.6946584f, -0.6819984f,
-0.6691306f, -0.6560590f, -0.6427876f, -0.6293204f, -0.6156615f, -0.6018150f,
-0.5877853f, -0.5735764f, -0.5591929f, -0.5446390f, -0.5299193f, -0.5150381f,
-0.5000000f, -0.4848096f, -0.4694716f, -0.4539905f, -0.4383711f, -0.4226183f,
-0.4067366f, -0.3907311f, -0.3746066f, -0.3583679f, -0.3420201f, -0.3255682f,
-0.3090170f, -0.2923717f, -0.2756374f, -0.2588190f, -0.2419219f, -0.2249511f,
-0.2079117f, -0.1908090f, -0.1736482f, -0.1564345f, -0.1391731f, -0.1218693f,
-0.1045285f, -0.0871557f, -0.0697565f, -0.0523360f, -0.0348995f, -0.0174524f,
-0.0000000f, 0.0174524f, 0.0348995f, 0.0523360f, 0.0697565f, 0.0871557f,
0.1045285f, 0.1218693f, 0.1391731f, 0.1564345f, 0.1736482f, 0.1908090f,
0.2079117f, 0.2249511f, 0.2419219f, 0.2588190f, 0.2756374f, 0.2923717f,
0.3090170f, 0.3255682f, 0.3420201f, 0.3583679f, 0.3746066f, 0.3907311f,
0.4067366f, 0.4226183f, 0.4383711f, 0.4539905f, 0.4694716f, 0.4848096f,
0.5000000f, 0.5150381f, 0.5299193f, 0.5446390f, 0.5591929f, 0.5735764f,
0.5877853f, 0.6018150f, 0.6156615f, 0.6293204f, 0.6427876f, 0.6560590f,
0.6691306f, 0.6819984f, 0.6946584f, 0.7071068f, 0.7193398f, 0.7313537f,
0.7431448f, 0.7547096f, 0.7660444f, 0.7771460f, 0.7880108f, 0.7986355f,
0.8090170f, 0.8191520f, 0.8290376f, 0.8386706f, 0.8480481f, 0.8571673f,
0.8660254f, 0.8746197f, 0.8829476f, 0.8910065f, 0.8987940f, 0.9063078f,
0.9135455f, 0.9205049f, 0.9271839f, 0.9335804f, 0.9396926f, 0.9455186f,
0.9510565f, 0.9563048f, 0.9612617f, 0.9659258f, 0.9702957f, 0.9743701f,
0.9781476f, 0.9816272f, 0.9848078f, 0.9876883f, 0.9902681f, 0.9925462f,
0.9945219f, 0.9961947f, 0.9975641f, 0.9986295f, 0.9993908f, 0.9998477f,
1.0000000f
};

void sincos( int angle, float& cosval, float& sinval)
{
	angle += (angle < 0 ? 360 : 0);
	sinval = SinTable[angle];
	cosval = SinTable[450 - angle];
}

//寻找文件夹folde下的所有suffix后缀文件的名称(不包括路径)，保存在vector中并返回
//folde--输入变量,所要寻找的文件夹(命名规则待定)
//suffix--输入变量,所要寻找的文件的后缀（所有文件请用"*.*"）
//ishavepath--输入变量，返回的文件名包不包含路径，默认为false不包含
vector<string> GetAllFileName(string folde,string suffix, bool ishavepath/*=false*/)
{
	vector<string> allFileName;
	if (folde.at(folde.length()-1)!='\\')
	{
		folde+='\\';
	}
	string myfolde=folde;
	myfolde+=suffix;
	struct _finddata_t c_file; 
	intptr_t hFile; 
	if( (hFile = _findfirst( myfolde.c_str(), &c_file )) == -1L )//这里路径字符串可以用宏定义以便win平台和linux平台通用 
	{	
		//printf( "No *.txt files in current directory!\n" ); 
		return allFileName;
	}
	else 
	{ 
		do
		{ 
			//printf( " %-12s %9ld\n", c_file.name, c_file.size ); 
			string fileName(c_file.name);
			if (ishavepath)
			{
				allFileName.push_back(folde+fileName);
			}else{
				allFileName.push_back(fileName);
			}
		} while( _findnext( hFile, &c_file ) == 0 ); 
		_findclose( hFile ); 
	}
	return allFileName;
}

void cvInternetLightingCompensation(const IplImage *inimg, IplImage *outimg)
{
	CV_Assert( (inimg->depth==outimg->depth) && (inimg->nChannels==3 || inimg->nChannels==1) &&
		inimg->width==outimg->width && inimg->height==outimg->height &&
		inimg->nChannels==outimg->nChannels );
	long histogramGray[256];
	memset(histogramGray,0,sizeof(long)*256);
	if (inimg->nChannels==3)
	{
		for (int i=0; i<inimg->height; ++i)
		{
			for (int j=0; j<inimg->width; ++j)
			{
				uchar rpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+2];
				uchar gpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+1];
				uchar bpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+0];
				uchar graypiex=0.212671*rpiex + 0.715160*gpiex + 0.072169*bpiex;
				histogramGray[graypiex]++;
			}
		}

	}else if (inimg->nChannels==1)
	{
		for (int i=0; i<inimg->height; ++i)
		{
			for (int j=0; j<inimg->width; ++j)
			{
				uchar graypiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j];
				histogramGray[graypiex]++;
			}
		}
	}
	int calnum=0, num=255;
	const int total=inimg->width * inimg->height;
	double threshodlco=0.05;//前5%的高亮像素
	for (int i=0; i<256; ++i)//得到前5%的高亮像素的位置num
	{
		if (calnum*1.0/total < threshodlco)
		{
			calnum +=histogramGray[255-i];
			num=i;
		}
		else
		{
			break;
		}
	}
	//if (num>100)//不需要进行光线补偿
	//{
	//	cvCopy(inimg,outimg);
	//	return;
	//}
	int avggray=0;
	calnum=0;
	for (int i=255; i>=255-num; --i)
	{
		avggray +=histogramGray[i] *i;//总的像素的个数*灰度值 
		calnum +=histogramGray[i];//总的像素的个数
	}

	//得到光线补偿的系数
	double co = 255.0/(avggray*1.0/calnum);
	//下面的循环对图象进行光线补偿
	for (int i=0; i<inimg->height; ++i)
	{
		for (int j=0; j<inimg->width; ++j)
		{
			for (int k=0; k<inimg->nChannels; ++k)
			{
				int  piex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+k] * co;
				piex =(piex=piex >255 ? 255 : piex) < 0 ? 0 : piex;
				((uchar*)(outimg->imageData+i*outimg->widthStep))[j*outimg->nChannels+k]=piex;
			}
		}
	}
}
//我的按照Face Detection in Color Images编的方法
void cvMyLightingCompensation(const IplImage *inimg, IplImage *outimg)
{
	CV_Assert( (inimg->depth==outimg->depth) && (inimg->nChannels==3 || inimg->nChannels==1) &&
		inimg->width==outimg->width && inimg->height==outimg->height &&
		inimg->nChannels==outimg->nChannels );
	long histogram[256];
	memset(histogram,0,sizeof(long)*256);
	if (inimg->nChannels==3)
	{
		for (int i=0; i<inimg->height; ++i)
		{
			for (int j=0; j<inimg->width; ++j)
			{
				uchar rpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+2];
				uchar gpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+1];
				uchar bpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+0];
				uchar graypiex=0.212671*rpiex + 0.715160*gpiex + 0.072169*bpiex;
				histogram[graypiex]++;
			}
		}

	}else if (inimg->nChannels==1)
	{
		for (int i=0; i<inimg->height; ++i)
		{
			for (int j=0; j<inimg->width; ++j)
			{
				uchar graypiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j];
				histogram[graypiex]++;
			}
		}
	}
	long calnum=0, num=0;
	const int total=inimg->width * inimg->height;
	double threshodlco=0.05;//前5%的高亮像素
	for (int i=255; i>=0; --i)
	{
		if (histogram[i]>0)
		{
			num=i*(1-threshodlco);//得到前5%的高亮像素的位置num
			break;
		}
	}
	for (int i=255; i>=num; --i)//得到前5%的高亮像素的位置num
	{
		calnum +=histogram[i];
	}
	if (calnum<(total*0.01))//不需要进行光线补偿
	{
		cvCopy(inimg,outimg);
		return;
	}
	int avggray=0;
	calnum=0;
	for (int i=255; i>=num; --i)
	{
		avggray +=histogram[i] *i;//总的像素的个数*灰度值 
		calnum +=histogram[i];//总的像素的个数
	}

	//得到光线补偿的系数
	double co = 255.0/(avggray*1.0/calnum);
	//下面的循环对图象进行光线补偿
	for (int i=0; i<inimg->height; ++i)
	{
		for (int j=0; j<inimg->width; ++j)
		{
			for (int k=0; k<inimg->nChannels; ++k)
			{
				int  piex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+k] * co;
				piex =(piex=piex >255 ? 255 : piex) < 0 ? 0 : piex;
				((uchar*)(outimg->imageData+i*outimg->widthStep))[j*outimg->nChannels+k]=piex;
			}
		}
	}
}

//基于肤色的人脸检测算法研究中提到的改进方法
void cvSuperLightingCompensation(const IplImage *inimg, IplImage *outimg)
{
	CV_Assert( (inimg->depth==outimg->depth) && (inimg->nChannels==3 || inimg->nChannels==1) &&
		inimg->width==outimg->width && inimg->height==outimg->height &&
		inimg->nChannels==outimg->nChannels );
	long histogramGray[256];
	memset(histogramGray,0,sizeof(long)*256);
	if (inimg->nChannels==3)
	{
		for (int i=0; i<inimg->height; ++i)
		{
			for (int j=0; j<inimg->width; ++j)
			{
				uchar rpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+2];
				uchar gpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+1];
				uchar bpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+0];
				uchar graypiex=0.212671*rpiex + 0.715160*gpiex + 0.072169*bpiex;
				histogramGray[graypiex]++;
			}
		}

	}else if (inimg->nChannels==1)
	{
		for (int i=0; i<inimg->height; ++i)
		{
			for (int j=0; j<inimg->width; ++j)
			{
				uchar graypiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j];
				histogramGray[graypiex]++;
			}
		}
	}
	long calnum=0, num=0;
	const int total=inimg->width * inimg->height;
	double threshodlco=0.05;//前5%的高亮像素
	for (int i=255; i>=0; --i)
	{
		if (histogramGray[i]>0)
		{
			num=i*(1-threshodlco);//得到前5%的高亮像素的位置num
			break;
		}
	}
	for (int i=255; i>=num; --i)//得到前5%的高亮像素的位置num
	{
		calnum +=histogramGray[i];
	}
	if (calnum<(total*0.01))//不需要进行光线补偿
	{
		cvCopy(inimg,outimg);
		return;
	}
	long avggray=0;
	calnum=0;
	for (int i=255; i>=num; --i)
	{
		avggray +=histogramGray[i] *i;//总的像素的个数*灰度值 
		calnum +=histogramGray[i];//总的像素的个数
	}

	//得到光线补偿的系数
	double co = 255.0/(avggray*1.0/calnum);
	//下面的循环对图象进行光线补偿
	for (int i=0; i<inimg->height; ++i)
	{
		for (int j=0; j<inimg->width; ++j)
		{
			if (inimg->nChannels==3)
			{
				int rpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+2];
				int gpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+1];
				int bpiex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+0];
				uchar graypiex=0.212671*rpiex + 0.715160*gpiex + 0.072169*bpiex;
				if (graypiex>=num)//将前5%像素的B分量和G分量的调整按照前5%像素R分量调整均值的90%（经验值）进行，
					//替代了原来统一调整为255的做法
				{
					rpiex*=co;
					rpiex =(rpiex=rpiex >255 ? 255 : rpiex) < 0 ? 0 : rpiex;
					((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+2]=rpiex;
					gpiex*=co*0.9;
					gpiex =(gpiex=gpiex >255 ? 255 : gpiex) < 0 ? 0 : gpiex;
					((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+1]=gpiex;
					bpiex*=co*0.9;
					bpiex =(bpiex=bpiex >255 ? 255 : bpiex) < 0 ? 0 : bpiex;
					((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+0]=bpiex;
				}else{//其他像素不变
					for (int k=0; k<inimg->nChannels; ++k)
					{
						int  piex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+k] * co;
						piex =(piex=piex >255 ? 255 : piex) < 0 ? 0 : piex;
						((uchar*)(outimg->imageData+i*outimg->widthStep))[j*outimg->nChannels+k]=piex;
					}
				}
			}else{
				for (int k=0; k<inimg->nChannels; ++k)
				{
					int  piex=((uchar*)(inimg->imageData+i*inimg->widthStep))[j*inimg->nChannels+k] * co;
					piex =(piex=piex >255 ? 255 : piex) < 0 ? 0 : piex;
					((uchar*)(outimg->imageData+i*outimg->widthStep))[j*outimg->nChannels+k]=piex;
				}
			}
		}
	}
}

CvRect GetMinRect(IplImage* srcImg, CvScalar bgColor/*=CV_RGB(0,0,0)*/)//返回图像的外包围矩形范围
{
	CvRect roiRc=cvRect(0,0,0,0);
	if(!CV_IS_IMAGE(srcImg) )
	{
		return roiRc;
	}
	bool isGetTop=false,isGetBottom=false,isGetLeft=false,isGetRight=false;//是否得到各边界
	for(int i=0; i<srcImg->height; ++i){
		for(int top=0; top<srcImg->width && !isGetTop; ++top){
			CvScalar piexColor=bgColor;
			for (int k=0; k<srcImg->nChannels; ++k){
				uchar piex=((uchar*)srcImg->imageData + i*srcImg->widthStep)[top*srcImg->nChannels+k];
				piexColor.val[k]=piex;
			}
			if (piexColor.val[0]==bgColor.val[0] &&
				piexColor.val[1]==bgColor.val[1] &&
				piexColor.val[2]==bgColor.val[2] &&
				piexColor.val[3]==bgColor.val[3]){
					continue;
			}else{
				roiRc.y=i;//得到最顶行
				isGetTop=true;
				break;
			}
		}
	}
	for(int i=srcImg->height-1; i>0; --i){
		for(int bottom=0; bottom < srcImg->width && !isGetBottom; ++bottom){
			CvScalar piexColor=bgColor;
			for (int k=0; k<srcImg->nChannels; ++k){
				uchar piex=((uchar*)srcImg->imageData + i*srcImg->widthStep)[bottom*srcImg->nChannels+k];
				piexColor.val[k]=piex;
			}
			if (piexColor.val[0]==bgColor.val[0] &&
				piexColor.val[1]==bgColor.val[1] &&
				piexColor.val[2]==bgColor.val[2] &&
				piexColor.val[3]==bgColor.val[3]){
					continue;
			}else{
				roiRc.height=i;//得到最低行
				isGetBottom=true;
				break;
			}
		}
	}

	for(int i=0; i<srcImg->width; ++i){
		for(int left=0; left<srcImg->height && !isGetLeft; ++left){
			CvScalar piexColor=bgColor;
			for (int k=0; k<srcImg->nChannels; ++k){
				uchar piex=((uchar*)srcImg->imageData + left*srcImg->widthStep)[i*srcImg->nChannels+k];
				piexColor.val[k]=piex;
			}
			if (piexColor.val[0]==bgColor.val[0] &&
				piexColor.val[1]==bgColor.val[1] &&
				piexColor.val[2]==bgColor.val[2] &&
				piexColor.val[3]==bgColor.val[3]){
					continue;
			}else{
				roiRc.x=i;//得到最左列
				isGetLeft=true;
				break;
			}
		}
	}
	for(int i=srcImg->width - 1; i>0; --i)
	{
		for(int right=0; right<srcImg->height && !isGetRight; ++right){
			CvScalar piexColor=bgColor;
			for (int k=0; k<srcImg->nChannels; ++k){
				uchar piex=((uchar*)srcImg->imageData + right*srcImg->widthStep)[i*srcImg->nChannels+k];
				piexColor.val[k]=piex;
			}
			if (piexColor.val[0]==bgColor.val[0] &&
				piexColor.val[1]==bgColor.val[1] &&
				piexColor.val[2]==bgColor.val[2] &&
				piexColor.val[3]==bgColor.val[3]){
					continue;
			}else{
				roiRc.width=i;//得到最右列
				isGetRight=true;
				break;
			}
		}
	}
	if (!isGetLeft || !isGetRight || !isGetTop || !isGetBottom)
	{
		return roiRc;//没找到目标，全部都是背景
	}
	roiRc.width=roiRc.width-roiRc.x+1;
	roiRc.height=roiRc.height-roiRc.y+1;

	if (roiRc.x+roiRc.width>srcImg->width)
	{
		roiRc.width--;
	}
	if (roiRc.y+roiRc.height>srcImg->height)
	{
		roiRc.height--;
	}
	return roiRc;

}
//裁减图像（根据图像的最小包围矩形提取图像）
//srcImg--输入变量,所要裁减的灰度字符图像
//dstImg--输出变量,提取后的灰度图像       注:由于dstImg在函数中被释放,故得使用指针的指针
//bgColor--输入变量，背景颜色（非此颜色则为前景）
//返回值：true表成功，false表失败（图像指针有误或输入图像没有全部都是背景）
bool CutDownImage(IplImage* srcImg, IplImage **dstImg,CvScalar bgColor/*=CV_RGB(0,0,0)*/)
{
	if(!srcImg || !dstImg || srcImg==*dstImg || srcImg->nChannels!=1)
	{
		return false;
	}
	CvRect roiRc=GetMinRect(srcImg,bgColor);

	IplImage *roiImg=cvCreateImage(cvSize(roiRc.width,roiRc.height),IPL_DEPTH_8U,1);
	cvSetImageROI(srcImg,roiRc);
	cvCopyImage(srcImg,roiImg);
	cvResetImageROI(srcImg);

	cvReleaseImage(dstImg);
	*dstImg=cvCloneImage(roiImg);
	return true;
}
//找出图像surMark中的最大轮廓
CvSeq* GetMaxContour(IplImage *surMark, CvMemStorage* storage)
{
	if (!CV_IS_IMAGE(surMark) || surMark->nChannels!=1 || surMark->depth !=IPL_DEPTH_8U)
	{
		return NULL;
	}
	CvSeq* contour = 0;
	CvSeq* maxcontour=0;
	double dMaxArea=0;
	IplImage *tmpImg=cvCloneImage(surMark);//cvFindContours会改变图像的内容
	cvFindContours( tmpImg, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	cvReleaseImage(&tmpImg);
	for( ; contour != 0; contour = contour->h_next )
	{
		CvRect rc=((CvContour*)contour)->rect;
		if (rc.width*rc.height>dMaxArea)
		{
			dMaxArea=rc.width*rc.height;
			maxcontour=contour;
		}
	}
	return maxcontour;
}
//在图像srcImg上根据contour轮廓画上最小外接矩形
CvBox2D DrawMinAreaRect(IplImage *srcImg,CvSeq *contour,CvScalar color/*=CV_RGB(255,0,0)*/)
{
	CvBox2D box=cvMinAreaRect2(contour);
	CvPoint2D32f box_vtx[4];
	CvPoint pt0, pt;
	cvBoxPoints( box, box_vtx );
	pt0.x = cvRound(box_vtx[3].x);
	pt0.y = cvRound(box_vtx[3].y);
	for(int i = 0; i < 4; i++ )
	{
		pt.x = cvRound(box_vtx[i].x);
		pt.y = cvRound(box_vtx[i].y);
		cvLine(srcImg, pt0, pt, color, 1, CV_AA, 0);
		pt0 = pt;
	}
	return box;
}

//在图像srcImg上根据contour轮廓画上最小外接椭圆
CvBox2D DrawMinAreaEllipse(IplImage *srcImg,CvSeq *contour,CvScalar color/*=CV_RGB(255,0,0)*/)
{
	int count = contour->total; // This is number point in contour
	CvPoint center;
	CvSize size;
	CvBox2D box;

	if( count < 6 )
		return box;

	CvMat* points_f = cvCreateMat( 1, count, CV_32FC2 );
	CvMat points_i = cvMat( 1, count, CV_32SC2, points_f->data.ptr );
	cvCvtSeqToArray( contour, points_f->data.ptr, CV_WHOLE_SEQ );
	cvConvert( &points_i, points_f );
	// 椭圆拟合
	box = cvFitEllipse2( points_f );
	cout<<"拟合的椭圆参数：angle="<<box.angle<<",center=("<<box.center.x<<","
		<<box.center.y<<")"<<",size(w,h)=("<<box.size.width<<","<<box.size.height<<")"<<endl;
	// 获得椭圆参数
	center = cvPointFrom32f(box.center);
	size.width = cvRound(box.size.width*0.5)+1;
	size.height = cvRound(box.size.height*0.5)+1;
	// 画椭圆
	cvEllipse(srcImg, center, size,
		-box.angle, 0, 360,	color, 1, CV_AA, 0);
	cvReleaseMat(&points_f);
	return box;
}

//对比矩形共有面积是否在阈值范围内
bool CommonArea(CvRect rectT1,CvRect rectT2,float threshold)
{
	if (rectT1.x>rectT2.x+rectT2.width||rectT2.x>rectT1.x+rectT1.width)
	{
		return false;
	}
	if (rectT1.y>rectT2.y+rectT2.height||rectT2.y>rectT1.y+rectT1.height)
	{
		return false;
	}
	int maxLeftTopX,maxLeftTopY,minRightButtonX,minRightButtonY;
	float commonArea,rectT1Area,rectT2Area;
	float rate1,rate2;
	maxLeftTopX=std::max(rectT1.x,rectT2.x);
	maxLeftTopY=std::max(rectT1.y,rectT2.y);
	minRightButtonX=std::min(rectT1.x+rectT1.width,rectT2.x+rectT2.width);
	minRightButtonY=std::min(rectT1.y+rectT1.height,rectT2.y+rectT2.height);
	
	commonArea=(float)(minRightButtonX-maxLeftTopX)*(minRightButtonY-maxLeftTopY);
	rectT1Area=(float)rectT1.height*rectT1.width;
	rectT2Area=(float)rectT2.height*rectT2.width;
	rate1=commonArea/rectT1Area;
	rate2=commonArea/rectT2Area;
	return (rate1>threshold || rate2>threshold);
}