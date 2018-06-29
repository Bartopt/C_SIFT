#ifndef SIFT_H  
#define SIFT_H  
#include "ImageProcess.h"
#include "MathFunc.h"
#define random(x) (rand()%x);

typedef struct
{
	int OctaveNum;
	int IntervalNum;
	Image*** ImageData;//三级指针，更方便的调用像素
}ImagePyramid;

typedef struct
{
	Point point;
	int octavenum;
	int intervalnum;
	double sigma;//尺度数,先用图片顺序记着
	Gradient grad;//主方向0到350°
	double* descriptor;
	bool flag;
}KeyPoint;

typedef struct
{
	KeyPoint* point;
	int number;
}KeyPointGroup;

typedef struct
{
	Point* Img1;
	Point* Img2;
	int Matchnumber;
}MatchPoint;

ImagePyramid* GetGaussPyramid(Image* Img, int t, int s);//t是金字塔最顶层图像尺寸，s是每个Octave内特征点的尺度数；

ImagePyramid* GetDiffPyramid(ImagePyramid* ImgPyramid);

KeyPointGroup* GetKeyPointPosition(ImagePyramid* DiffPyramid, ImagePyramid* ImgPyramid);

KeyPointGroup* GetSIFT(Image* Img);

MatchPoint* GetMatchPoint(KeyPointGroup* KPG1, KeyPointGroup* KPG2);

Point GetInitPosition(KeyPoint* KP);

void GetKeyPointDescriptor(ImagePyramid* DiffPyramid, ImagePyramid* ImgPyramid, KeyPointGroup* KeyPG);

Image* DrawMatchImage(Image* Img1, Image* Img2, MatchPoint* Matchpoint);

Image* DrawKeyPoint(Image* Img, KeyPointGroup* KPG);

#endif