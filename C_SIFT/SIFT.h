#ifndef SIFT_H  
#define SIFT_H  
#include "ImageProcess.h"
#include "MathFunc.h"
#define random(x) (rand()%x);

typedef struct
{
	int OctaveNum;
	int IntervalNum;
	Image*** ImageData;//����ָ�룬������ĵ�������
}ImagePyramid;

typedef struct
{
	Point point;
	int octavenum;
	int intervalnum;
	double sigma;//�߶���,����ͼƬ˳�����
	Gradient grad;//������0��350��
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

ImagePyramid* GetGaussPyramid(Image* Img, int t, int s);//t�ǽ��������ͼ��ߴ磬s��ÿ��Octave��������ĳ߶�����

ImagePyramid* GetDiffPyramid(ImagePyramid* ImgPyramid);

KeyPointGroup* GetKeyPointPosition(ImagePyramid* DiffPyramid, ImagePyramid* ImgPyramid);

KeyPointGroup* GetSIFT(Image* Img);

MatchPoint* GetMatchPoint(KeyPointGroup* KPG1, KeyPointGroup* KPG2);

Point GetInitPosition(KeyPoint* KP);

void GetKeyPointDescriptor(ImagePyramid* DiffPyramid, ImagePyramid* ImgPyramid, KeyPointGroup* KeyPG);

Image* DrawMatchImage(Image* Img1, Image* Img2, MatchPoint* Matchpoint);

Image* DrawKeyPoint(Image* Img, KeyPointGroup* KPG);

#endif