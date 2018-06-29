#ifndef IMAGEPROCESS_H  
#define IMAGEPROCESS_H  
#include "BMPread.h"  
#include "MathFunc.h"

typedef struct
{
	int width;
	int height;
	int channels;
	double*** imageData;//����ָ�룬������ĵ�������
}Image;

typedef struct
{
	Image* NormImage;
	Image* DirectionImage;
}GradientImage;

typedef struct
{
	int Size;
	double** FilterData;//����ָ�룬��֧�ֶ�ά���
}Filter;

typedef struct
{
	int* angle;
	double* norm;
}GradHist;

ClImage* SaveImage(Image* Img);//��ͼ��תΪClImage���Ϳ��Ե��ú������棬��ʵ���Ժϲ��������C++�Ļ�����֮������ݳ�Ա���ͨ�ţ�

Image* RGB2Gray(Image* Img);//��RGBתΪGray

Image* GetImage(ClImage* bmpImg);//��BMP�ж�ȡͼ��

Image* DownSampling(Image* Img);//�²���,����ԻҶ�ͼ��

Image* Conv2d(Image* Img, Filter* filter);//���

Image* Padding(Image* Img, Filter* filter);//padding

Image* Copy(Image* Img); //����ͼ��

Image* Difference(Image* Imgl, Image* Imgr);

Image* CropImage(Image* Img, Rect* Rect);

Image* RotateImage(Image* Img, int MainDerection);

Filter* GetGaussFilter(double Sigma);

Mat* GetHessian(Point* point, Image* img);

Gradient* GetGradient(Point* point, Image* img);

GradHist* GetGradHist(GradientImage* GradientImg, int AngularResolution, Rect* rect);

GradientImage* GetGradientImage(Image* Img);

double BilinearInterpolation(double pixel[4], Point p);

void DrawCircle(Image* Img, Point* p, int size, double red, double green, double blue);//size����Ϊ����

void freeImg(Image* Img);

#endif