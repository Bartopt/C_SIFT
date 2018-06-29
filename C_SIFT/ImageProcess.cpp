#include "BMPread.h"  
#include "ImageProcess.h"  
#include <stdio.h>  
#include <stdlib.h> 
#include <assert.h>
#include <math.h>

Image* RGB2Gray(Image* RGBImg){
	Image* GrayImg;//只声明了指针，并没有指定指针指向哪里，野指针，所以要为它分配内存
	int i;
	int j;
	int k;
	int height;
	int width;
	int channels;
	int step;
	int blue;
	int red;
	int green;

	GrayImg = (Image*)malloc(sizeof(Image));

	height = RGBImg->height;
	width = RGBImg->width;
	channels = RGBImg->channels;
	GrayImg->height = height;
	GrayImg->width = width;
	GrayImg->channels = 1;
	step = channels * width;


	GrayImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		GrayImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			GrayImg->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			blue = RGBImg->imageData[i][j][0];//blue 为 int是怕计算的时候溢出
			green = RGBImg->imageData[i][j][1];
			red = RGBImg->imageData[i][j][2];
			GrayImg->imageData[i][j][0] = (red * 299 + green * 587 + blue * 114 + 500) / 1000;//避免浮点运算
		}
	}
	
	return GrayImg;
}

Image* GetImage(ClImage* bmpImg){
	Image* Img;
	int i;
	int j;
	int k;
	int step;
	int height;
	int width;
	int channels;

	Img = (Image*)malloc(sizeof(Image));
	Img->channels = bmpImg->channels;
	Img->height = bmpImg->height;
	Img->width = bmpImg->width;
	channels = bmpImg->channels;
	height = bmpImg->height;
	width = bmpImg->width;
	step = width * channels;

	if (channels == 3){
		Img->imageData = (double ***)malloc(height * sizeof(double**));
		for (i = 0; i < height; i++){
			Img->imageData[i] = (double **)malloc(width * sizeof(double*));
		}
		for (i = 0; i < height; i++){
			for (j = 0; j < width; j++){
				Img->imageData[i][j] = (double *)malloc(channels * sizeof(double));
			}
		}
		for (i = 0; i < height; i++){
			for (j = 0; j < width; j++){
				for (k = 0; k < channels; k++){//这层循环可以不要，直接赋值
					Img->imageData[i][j][k] = bmpImg->imageData[i * step + j * 3 + k];
				}
			}
		}
	}

	if (Img->channels == 1){
		Img->imageData = (double ***)malloc(height * sizeof(double**));
		for (i = 0; i < height; i++){
			Img->imageData[i] = (double **)malloc(width * sizeof(double*));
		}
		for (i = 0; i < height; i++){
			for (j = 0; j < width; j++){
				Img->imageData[i][j] = (double *)malloc(channels * sizeof(double));
			}
		}
		for (i = 0; i < height; i++){
			for (j = 0; j < width; j++){				
				Img->imageData[i][j][0]= bmpImg->imageData[i * step + j];
			}
		}
	}
	return Img;
}

ClImage* SaveImage(Image* Img){
	ClImage* BmpImg;
	int i;
	int j;
	int k;
	int step;
	int height;
	int width;
	int channels;

	BmpImg = (ClImage*)malloc(sizeof(ClImage));
	BmpImg->channels = Img->channels;
	BmpImg->height = Img->height;
	BmpImg->width = Img->width;
	channels = Img->channels;
	height = Img->height;
	width = Img->width;
	step = width * channels;

	if (channels == 3){
		BmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char) * width * 3 * height);
		for (i = 0; i < height; i++){
			for (j = 0; j < width; j++){
				for (k = 0; k < channels; k++){//这层循环可以不要，直接赋值
					if (Img->imageData[i][j][k] < 0){//考虑到类型转化double转unsigned char
						BmpImg->imageData[i * step + j * 3 + k] = 0;
					}
					else if (Img->imageData[i][j][k] > 255){
						BmpImg->imageData[i * step + j * 3 + k] = 255;
					}
					else
						BmpImg->imageData[i * step + j * 3 + k] = Img->imageData[i][j][k];
				}
			}
		}
	}

	if (Img->channels == 1){
		BmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)* width * height);
		for (i = 0; i < height; i++){
			for (j = 0; j < width; j++){
				if (Img->imageData[i][j][0] < 0){
					BmpImg->imageData[i * step + j] = 0;
				}
				else if (Img->imageData[i][j][0] > 255){
					BmpImg->imageData[i * step + j] = 255;
				}
				else
					BmpImg->imageData[i * step + j] = Img->imageData[i][j][0];
			}
		}
	}

	return BmpImg;
}

Image* DownSampling(Image* Img){
	Image* DownImg;//只声明了指针，并没有指定指针指向哪里，野指针，所以要为它分配内存
	int i;
	int j;
	int k;
	int height;
	int width;
	int channels;
	int step;

	DownImg = (Image*)malloc(sizeof(Image));

	height = Img->height / 2;
	width = Img->width / 2;
	channels = Img->channels;
	DownImg->height = height;
	DownImg->width = width;
	DownImg->channels = 1;

	assert(channels == 1);//如果不是灰度图像，则不进行下采样

	DownImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		DownImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			DownImg->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			DownImg->imageData[i][j][0] = Img->imageData[i * 2][j * 2][0];//避免浮点运算
		}
	}

	return DownImg;
}

Filter* GetGaussFilter(double Sigma){
	Filter* filter;
	int i, j;
	int Size;
	double sum = 0.0;

	filter = (Filter*)malloc(sizeof(Filter));
	Size = 6 * Sigma + 1;
	if (Size % 2 == 0){
		Size += 1;//保证卷积核大小为奇数
	}
	filter->Size = Size;

	filter->FilterData = (double **)malloc(Size * sizeof(double*));
	for (i = 0; i < Size; i++){
		filter->FilterData[i] = (double *)malloc(Size * sizeof(double));
	}

	for (i = 0; i < Size; i++){
		for (j = 0; j < Size; j++){
			filter->FilterData[i][j] = exp(-((i - Size / 2)*(i - Size / 2) + (j - Size / 2)*(j - Size / 2)) / (2.0*Sigma*Sigma));
			sum += filter->FilterData[i][j];
		}
	}

	for (i = 0; i < Size; i++){
		for (j = 0; j < Size; j++){
			filter->FilterData[i][j] = filter->FilterData[i][j] / sum;
		}
	}

	return filter;
}

Image* Padding(Image* Img, Filter* Filter){
	Image* PaddingImg;
	int i;
	int j;
	int k;
	int height;
	int width;
	int Size;
	int bound;
	int channels;

	PaddingImg = (Image*)malloc(sizeof(Image));

	height = Img->height + Filter->Size - 1;
	width = Img->width + Filter->Size - 1;
	Size = Filter->Size;
	channels = Img->channels;
	PaddingImg->height = height;
	PaddingImg->width = width;
	PaddingImg->channels = 1;
	bound = (Size - 1) / 2;

	assert(channels == 1);//如果不是灰度图像，则不进行padding

	PaddingImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		PaddingImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			PaddingImg->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			if (bound - 1 < i && i <= (height - bound) - 1 &&
				bound - 1 < j && j <= (width - bound) - 1){
				PaddingImg->imageData[i][j][0] = Img->imageData[i - bound][j - bound][0];
			}
			else
			{
				PaddingImg->imageData[i][j][0] = 0;
			}
		}
	}

	return PaddingImg;

}

Image* Conv2d(Image* Img, Filter* Filter){
	Image* ConvImg;
	Image* PaddingImg;
	int i;
	int j;
	int m;
	int n;
	int height;
	int width;
	int Size;
	int channels;
	int bound;
	double sum = 0;

	ConvImg = (Image*)malloc(sizeof(Image));

	height = Img->height;
	width = Img->width;
	Size = Filter->Size;
	channels = Img->channels;
	ConvImg->height = height;
	ConvImg->width = width;
	ConvImg->channels = 1;
	bound = (Size - 1) / 2;

	ConvImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		ConvImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			ConvImg->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	PaddingImg = Padding(Img, Filter);//先将图像延拓
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			for (m = 0; m < Size; m++){
				for (n = 0; n < Size; n++){
					sum += PaddingImg->imageData[i + m][j + n][0] * Filter->FilterData[m][n];
				}
			}
			ConvImg->imageData[i][j][0] = sum;
			sum = 0;
		}
	}

	//freeImg(PaddingImg);

	return ConvImg;
}

Image* Copy(Image* Img){
	Image* CopyImg;//只声明了指针，并没有指定指针指向哪里，野指针，所以要为它分配内存
	int i;
	int j;
	int k;
	int height;
	int width;
	int channels;
	int step;

	CopyImg = (Image*)malloc(sizeof(Image));

	height = Img->height;
	width = Img->width;
	channels = Img->channels;
	CopyImg->height = height;
	CopyImg->width = width;
	CopyImg->channels = 1;

	assert(channels == 1);//如果不是灰度图像，则不进行COPY

	CopyImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		CopyImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			CopyImg->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			CopyImg->imageData[i][j][0] = Img->imageData[i][j][0];
		}
	}

	return CopyImg;
}

Image* Difference(Image* Imgl, Image* Imgr){
	Image* DiffImg;//只声明了指针，并没有指定指针指向哪里，野指针，所以要为它分配内存
	int i;
	int j;
	int k;
	int height;
	int width;
	int channels;
	int step;

	DiffImg = (Image*)malloc(sizeof(Image));

	height = Imgl->height;
	width = Imgl->width;
	channels = Imgl->channels;
	DiffImg->height = height;
	DiffImg->width = width;
	DiffImg->channels = 1;

	assert(channels == 1);//如果不是灰度图像

	DiffImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		DiffImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			DiffImg->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			DiffImg->imageData[i][j][0] = Imgl->imageData[i][j][0] - Imgr->imageData[i][j][0];//可以乘个10可视化一下
		}
	}

	return DiffImg;
}

Image* CropImage(Image* Img, Rect* Rect){
	Image* CropImg;
	int i;
	int j;
	int k;
	int height;
	int width;
	int channels;
	int step;

	CropImg = (Image*)malloc(sizeof(Image));

	height = Rect->height;
	width = Rect->width;
	channels = Img->channels;
	CropImg->height = height;
	CropImg->width = width;
	CropImg->channels = 1;

	assert(channels == 1);//如果不是灰度图像，则不进行COPY

	CropImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		CropImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			CropImg->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			CropImg->imageData[i][j][0] = Img->imageData[Rect->LefttopPoint.row + i][Rect->LefttopPoint.column + j][0];
		}
	}

	return CropImg;
}

Image* RotateImage(Image* Img, int MainDerection){
	Image* RotateImg;
	Point p;
	int i;
	int j;
	int k;
	int height;
	int width;
	int channels;
	int step;
	double x, y;
	double xr, yr;
	double pixel[4];
	double XAxisLength;
	double YAxisLength;

	RotateImg = (Image*)malloc(sizeof(Image));

	height = Img->height;
	width = Img->width;
	channels = Img->channels;
	RotateImg->height = height;
	RotateImg->width = width;
	RotateImg->channels = 1;
	XAxisLength = ((double)width - 1) / 2;
	YAxisLength = ((double)height - 1) / 2;

	assert(channels == 1);

	RotateImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		RotateImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			RotateImg->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			xr = j - XAxisLength;
			yr = -i + YAxisLength;
			x = cos((double)MainDerection * 3.1415926 / 180.0) * xr - sin((double)MainDerection * 3.1415926 / 180.0) * yr;
			y = sin((double)MainDerection * 3.1415926 / 180.0) * xr + cos((double)MainDerection * 3.1415926 / 180.0) * yr;
			//超出范围则赋值0
			if ((int)(x + XAxisLength) < 0 || (int)(-y + YAxisLength) < 0
				|| (int)(x + XAxisLength) >= width - 1 || (int)(-y + YAxisLength) >= height -1){
				RotateImg->imageData[i][j][0] = 0;
				continue;
			}
			//双线性插值得到旋转图像
			p.column = x - (int)x;
			p.row = y - (int)y;
			pixel[0] = Img->imageData[(int)(-y + YAxisLength)][(int)(x + XAxisLength)][0];
			pixel[1] = Img->imageData[(int)(-y + YAxisLength)][(int)(x + XAxisLength + 1)][0];
			pixel[2] = Img->imageData[(int)(-y + YAxisLength + 1)][(int)(x + XAxisLength)][0];
			pixel[3] = Img->imageData[(int)(-y + YAxisLength + 1)][(int)(x + XAxisLength + 1)][0];
			RotateImg->imageData[i][j][0] = BilinearInterpolation(pixel,p);
		}
	}

	return RotateImg;

}

Mat* GetHessian(Point* point, Image* img){
	Mat* HessianMat;
	const int size = 2;
	int i;
	double f0, f1, f2, f3, f4, f5, f6, f7, f8;


	HessianMat = (Mat*)malloc(sizeof(Mat));
	HessianMat->column = size;
	HessianMat->row = size;

	HessianMat->Data = (double **)malloc(size * sizeof(double*));
	for (i = 0; i < size; i++){
		HessianMat->Data[i] = (double *)malloc(size * sizeof(double));
	}

	f0 = img->imageData[point->row][point->column][0];//不用考虑指针越界，因为留下了一的范围
	f1 = img->imageData[point->row][point->column + 1][0];
	f2 = img->imageData[point->row + 1][point->column][0];
	f3 = img->imageData[point->row][point->column - 1][0];
	f4 = img->imageData[point->row - 1][point->column][0];
	f5 = img->imageData[point->row - 1][point->column + 1][0];
	f6 = img->imageData[point->row + 1][point->column + 1][0];
	f7 = img->imageData[point->row + 1][point->column - 1][0];
	f8 = img->imageData[point->row - 1][point->column - 1][0];

	HessianMat->Data[0][0] = (f1 + f3 - 2 * f0);
	HessianMat->Data[1][1] = (f2 + f4 - 2 * f0);
	HessianMat->Data[0][1] = (f8 + f6 - f5 - f7) / 4;
	HessianMat->Data[1][0] = (f8 + f6 - f5 - f7) / 4;

	return HessianMat;
}

Gradient* GetGradient(Point* point, Image* img){
	Gradient* grad;
	double dx;
	double dy;
	double angle;
	double f1, f3, f2, f4;

	grad = (Gradient*)malloc(sizeof(Gradient));

	f3 = img->imageData[point->row + 1][point->column][0];
	f1 = img->imageData[point->row - 1][point->column][0];
	f4 = img->imageData[point->row][point->column + 1][0];
	f2 = img->imageData[point->row][point->column - 1][0];

	dx = (f3 - f1) / 2.0;
	dy = (f4 - f2) / 2.0;
	angle = atan2(dy, dx) * 180 / 3.1415926;//x为0°方向，顺时针旋转
	if (angle < 0){
		angle = angle + 360;
	}

	grad->norm = sqrt(pow(dx, 2) + pow(dy, 2));
	grad->direction = angle;

	return grad;
}

GradHist* GetGradHist(GradientImage* GradientImg, int AngularResolution, Rect* rect){
	GradHist* Gradhist;
	int AngleNumber;
	int temp;
	int i, j;
	int index;

	AngleNumber = 360 / AngularResolution;

	Gradhist = (GradHist*)malloc(sizeof(GradHist));
	Gradhist->angle = (int*)malloc(sizeof(int)* AngleNumber);
	Gradhist->norm = (double*)malloc(sizeof(double)* AngleNumber);

	for (i = 0; i < AngleNumber; i++){
		Gradhist->angle[i] = i * AngularResolution;
		Gradhist->norm[i] = 0;
	}

	for (i = rect->LefttopPoint.row; i < rect->LefttopPoint.row + rect->height; i++){
		for (j = rect->LefttopPoint.column; j < rect->LefttopPoint.column + rect->width; j++){
			if ((int)GradientImg->DirectionImage->imageData[i][j][0] % AngularResolution > AngularResolution / 2){
				temp = ((int)GradientImg->DirectionImage->imageData[i][j][0] / AngularResolution + 1) * AngularResolution;
				index = temp / AngularResolution;
				Gradhist->norm[index] = Gradhist->norm[index] + GradientImg->NormImage->imageData[i][j][0];
			}
			else{
				temp = (int)GradientImg->DirectionImage->imageData[i][j][0] / AngularResolution * AngularResolution;
				index = temp / AngularResolution;
				Gradhist->norm[index] = Gradhist->norm[index] + GradientImg->NormImage->imageData[i][j][0];
			}
		}
	}

	/*
	AngleNumber = 360 / AngularResolution;
	height = Img->height;
	width = Img->width;

	Gradhist = (GradHist*)malloc(sizeof(GradHist));
	Gradhist->angle = (int*)malloc(sizeof(int)* AngleNumber);
	Gradhist->norm = (double*)malloc(sizeof(double)* AngleNumber);

	for (i = 0; i < AngleNumber; i++){
	Gradhist->angle[i] = i * AngularResolution;
	}

	for (i = 1; i < width - 1; i++){
	for (j = 1; j < height - 1; j++){
	point.column = i;
	point.row = j;
	grad = GetGradient(&point, Img);
	if ((int)grad->direction % AngularResolution > AngularResolution / 2){
	grad->direction = ((int)grad->direction / AngularResolution + 1) * AngularResolution;
	index = grad->direction / AngularResolution - 1;
	Gradhist->norm[index] = grad->norm;
	}
	else
	{
	grad->direction = (int)grad->direction / AngularResolution * AngularResolution;
	index = grad->direction / AngularResolution - 1;
	Gradhist->norm[index] = grad->norm;
	}
	}
	}
	*/

	return Gradhist;
}

GradientImage* GetGradientImage(Image* Img){
	GradientImage* GradientImg;
	Point p;
	Gradient* grad;
	int i;
	int j;
	int k;
	int height;
	int width;
	int channels;

	GradientImg = (GradientImage*)malloc(sizeof(GradientImage));
	GradientImg->NormImage = (Image*)malloc(sizeof(Image));
	GradientImg->DirectionImage = (Image*)malloc(sizeof(Image));

	height = Img->height;
	width = Img->width;
	channels = Img->channels;
	GradientImg->NormImage->height = height;
	GradientImg->NormImage->width = width;
	GradientImg->NormImage->channels = 1;
	GradientImg->DirectionImage->height = height;
	GradientImg->DirectionImage->width = width;
	GradientImg->DirectionImage->channels = 1;
	
	assert(channels == 1);

	GradientImg->NormImage->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		GradientImg->NormImage->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			GradientImg->NormImage->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	GradientImg->DirectionImage->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		GradientImg->DirectionImage->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			GradientImg->DirectionImage->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			//边缘无法计算梯度，梯度置为0
			if (i - 1 < 0 || j - 1 < 0 || i + 2 > height || j + 2 > width){
				GradientImg->DirectionImage->imageData[i][j][0] = 0;
				GradientImg->NormImage->imageData[i][j][0] = 0;
				continue;
			}
			p.row = i;
			p.column = j;
			grad = GetGradient(&p, Img);
			GradientImg->DirectionImage->imageData[i][j][0] = grad->direction;
			GradientImg->NormImage->imageData[i][j][0] = grad->norm;
			//free(grad);
		}
	}

	return GradientImg;
}

double BilinearInterpolation(double pixel[4], Point p){
	double value;

	value = pixel[0] * (1 - p.row) * (1 - p.column) + pixel[1] * p.column * (1 - p.row) + pixel[2] * p.row * (1 - p.column) + pixel[3] * p.row * p.column;

	return value;
}

void freeImg(Image* Img){//这个函数超级慢，没办法使用
	int i, j;
	int height, width;

	height = Img->height;
	width = Img->width;

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			free(Img->imageData[i][j]);
			printf("%d  ", i);
			printf("%d\n", j);
		}
	}
	for (i = 0; i < height; i++){
		free(Img->imageData[i]);
	}
	free(Img->imageData);	
}

void DrawCircle(Image* Img, Point* p, int size, double red, double green, double blue){
	if (p->column + (size - 1) / 2 >= Img->width || p->column - (size - 1) / 2 < 0
		|| p->row + (size - 1) / 2 >= Img->height || p->row - (size - 1) / 2 < 0){
		return;
	}

	double r;
	double tempr;
	int i, j;

	r = size / 2;

	for (i = -(size - 1) / 2; i <= (size - 1) / 2; i++){
		for (j = -(size - 1) / 2; j <= (size - 1) / 2; j++){
			tempr = pow((double)i, 2) + pow((double)j, 2);
			tempr = pow(tempr, 0.5);
			if (fabs((tempr - r)) < 0.5){
				Img->imageData[p->row + i][p->column + j][0] = red;
				Img->imageData[p->row + i][p->column + j][1] = green;
				Img->imageData[p->row + i][p->column + j][2] = blue;
			}
		}
	}
}