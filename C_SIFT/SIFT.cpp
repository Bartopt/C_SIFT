#include "ImageProcess.h"
#include "SIFT.h"
#include "MathFunc.h"
#include <math.h>
#include <stdio.h>  
#include <stdlib.h> 
#include <windows.h>
#include <windef.h>
#include <float.h>
#include <time.h>

ImagePyramid* GetGaussPyramid(Image* Img, int t, int s){
	ImagePyramid* ImgPyramid;
	Image*** ImgP;//Ϊ�˱�ʾ��࣬�������������������ָ���ImgPyramid->ImageData��ok
	Filter* filter;
	int height;
	int width;
	int octavenum;
	int intervalnum;
	int channels = 1;
	float filterSize;
	int i;
	int j;
	float k;
	float sigma;//��һ��ĳ�ʼ�߶�

	ImgPyramid = (ImagePyramid*)malloc(sizeof(ImagePyramid));
	filter = (Filter*)malloc(sizeof(Filter));

	height = Img->height;
	width = Img->width;
	octavenum = (log(double(min(height, width))) - log(double(t))) / log(2.0) + 1;
	intervalnum = s + 3;
	ImgPyramid->IntervalNum = intervalnum;
	ImgPyramid->OctaveNum = octavenum;
	k = pow((float)2, 1 / (float)intervalnum);
	sigma = 0.5;

	//�����ڴ�
	ImgP = (Image ***)malloc(octavenum * sizeof(Image**));
	for (i = 0; i < octavenum; i++){
		ImgP[i] = (Image **)malloc(intervalnum * sizeof(Image*));
	}

	//���������
	for (i = 0; i < octavenum; i++){
		if (i == 0){
			ImgP[i][0] = Copy(Img);//��һ��Ϊԭͼ
		}
		else
		{
			ImgP[i][0] = DownSampling(ImgP[i - 1][intervalnum - 3]);//�ڶ��㼰�Ժ�Ϊ��һ�㵹��������ͼƬ���²���
		}
		filterSize = pow((double)2, (int)i) * sigma;//ÿһoctave��ʼ�ĳ߶�
		for (j = 1; j < intervalnum; j++){
			filter = GetGaussFilter(filterSize);
			ImgP[i][j] = Conv2d(ImgP[i][0], filter);
			filterSize = filterSize * k; 
		}
		
	}

	ImgPyramid->ImageData = ImgP;

	return ImgPyramid;
}

ImagePyramid* GetDiffPyramid(ImagePyramid* ImgPyramid){
	ImagePyramid* DiffImgPyramid;
	Image*** ImgP;//Ϊ�˱�ʾ��࣬�������������������ָ���ImgPyramid->ImageData��ok
	int height;
	int width;
	int octavenum;
	int intervalnum;
	int channels = 1;
	int i;
	int j;


	DiffImgPyramid = (ImagePyramid*)malloc(sizeof(ImagePyramid));

	height = ImgPyramid->ImageData[0][0]->height;
	width = ImgPyramid->ImageData[0][0]->width;
	octavenum = ImgPyramid->OctaveNum;
	intervalnum = ImgPyramid ->IntervalNum - 1;//������1
	DiffImgPyramid->IntervalNum = intervalnum;
	DiffImgPyramid->OctaveNum = octavenum;

	ImgP = (Image ***)malloc(octavenum * sizeof(Image**));
	for (i = 0; i < octavenum; i++){
		ImgP[i] = (Image **)malloc(intervalnum * sizeof(Image*));
	}

	//���������
	for (i = 0; i < octavenum; i++){
		for (j = 0; j < intervalnum; j++){
			ImgP[i][j] = Difference(ImgPyramid->ImageData[i][j + 1], ImgPyramid->ImageData[i][j]);
		}
	}

	DiffImgPyramid->ImageData = ImgP;

	return DiffImgPyramid;
}

KeyPointGroup* GetKeyPointPosition(ImagePyramid* DiffPyramid, ImagePyramid* ImgPyramid){
	KeyPointGroup*  Keypointgroup;
	KeyPoint* TempPoint;
	KeyPoint* TempPoint2;
	Mat* Hessian;
	Filter* gaussfilter;
	Point* Tempoint;
	Gradient* gradient;
	const int bound = 3;
	int i, j;
	int height, width;
	int octavenum, intervalnum;
	int m, n;
	int p, q;
	int flag;//����ֵ�Ͳ��жϼ�Сֵ
	int count;//����
	int length = 1;//��ֵ�����
	int length2 = 1;
	double Tr2, Det;
	double threshold = pow((double)11.0, 2.0) / 10;
	double direction[36];
	float FilterSize;
	int WindowSize;
	float k;//sigma�Ĳ���
	double sigma = 0.5;
	int DirectionIndex;
	int PrincipalDirectionIndex;
	float threshold2 = 5;//��ֵ�����ڴ�����ֵ0.03*255

	Keypointgroup = (KeyPointGroup*)malloc(sizeof(KeyPointGroup));
	TempPoint = (KeyPoint*)malloc(sizeof(KeyPoint));
	TempPoint2 = (KeyPoint*)malloc(sizeof(KeyPoint));
	Tempoint = (Point*)malloc(sizeof(Point));

	height = DiffPyramid->ImageData[0][0]->height;
	width = DiffPyramid->ImageData[0][0]->width;
	octavenum = DiffPyramid->OctaveNum;
	intervalnum = DiffPyramid->IntervalNum;
	TempPoint[0].point.row = 0;//��һ����û��
	TempPoint[0].point.column = 0;
	TempPoint2[0].point.row = 0;//��һ����û��
	TempPoint2[0].point.column = 0;
	k = pow((float)2, 1 / (float)intervalnum);

	//Ѱ�Ҽ�ֵ��
	for (i = 0; i < octavenum; i++){
		for (j = 1; j < intervalnum - 1; j++){//��ֵ��Ҫ��֤���¶���
			for (m = 1; m < DiffPyramid->ImageData[i][0]->height - 1; m++){//����1�ķ�Χ��3*3�Ĵ���ʵ��
				for (n = 1; n < DiffPyramid->ImageData[i][0]->width - 1; n++){
					flag = 1;//�жϼ���ֵ
					for (count = 0,p = -1; p < 2; p++){
						for (q = -1; q < 2; q++){
							if (DiffPyramid->ImageData[i][j]->imageData[m][n][0] >= DiffPyramid->ImageData[i][j]->imageData[m + p][n + q][0]){
								count++;
							}
						}
					}
					if (count == 9 && fabs(DiffPyramid->ImageData[i][j]->imageData[m][n][0]) > threshold2){
						flag = 0;//�����жϼ�Сֵ
						length++;
						TempPoint = (KeyPoint *)realloc(TempPoint, sizeof(KeyPoint)* length);
						TempPoint[length - 1].point.row = m;
						TempPoint[length - 1].point.column = n;
						TempPoint[length - 1].octavenum = i;
						TempPoint[length - 1].intervalnum = j;
						TempPoint[length - 1].sigma = pow((float)2, i + j / (float)intervalnum);//����ͼƬ˳�����
						count = 0;
					}
					if (flag){
						for (count = 0,p = -1; p < 2; p++){
							for (q = -1; q < 2; q++){
								if (DiffPyramid->ImageData[i][j]->imageData[m][n][0] <= DiffPyramid->ImageData[i][j]->imageData[m + p][n + q][0]){
									count++;
								}
							}
						}
					}
					if (count == 9 && fabs(DiffPyramid->ImageData[i][j]->imageData[m][n][0]) > threshold2){
						length++;
						TempPoint = (KeyPoint *)realloc(TempPoint, sizeof(KeyPoint)* length);
						TempPoint[length - 1].point.row = m;
						TempPoint[length - 1].point.column = n;
						TempPoint[length - 1].octavenum = i;
						TempPoint[length - 1].intervalnum = j;
						TempPoint[length - 1].sigma = pow((float)2, i + j / (float)intervalnum);//����ͼƬ˳�����
					}
				}
			}
		}
	}

	//������Ե��
	for (i = 1; i < length; i++){
		Hessian = GetHessian(&TempPoint[i].point, ImgPyramid->ImageData[TempPoint[i].octavenum][TempPoint[i].intervalnum]);
		Tr2 = pow(Hessian->Data[0][0] + Hessian->Data[1][1],2.0);
		Det = Hessian->Data[0][0] * Hessian->Data[1][1] - Hessian->Data[1][0] * Hessian->Data[1][0];
		if ((Tr2 / Det) < threshold){
			length2++;
			TempPoint2 = (KeyPoint *)realloc(TempPoint2, sizeof(KeyPoint)* length2);
			TempPoint2[length2 - 1].point.row = TempPoint[i].point.row;
			TempPoint2[length2 - 1].point.column = TempPoint[i].point.column;
			TempPoint2[length2 - 1].octavenum = TempPoint[i].octavenum;;
			TempPoint2[length2 - 1].intervalnum = TempPoint[i].intervalnum;
			TempPoint2[length2 - 1].sigma = TempPoint[i].sigma;//����ͼƬ˳�����
		}
	}

	int debug = 0;
	//Ѱ��������
	for (i = 1; i < length2; i++){
		for (j = 0; j < 36; j++){
			direction[j] = 0;
		}

		FilterSize = pow((double)2, (int)TempPoint2[length2 - 1].octavenum) * sigma * pow((double)k, (int)TempPoint2[length2 - 1].intervalnum);
		gaussfilter = GetGaussFilter(FilterSize);
		WindowSize = gaussfilter->Size;
		height = ImgPyramid->ImageData[TempPoint[i].octavenum][TempPoint[i].intervalnum]->height;
		width = ImgPyramid->ImageData[TempPoint[i].octavenum][TempPoint[i].intervalnum]->width;

		for (m = -(WindowSize - 1) / 2; m < (WindowSize - 1) / 2; m++){
			for (n = -(WindowSize - 1) / 2; n < (WindowSize - 1) / 2; n++){
				if (TempPoint2[i].point.row + m < 1 || TempPoint2[i].point.row + m > height - 2
					|| TempPoint2[i].point.column + n < 1 || TempPoint2[i].point.column + n > width - 2){
					continue;//�������ͼ��Χ�Լ����ֵķ�Χ����������������õ���ݶ�
				}
				Tempoint->row = TempPoint2[i].point.row + m;
				Tempoint->column = TempPoint2[i].point.column + n;
				gradient = GetGradient(Tempoint, ImgPyramid->ImageData[TempPoint[i].octavenum][TempPoint[i].intervalnum]);
				DirectionIndex = gradient->direction / 10;//����������׼ȷ
				direction[DirectionIndex] = direction[DirectionIndex] + gradient->norm * gaussfilter->FilterData[m + (WindowSize - 1) / 2][n + (WindowSize - 1) / 2];
			}
		}

		PrincipalDirectionIndex = MaxIndex(direction, 36);

		TempPoint2[i].grad.norm = direction[PrincipalDirectionIndex];
		TempPoint2[i].grad.direction = PrincipalDirectionIndex * 10;
		//printf("%d\n", debug++);
		/*
		for (j = 0; j < WindowSize; j++){
			free(gaussfilter->FilterData[j]);
		}
		free(gaussfilter->FilterData);
		free(gaussfilter);
		gaussfilter = NULL;
		*/
	}

	Keypointgroup->point = TempPoint2;
	Keypointgroup->number = length2;

	return Keypointgroup;
}

void GetKeyPointDescriptor(ImagePyramid* DiffPyramid, ImagePyramid* ImgPyramid, KeyPointGroup* KeyPG){
	GradHist* TempGradHist;
	Rect Rect;
	Image* RegionImg;
	Image* RotateImg;
	GradientImage* GradientImg;
	double* Tempdesc;
	int KeyPointNumber;
	int Sigma;
	int RegionSize;
	int SquareSize;
	int MainDerection;
	int AngularResolution;
	int width;
	int height;
	int radius;
	int i;
	int j;
	int m;

	KeyPointNumber = KeyPG->number;
	Sigma = 0.5;
	AngularResolution = 45;

	//��������������
	for (i = 0; i < KeyPointNumber; i++){
		KeyPG->point[i].flag = true;
	}

	for (i = 1; i < KeyPointNumber; i++){
		RegionSize = 5 * 3 * KeyPG->point[i].sigma;
		SquareSize = RegionSize / 5;
		radius = 0.71 * RegionSize;

		//�������������򳬹��߽磬������flagΪfalse��������������
		width = ImgPyramid->ImageData[KeyPG->point[i].octavenum][KeyPG->point[i].intervalnum]->width;
		height = ImgPyramid->ImageData[KeyPG->point[i].octavenum][KeyPG->point[i].intervalnum]->height;
		if (KeyPG->point[i].point.row - radius < 0 || KeyPG->point[i].point.column - radius < 0
			|| KeyPG->point[i].point.column + radius > width || KeyPG->point[i].point.row + radius > height){
			KeyPG->point[i].flag = false;
			continue;
		}

		KeyPG->point[i].descriptor = (double*)malloc(sizeof(double)* 128);
		MainDerection = KeyPG->point[i].grad.direction;
		Rect.LefttopPoint.row = KeyPG->point[i].point.row - radius;//��������һ��һ�е�����
		Rect.LefttopPoint.column = KeyPG->point[i].point.column - radius;
		Rect.height = 2 * radius;
		Rect.width = 2 * radius;

		RegionImg = CropImage(ImgPyramid->ImageData[KeyPG->point[i].octavenum][KeyPG->point[i].intervalnum], &Rect);
		RotateImg = RotateImage(RegionImg, MainDerection);
		GradientImg = GetGradientImage(RotateImg);

		//debug��
		/*
		ClImage* img_result1;
		img_result1 = SaveImage(RegionImg);
		bool flag1 = clSaveImage("C:/Users/Administrator/Desktop/region.bmp", img_result1);
		img_result1 = SaveImage(RotateImg);
		flag1 = clSaveImage("C:/Users/Administrator/Desktop/ratate.bmp", img_result1);
		*/

		for (j = 0; j < 16; j++){
			Rect.LefttopPoint.row = radius - SquareSize * 2 + (j % 4) * SquareSize;//��������һ��һ�е�����
			Rect.LefttopPoint.column = radius - SquareSize * 2 + (j / 4) * SquareSize;
			Rect.height = SquareSize;
			Rect.width = SquareSize;

			TempGradHist = GetGradHist(GradientImg, AngularResolution, &Rect);

			for (m = 0; m < 8; m++){
				KeyPG->point[i].descriptor[j * 8 + m] = TempGradHist->norm[m];
			}

			//��֪��free�ĶԲ���
			//free(TempGradHist);
		}

		//��һ��
		Tempdesc = Normalize(KeyPG->point[i].descriptor, 128);
		for (j = 0; j < 128; j++){
			KeyPG->point[i].descriptor[j] = Tempdesc[j];
		}
		//free(Tempdesc);
	}

}

MatchPoint* GetMatchPoint(KeyPointGroup* KPG1, KeyPointGroup* KPG2){
	MatchPoint* Matchpoint;
	int KPG1Number;
	int KPG2Number;
	int MatchNumber;
	int i, j;
	double nn1,nn2;
	double TempNorm;
	int index;
	double threshold;

	Matchpoint = (MatchPoint*)malloc(sizeof(MatchPoint));
	Matchpoint->Img1 = (Point*)malloc(sizeof(Point));
	Matchpoint->Img2 = (Point*)malloc(sizeof(Point));
	Matchpoint->Matchnumber = 0;
	nn1 = DBL_MAX;
	nn2 = DBL_MAX;
	threshold = 0.9;
	MatchNumber = 0;

	KPG1Number = KPG1->number;
	KPG2Number = KPG2->number;

	for (i = 1; i < KPG1Number; i++){
		if (KPG1->point[i].flag == false){
			continue;
		}

		for (j = 1; j < KPG2Number; j++){
			if (KPG2->point[j].flag == false){
				continue;
			}
			TempNorm = Norm(KPG1->point[i].descriptor, KPG2->point[j].descriptor, 128);
			if (TempNorm < nn1){
				nn1 = TempNorm;
				index = j;
			}
			else if (TempNorm < nn2){
				nn2 = TempNorm;
			}
		}

		if (nn1 / nn2 < threshold){
			MatchNumber++;
			Matchpoint->Img1 = (Point*)realloc(Matchpoint->Img1, sizeof(Point) * MatchNumber);
			Matchpoint->Img2 = (Point*)realloc(Matchpoint->Img2, sizeof(Point)* MatchNumber);
			Matchpoint->Img1[MatchNumber - 1] = GetInitPosition(&KPG1->point[i]);
			Matchpoint->Img2[MatchNumber - 1] = GetInitPosition(&KPG2->point[index]);
			//���ĳ��������ƥ�䣬���ٲ���ƥ��
			KPG2->point[index].flag = false;
			nn1 = DBL_MAX;
			nn2 = DBL_MAX;
		}
	}

	Matchpoint->Matchnumber = MatchNumber;

	return Matchpoint;
}

Point GetInitPosition(KeyPoint* KP){
	Point p;

	p.row = KP->point.row * pow((double)2, KP->octavenum);
	p.column = KP->point.column * pow((double)2, KP->octavenum);

	return p;
}

KeyPointGroup* GetSIFT(Image* Img){
	Image* grayimg;
	ImagePyramid* IP;//64�Ļ���3octave
	ImagePyramid* DIP;
	KeyPointGroup* KPG;
	
	grayimg = RGB2Gray(Img);
	printf("���ڹ�����˹������...\n");
	IP = GetGaussPyramid(grayimg, 64, 1);//ֻ��1��1+3�Ų�����ͨ���������ӣ��������ӣ�����ڶ���ͼ���������ʱ�ͻᱨ�ڴ����
	printf("���ڹ�����˹��ֽ�����...\n"); 
	DIP = GetDiffPyramid(IP);
	printf("����Ѱ�ҹؼ���...\n");
	KPG = GetKeyPointPosition(DIP, IP);
	printf("���ڼ���ؼ���������...\n");
	GetKeyPointDescriptor(DIP, IP, KPG);

	return KPG;
}

Image* DrawMatchImage(Image* Img1, Image* Img2, MatchPoint* Matchpoint){
	Image* MatchImg;
	int linewidth = 2;
	Point* tempp;
	int height;
	int width;
	int channels;
	int i, j, m;
	double k;
	double x1, y1, x2, y2;
	double r, g, b;

	tempp = (Point*)malloc(sizeof(Point));
	MatchImg = (Image*)malloc(sizeof(Image));

	height = Img1->height;
	width = Img1->width + Img2->width;
	channels = Img1->channels;
	MatchImg->height = height;
	MatchImg->width = width;
	MatchImg->channels = channels;
	srand((int)time(0));//�������������

	MatchImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		MatchImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			MatchImg->imageData[i][j] = (double *)malloc(channels * sizeof(double) * channels);
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			if (j < Img1->width){
				MatchImg->imageData[i][j][0] = Img1->imageData[i][j][0];
				MatchImg->imageData[i][j][1] = Img1->imageData[i][j][1];
				MatchImg->imageData[i][j][2] = Img1->imageData[i][j][2];
			}
			else{
				MatchImg->imageData[i][j][0] = Img2->imageData[i][j - Img1->width][0];
				MatchImg->imageData[i][j][1] = Img2->imageData[i][j - Img1->width][1];
				MatchImg->imageData[i][j][2] = Img2->imageData[i][j - Img1->width][2];
			}
		}
	}

	for (m = 0; m < Matchpoint->Matchnumber; m++){
		x1 = Matchpoint->Img1[m].column;
		y1 = Matchpoint->Img1[m].row;
		x2 = Img1->width + Matchpoint->Img2[m].column;
		y2 = Matchpoint->Img2[m].row;
		k = (y2 - y1) / (x2 - x1);
		r = random(255);
		g = random(255);
		b = random(255);

		for (j = 0; j < Matchpoint->Img2[m].column + Img1->width - Matchpoint->Img1[m].column; j++){
			i = Matchpoint->Img1[m].row + k * j;
			tempp->column = Matchpoint->Img2[m].column + Img1->width;
			tempp->row = Matchpoint->Img2[m].row;

			DrawCircle(MatchImg, &Matchpoint->Img1[m], 15, r, g, b);
			DrawCircle(MatchImg, tempp, 15, r, g, b);

			MatchImg->imageData[i][Matchpoint->Img1[m].column + j][0] = r;
			MatchImg->imageData[i][Matchpoint->Img1[m].column + j][1] = g;
			MatchImg->imageData[i][Matchpoint->Img1[m].column + j][2] = b;
			MatchImg->imageData[i + 1][Matchpoint->Img1[m].column + j][0] = r;
			MatchImg->imageData[i + 1][Matchpoint->Img1[m].column + j][1] = g;
			MatchImg->imageData[i + 1][Matchpoint->Img1[m].column + j][2] = b;
		}
	}

	return MatchImg;
}

Image* DrawKeyPoint(Image* Img, KeyPointGroup* KPG){
	Image* KeyPointImg;
	int height;
	int width;
	int channels;
	int i, j, m;
	double r, g, b;

	KeyPointImg = (Image*)malloc(sizeof(Image));

	height = Img->height;
	width = Img->width;
	channels = Img->channels;
	KeyPointImg->height = height;
	KeyPointImg->width = width;
	KeyPointImg->channels = channels;


	KeyPointImg->imageData = (double ***)malloc(height * sizeof(double**));
	for (i = 0; i < height; i++){
		KeyPointImg->imageData[i] = (double **)malloc(width * sizeof(double*));
	}
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			KeyPointImg->imageData[i][j] = (double *)malloc(channels * sizeof(double));
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			KeyPointImg->imageData[i][j][0] = Img->imageData[i][j][0];
			KeyPointImg->imageData[i][j][1] = Img->imageData[i][j][1];
			KeyPointImg->imageData[i][j][2] = Img->imageData[i][j][2];
		}
	}
	
	srand((int)time(0));

	for (i = 1; i < KPG->number; i++){
		r = random(255);
		g = random(255);
		b = random(255);
		DrawCircle(KeyPointImg, &KPG->point[i].point, 15, r, g, b);
	}

	return KeyPointImg;
}