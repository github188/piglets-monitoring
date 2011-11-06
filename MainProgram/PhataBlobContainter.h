#pragma once
#include <vector>
#include "../GoogleCode_video-tracker/opencv_cvBlobsLibV8.3/Blob.h"
#include "../GoogleCode_video-tracker/opencv_cvBlobsLibV8.3/BlobOperators.h"
typedef std::vector<CBlob*>	Blob_vector;
typedef std::vector<CBlob*>::iterator	Blob_vector_it;
typedef std::vector<CBlob*>::const_iterator	Blob_vector_constit;
typedef std::vector<double> double_stl_vector;
/**************************************************************************
	Filtres / Filters
**************************************************************************/

//过滤开关（包含与不包含）
#define B_INCLUDE				1L
#define B_EXCLUDE				2L

//过滤条件(比较运算符)
#define B_EQUAL					3L
#define B_NOT_EQUAL				4L
#define B_GREATER				5L
#define B_LESS					6L
#define B_GREATER_OR_EQUAL		7L
#define B_LESS_OR_EQUAL			8L
#define B_INSIDE			    9L
#define B_OUTSIDE			    10L

//计算直方图所使用的颜色空间
#define HIS_RGB                 11L
#define HIS_YCrCb               12L
#define HIS_HSV                 13L
#define HIS_Lab                 14L

class PhataBlobContainter
{
public:
	PhataBlobContainter(void);
	PhataBlobContainter(const PhataBlobContainter &source);//复制构造函数
	virtual ~PhataBlobContainter(void);
	PhataBlobContainter& operator=(const PhataBlobContainter& source);
	PhataBlobContainter  operator+(const PhataBlobContainter& source);
	//虚函数，派生类可改写
	virtual bool findBlobs( IplImage* thresh_image,IplImage *maskImg,unsigned char bgColor);
public:
	//为增加一个blob到blob容器中
	//成功返回true,失败返回false
	bool AddBlob(CBlob *blob);

	//计算所有blob的某一种属性（由funcio_calculBlob决定）的属性值，保存于std::vector<double>中并返回
	double_stl_vector GetSTLResult( funcio_calculBlob *evaluador ) const;

	//计算第indexblob(以0为基数)的某一种属性（由funcio_calculBlob决定）的属性值，保存于dvalue中
	//成功返回true,若索引值indexblob无效则返回false
	bool GetFuncalcuBlobValue( int indexblob, funcio_calculBlob *evaluador, double &dvalue) const;

	//把某一种属性不符合条件的blob过滤掉，把符合条件的blob添加到dst中。const则先添加后再删除原先有的blob
	//filterAction---过滤开关（包含或不包含)
	//evaluador   ---blob的属性
	//condition   ---过滤条件（大于，小于，等于......)
	//lowLimit    ---属性值上限
	//highLimit   ---属性值下限
	void Filter(PhataBlobContainter &dst,
		int filterAction, funcio_calculBlob *evaluador, 
		int condition, double lowLimit, double highLimit = 0 );
	void Filter(PhataBlobContainter &dst,
		int filterAction, funcio_calculBlob *evaluador, 
		int condition, double lowLimit, double highLimit = 0 ) const;


	//把所有blob按某一种属性（由funcio_calculBlob决定）的属性值排序(降序排序)后将第nBlob(以0为基数)个blob保存于dst中.
	//若nBlob不在索引范围内，则dst为空blob，返回false,否则返回true
	bool GetNthBlob( funcio_calculBlob *criteri, int indexblob, CBlob &dst ) const;

	//返回第indexblob(以0为基数)个blob,若indexblob不在索引范围内，则返回空blob
	CBlob GetBlob(int indexblob) const;
	CBlob *GetBlob(int indexblob);

	//查找第一个blob的m_id等于blobID的blob,找不到则返回NULL
	CBlob *FindBlobByID(int blobID);

	//清空所有blob
	void ClearBlobs();

	//把blobs的信息保存于文件中
	bool PrintBlobs( char *nom_fitxer ) const;

	//返回blob容器的大小
	int GetContainterSize() const 
	{ 
		return(m_blobs.size()); 
	}

	//根据srcImg计算所有blobs对应的直方图,srcImg必须能够包含所以blobs的范围
	bool calHis(IplImage *srcImg,int colorSpace=HIS_RGB,IplImage *mask=0);
private:
	//过滤blob的真正调用函数
	void DoFilter(PhataBlobContainter &dst,
		int filterAction, funcio_calculBlob *evaluador, 
		int condition, double lowLimit, double highLimit = 0) const;
public:
	Blob_vector m_blobs;
};

