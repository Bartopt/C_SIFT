#ifndef IMAGEPROCESS_H  
#define IMAGEPROCESS_H  
#include "BMPread.h"  
#include "MathFunc.h"

typedef struct
{
	int width;
	int height;
	int channels;
	double*** imageData;//三级指针，更方便的调用像素
}Image;

typedef struct
{
	Image* NormImage;
	Image* DirectionImage;
}GradientImage;

typedef struct
{
	int Size;
	double** FilterData;//二级指针，仅支持二维卷积
}Filter;

typedef struct
{
	int* angle;
	double* norm;
}GradHist;

ClImage* SaveImage(Image* Img);//将图像转为ClImage，就可以调用函数保存，其实可以合并。如果是C++的话对象之间的数据成员如何通信？

Image* RGB2Gray(Image* Img);//将RGB转为Gray

Image* GetImage(ClImage* bmpImg);//从BMP中读取图像

Image* DownSampling(Image* Img);//下采样,仅针对灰度图像

Image* Conv2d(Image* Img, Filter* filter);//卷积

Image* Padding(Image* Img, Filter* filter);//padding

Image* Copy(Image* Img); //复制图像

Image* Difference(Image* Imgl, Image* Imgr);

Image* CropImage(Image* Img, Rect* Rect);

Image* RotateImage(Image* Img, int MainDerection);

Filter* GetGaussFilter(double Sigma);

Mat* GetHessian(Point* point, Image* img);

Gradient* GetGradient(Point* point, Image* img);

GradHist* GetGradHist(GradientImage* GradientImg, int AngularResolution, Rect* rect);

GradientImage* GetGradientImage(Image* Img);

double BilinearInterpolation(double pixel[4], Point p);

void DrawCircle(Image* Img, Point* p, int size, double red, double green, double blue);//size必须为奇数

void freeImg(Image* Img);

#endif