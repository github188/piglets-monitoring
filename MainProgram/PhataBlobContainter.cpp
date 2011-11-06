#include <limits.h>
#include <stdio.h>
#include <functional>
#include <algorithm>
#include "PhataBlobContainter.h"
#include "../GoogleCode_video-tracker/opencv_cvBlobsLibV8.3/ComponentLabeling.h"


PhataBlobContainter::PhataBlobContainter(void)
{
	m_blobs=Blob_vector();
}

PhataBlobContainter::PhataBlobContainter( const PhataBlobContainter &source )
{
	ClearBlobs();
	m_blobs = Blob_vector( source.GetContainterSize() );
	Blob_vector::const_iterator pBlobsSrc = source.m_blobs.begin();
	Blob_vector::iterator pBlobsDst = m_blobs.begin();
	while( pBlobsSrc != source.m_blobs.end() )
	{
		*pBlobsDst = new CBlob(**pBlobsSrc);
		pBlobsSrc++;
		pBlobsDst++;
	}
}

PhataBlobContainter::~PhataBlobContainter(void)
{
	ClearBlobs();
}

PhataBlobContainter& PhataBlobContainter::operator=( const PhataBlobContainter& source )
{
	if (this != &source){
		ClearBlobs();
		m_blobs = Blob_vector( source.GetContainterSize() );
		Blob_vector::const_iterator pBlobsSrc = source.m_blobs.begin();
		Blob_vector::iterator pBlobsDst = m_blobs.begin();
		while( pBlobsSrc != source.m_blobs.end() )
		{
			*pBlobsDst = new CBlob(**pBlobsSrc);
			pBlobsSrc++;
			pBlobsDst++;
		}
	}
	return *this;
}

PhataBlobContainter PhataBlobContainter::operator+( const PhataBlobContainter& source )
{
	PhataBlobContainter resultat( *this );
	resultat.m_blobs.resize( resultat.GetContainterSize() + source.GetContainterSize() );

	Blob_vector::const_iterator pBlobsSrc = source.m_blobs.begin();
	Blob_vector::iterator pBlobsDst = resultat.m_blobs.end();
	while( pBlobsSrc != source.m_blobs.end() )
	{
		pBlobsDst--;
		*pBlobsDst = new CBlob(**pBlobsSrc);
		pBlobsSrc++;
	}
	return resultat;
}

bool PhataBlobContainter::findBlobs( IplImage* thresh_image,IplImage *maskImg,unsigned char bgColor)
{
	bool success;
	try
	{
		success = ComponentLabeling( thresh_image, maskImg, bgColor, m_blobs );
	}
	catch(...)
	{
		success = false;
	}
	return success;
}

bool PhataBlobContainter::AddBlob( CBlob *blob )
{
	if( blob){
		m_blobs.push_back( new CBlob( blob ) );
		return true;
	}
	return false;
}

double_stl_vector PhataBlobContainter::GetSTLResult( funcio_calculBlob *evaluador ) const
{

	if( GetContainterSize() <= 0 ){
		return double_stl_vector();
	}
	double_stl_vector result = double_stl_vector( GetContainterSize() );	
	double_stl_vector::iterator itResult = result.begin();
	Blob_vector::const_iterator itBlobs = m_blobs.begin();
	while( itBlobs != m_blobs.end() )
	{
		*itResult = (*evaluador)(**itBlobs);
		itBlobs++;
		itResult++;
	}
	return result;
}

bool PhataBlobContainter::GetFuncalcuBlobValue( int indexblob, funcio_calculBlob *evaluador, double &dvalue) const
{
	if( indexblob < 0 || indexblob >= GetContainterSize() ){
		return false;
	}
	dvalue=(*evaluador)( *m_blobs[indexblob] );
	return true;
}

void PhataBlobContainter::Filter( PhataBlobContainter &dst, int filterAction, 
	funcio_calculBlob *evaluador, int condition, 
	double lowLimit, double highLimit /*= 0 */ ) const
{
	DoFilter(dst, filterAction, evaluador, condition, lowLimit, highLimit );
}

void PhataBlobContainter::Filter( PhataBlobContainter &dst, int filterAction, 
	funcio_calculBlob *evaluador, int condition, 
	double lowLimit, double highLimit /*= 0 */ ) 
{
	int numBlobs = GetContainterSize();
	DoFilter(dst, filterAction, evaluador, condition, lowLimit, highLimit );
	
	if( &dst == this ) //由于为const函数，若相等，则删除原先的blobs
	{
		Blob_vector::const_iterator itBlobs = m_blobs.begin();
		for( int i = 0; i < numBlobs; i++ )
		{
			delete *itBlobs;
			itBlobs++;
		}
		m_blobs.erase( m_blobs.begin(), itBlobs );
	}
}

bool PhataBlobContainter::GetNthBlob( funcio_calculBlob *criteri, int indexblob, CBlob &dst ) const
{
	if( indexblob < 0 || indexblob >= GetContainterSize() )
	{
		dst = CBlob();
		return false;
	}
	double_stl_vector avaluacioBlobs = GetSTLResult(criteri);
	double_stl_vector avaluacioBlobsOrdenat = double_stl_vector( GetContainterSize() );
	std::partial_sort_copy( avaluacioBlobs.begin(), 
		avaluacioBlobs.end(),
		avaluacioBlobsOrdenat.begin(), 
		avaluacioBlobsOrdenat.end(),
		std::greater<double>() );
	double valorEnessim = avaluacioBlobsOrdenat[indexblob];
	double_stl_vector::const_iterator itAvaluacio = avaluacioBlobs.begin();

	bool trobatBlob = false;
	int indexBlob = 0;
	while( itAvaluacio != avaluacioBlobs.end() && !trobatBlob )
	{
		if( *itAvaluacio == valorEnessim )
		{
			trobatBlob = true;
			dst = CBlob( GetBlob(indexBlob));
		}
		itAvaluacio++;
		indexBlob++;
	}
	return true;
}

CBlob PhataBlobContainter::GetBlob( int indexblob ) const
{
	if( indexblob < 0 || indexblob >= GetContainterSize() )
	{
		return CBlob();
	}
	return *m_blobs[indexblob];
}

CBlob * PhataBlobContainter::GetBlob( int indexblob )
{
	if( indexblob < 0 || indexblob >= GetContainterSize() )
	{
		return NULL;		
	}
	return m_blobs[indexblob];
}

void PhataBlobContainter::ClearBlobs()
{
	Blob_vector::iterator itBlobs = m_blobs.begin();
	while( itBlobs != m_blobs.end() )
	{
		delete *itBlobs;
		itBlobs++;
	}
	m_blobs.clear();
}

bool PhataBlobContainter::PrintBlobs( char *nom_fitxer ) const
{
	double_stl_vector area, perimetre, exterior, compacitat, longitud, 
		externPerimeter, perimetreConvex/*, perimetre*/;
	int i;
	FILE *fitxer_sortida;
	fitxer_sortida = fopen( nom_fitxer, "w" );
	if (!fitxer_sortida)
	{
		return false;
	}
	area      = GetSTLResult( CBlobGetArea());
	perimetre = GetSTLResult( CBlobGetPerimeter());
	exterior  = GetSTLResult( CBlobGetExterior());
	compacitat = GetSTLResult(CBlobGetCompactness());
	longitud  = GetSTLResult( CBlobGetLength());
	externPerimeter = GetSTLResult( CBlobGetExternPerimeter());
	perimetreConvex = GetSTLResult( CBlobGetHullPerimeter());

	for(i=0; i<GetContainterSize(); i++)
	{
		//		fprintf( fitxer_sortida, "blob %d ->\t a=%7.0f\t p=%8.2f
		//		(%8.2f extern)\t pconvex=%8.2f\t ext=%.0f\t m=%7.2f\t
		//		c=%3.2f\t l=%8.2f\n",  // DTS 7/29/10 to eliminate build
		//		warning, removed m=%7.2f\t  not sure what m was supposed to be...
		fprintf( fitxer_sortida, "blob %d ->\t a=%7.0f\t p=%8.2f (%8.2f extern)\t pconvex=%8.2f\t ext=%.0f\t c=%3.2f\t l=%8.2f\n",
			i, area[i], perimetre[i], externPerimeter[i], perimetreConvex[i], exterior[i], compacitat[i], longitud[i] );
    }
	fclose( fitxer_sortida );
	return true;
}

void PhataBlobContainter::DoFilter( PhataBlobContainter &dst, int filterAction, 
	funcio_calculBlob *evaluador, int condition, double lowLimit, double highLimit /*= 0*/ ) const
{
	int i=0;
	bool resultavaluacio;
	double_stl_vector avaluacioBlobs;
	double_stl_vector::iterator itavaluacioBlobs;

	int numBlobs = GetContainterSize();
	if( numBlobs <= 0 ) return;
	if( !evaluador ) return;

	avaluacioBlobs = GetSTLResult(evaluador);
	itavaluacioBlobs = avaluacioBlobs.begin();
	switch(condition)
	{
	case B_EQUAL:
		for(i=0;i<numBlobs;i++, itavaluacioBlobs++){
			resultavaluacio= *itavaluacioBlobs == lowLimit;
			if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
				( !resultavaluacio && filterAction == B_EXCLUDE ))
			{
				dst.m_blobs.push_back( new CBlob( GetBlob( i ) ));
			}
		}
		break;
	case B_NOT_EQUAL:
		for(i=0;i<numBlobs;i++, itavaluacioBlobs++){
			resultavaluacio = *itavaluacioBlobs != lowLimit;
			if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
				( !resultavaluacio && filterAction == B_EXCLUDE ))
			{
				dst.m_blobs.push_back( new CBlob( GetBlob( i ) ));
			}
		}
		break;
	case B_GREATER:
		for(i=0;i<numBlobs;i++, itavaluacioBlobs++){
			resultavaluacio= *itavaluacioBlobs > lowLimit;
			if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
				( !resultavaluacio && filterAction == B_EXCLUDE ))
			{
				dst.m_blobs.push_back( new CBlob( GetBlob( i ) ));
			}
		}
		break;
	case B_LESS:
		for(i=0;i<numBlobs;i++, itavaluacioBlobs++){
			resultavaluacio= *itavaluacioBlobs < lowLimit;
			if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
				( !resultavaluacio && filterAction == B_EXCLUDE ))
			{
				dst.m_blobs.push_back( new CBlob( GetBlob( i ) ));
			}
		}
		break;
	case B_GREATER_OR_EQUAL:
		for(i=0;i<numBlobs;i++, itavaluacioBlobs++){
			resultavaluacio= *itavaluacioBlobs>= lowLimit;
			if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
				( !resultavaluacio && filterAction == B_EXCLUDE ))
			{
				dst.m_blobs.push_back( new CBlob( GetBlob( i ) ));
			}
		}
		break;
	case B_LESS_OR_EQUAL:
		for(i=0;i<numBlobs;i++, itavaluacioBlobs++){
			resultavaluacio= *itavaluacioBlobs <= lowLimit;
			if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
				( !resultavaluacio && filterAction == B_EXCLUDE ))
			{
				dst.m_blobs.push_back( new CBlob( GetBlob( i ) ));
			}
		}
		break;
	case B_INSIDE:
		for(i=0;i<numBlobs;i++, itavaluacioBlobs++){

			resultavaluacio=( *itavaluacioBlobs >= lowLimit) && ( *itavaluacioBlobs <= highLimit); 
			if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
				( !resultavaluacio && filterAction == B_EXCLUDE ))
			{
				dst.m_blobs.push_back( new CBlob( GetBlob( i ) ));
			}
		}
		break;
	case B_OUTSIDE:
		for(i=0;i<numBlobs;i++, itavaluacioBlobs++){
			resultavaluacio=( *itavaluacioBlobs < lowLimit) || ( *itavaluacioBlobs > highLimit); 
			if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
				( !resultavaluacio && filterAction == B_EXCLUDE ))
			{
				dst.m_blobs.push_back( new CBlob( GetBlob( i ) ));
			}
		}
		break;
	}
}

CBlob * PhataBlobContainter::FindBlobByID( int blobID )
{
	Blob_vector_constit it=m_blobs.begin();
	Blob_vector_constit itEnd=m_blobs.end();
	while(it!=itEnd){
		if ((*it)->GetID()==blobID)
		{
			return *it;
		}
		it++;
	}
	return NULL;
}

bool PhataBlobContainter::calHis( IplImage *srcImg,int colorSpace/*=HIS_RGB*/,IplImage *mask/*=0*/ )
{
	IplImage *tmpImg=0;
	IplImage *img_plane[3];
	for (int i=0; i<3; ++i){
		img_plane[i]=cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,1);
	}
	switch(colorSpace){
	case  HIS_RGB:
		tmpImg=srcImg;
		break;
	case HIS_YCrCb:
		tmpImg=cvCreateImage(cvGetSize(srcImg),srcImg->depth,srcImg->nChannels);
		cvCvtColor(srcImg,tmpImg,CV_BGR2YCrCb);
		break;
	case HIS_Lab:
		tmpImg=cvCreateImage(cvGetSize(srcImg),srcImg->depth,srcImg->nChannels);
		cvCvtColor(srcImg,tmpImg,CV_BGR2Lab);
		break;
	case HIS_HSV:
		tmpImg=cvCreateImage(cvGetSize(srcImg),srcImg->depth,srcImg->nChannels);
		cvCvtColor(srcImg,tmpImg,CV_BGR2HSV);
		break;
	default:
		tmpImg=srcImg;
		break;
	}
	cvSplit(tmpImg,img_plane[0],img_plane[1],img_plane[2],0);
	int size=this->GetContainterSize();
	for (int i=0; i<size; ++i)
	{
		CvRect rc=this->GetBlob(i)->GetBoundingBox();
		for(int j=0; j<3; ++j){
			this->GetBlob(i)->SetHistogram(j,img_plane[j],mask);
		}
	}

	if(tmpImg!=srcImg)cvReleaseImage(&tmpImg);
	for (int i=0; i<3; ++i){
		if(img_plane[i])cvReleaseImage(&img_plane[i]);
	}
	return true;
}
