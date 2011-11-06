/*¥�������֧�ֺ��������CvxText���ʵ�ã������Ǵ���淶��������ˡ�������ȫ�����Ʊ�̸��֣����������ƹ��ܡ�����

������Ϊ��˳��ʹ�ã�лл��

�ҵ��÷���
1�����Լ��Ĺ���Ŀ¼�½���FreeType��Ŀ¼��Ȼ��ֱ�ӿ���¥����FreeType��2��Ŀ¼�ļ���include��lib�����й�Freetype��֪ʶ�Ͳ���ȥ�����ˣ�
2�����Լ��Ĺ���Ŀ¼�½���¥�����׵����ļ���CvxText.h, CvxText.cpp
3��VC���ã�Project->Setting->C/C++->Preprocessor�µ�Additional include directories������.\FreeType\include
4��ֱ����CvxText.h�ļ�������lib�����룬���磺
#include <ft2build.h>
#include FT_FREETYPE_H
#pragma comment(lib, ".\\FreeType\\lib\\freetype235.lib")
��ע��vc6��Projects setting��δ���κθĶ���Ҳû������libcd.lib��ignore��
5����C:\WINDOWS\Fonts\�е�simhei.ttf,simsun.ttf��Ҫ�õ��ֿ��ļ��������Լ����̵������ļ�Ŀ¼��


�����ֲ��޸��˸�bug���ֺŴ�С���ò������ã�����
��Ҫ��setFont()���������ӣ�FT_Set_Pixel_Sizes(m_face, (int)m_fontSize.val[0], 0);


���ˣ�CvxText�����ʹ��������
*/

#include "cv.h"
#include "e:\OpenCV����\opencvʵ��������ֹ���\CvxText.h"
int main()
{
	const char *winName = "show";

	IplImage *image = cvLoadImage("D:/123.jpg", CV_LOAD_IMAGE_COLOR);
	assert(image);

	// �������

	{
		// "simhei.ttf"Ϊwindowsϵͳ�Դ��ĺ���
		// ����"simhei.ttf"������Ŀ¼

		CvxText text("simhei.ttf");

		const char *msg = "��OpenCV��������֣�";

		float p = 0.5;
		text.setFont(NULL, NULL, NULL, &p);   // ͸������

		text.putText(image, msg, cvPoint(100, 150), CV_RGB(0,0,255));
	}

	cvNamedWindow(winName);
	cvShowImage(winName, image);

	cvWaitKey(0);
	return 0;
}