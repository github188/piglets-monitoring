/*楼主的这个支持汉字输出的CvxText类很实用，尤其是代码规范、简洁明了、考虑周全，堪称编程高手！望继续完善功能。。。

此类已为我顺利使用，谢谢。

我的用法：
1、在自己的工程目录下建立FreeType子目录，然后直接拷贝楼主的FreeType那2个目录文件：include和lib。（有关Freetype的知识就不用去管它了）
2、在自己的工程目录下建立楼主奉献的类文件：CvxText.h, CvxText.cpp
3、VC配置：Project->Setting->C/C++->Preprocessor下的Additional include directories中增加.\FreeType\include
4、直接在CvxText.h文件中增加lib的引入，犹如：
#include <ft2build.h>
#include FT_FREETYPE_H
#pragma comment(lib, ".\\FreeType\\lib\\freetype235.lib")
（注：vc6下Projects setting中未做任何改动，也没有增加libcd.lib的ignore）
5、将C:\WINDOWS\Fonts\中的simhei.ttf,simsun.ttf等要用的字库文件拷贝到自己工程的运行文件目录中


另，发现并修改了个bug：字号大小设置不起作用！！！
需要在setFont()函数中增加：FT_Set_Pixel_Sizes(m_face, (int)m_fontSize.val[0], 0);


至此，CvxText类可以使用自如了
*/

#include "cv.h"
#include "e:\OpenCV资料\opencv实现输出汉字功能\CvxText.h"
int main()
{
	const char *winName = "show";

	IplImage *image = cvLoadImage("D:/123.jpg", CV_LOAD_IMAGE_COLOR);
	assert(image);

	// 输出汉字

	{
		// "simhei.ttf"为windows系统自带的黑体
		// 复制"simhei.ttf"到工程目录

		CvxText text("simhei.ttf");

		const char *msg = "在OpenCV中输出汉字！";

		float p = 0.5;
		text.setFont(NULL, NULL, NULL, &p);   // 透明处理

		text.putText(image, msg, cvPoint(100, 150), CV_RGB(0,0,255));
	}

	cvNamedWindow(winName);
	cvShowImage(winName, image);

	cvWaitKey(0);
	return 0;
}