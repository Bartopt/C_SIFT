#include<stdio.h>
#include <stdlib.h> 
#include <tchar.h>
#include<string.h>
#include "BMPread.h"  
#include "ImageProcess.h" 
#include "SIFT.h"

int _tmain(int argc, _TCHAR* argv[]){
	printf("处理第一张图像：\n");
	ClImage* Img1 = clLoadImage("../Image/1.bmp");
	Image* GetImg1 = GetImage(Img1);
	KeyPointGroup* KPG1;
	KPG1 = GetSIFT(GetImg1);
	printf("第一张图像处理完成，");
	printf("有%d个关键点\n", KPG1->number);

	printf("处理第二张图像：\n");
	ClImage* Img2 = clLoadImage("../Image/2.bmp");
	Image* GetImg2 = GetImage(Img2);
	KeyPointGroup* KPG2;
	KPG2 = GetSIFT(GetImg2);
	printf("第二张图像处理完成，");
	printf("有%d个关键点\n", KPG2->number);

	printf("绘制图像的特征点...\n");
	Image* KPImg1 = DrawKeyPoint(GetImg1, KPG1);
	ClImage* KPImg1_result = SaveImage(KPImg1);
	bool flag = clSaveImage("../Image/KP1.bmp", KPImg1_result);
	Image* KPImg2 = DrawKeyPoint(GetImg2, KPG2);
	ClImage* KPImg2_result = SaveImage(KPImg2);
	flag = clSaveImage("../Image/KP2.bmp", KPImg2_result);
	printf("绘制完成\n");

	printf("匹配两张图像的特征点...\n");
	MatchPoint* Matchpoint;
	Matchpoint = GetMatchPoint(KPG1, KPG2);
	printf("匹配完成，");
	
	printf("一共有%d对匹配点\n", Matchpoint->Matchnumber);
	printf("储存匹配图像...\n");
	Image* MatchImg = DrawMatchImage(GetImg1, GetImg2, Matchpoint);//要求两张图像一样大
	ClImage* Img_result = SaveImage(MatchImg);
	flag = clSaveImage("../Image/matchimg.bmp", Img_result);
	printf("完成...\n");

	/*输出匹配点坐标
	int i;
	for (i = 0; i < matchpoint->Matchnumber; i++){
	printf("第%d对匹配点", i);
	printf("Img1:   %d  ", matchpoint->Img1[i].row);
	printf("   %d  ", matchpoint->Img1[i].column);
	printf("Img2:   %d  ", matchpoint->Img2[i].row);
	printf("   %d  ", matchpoint->Img2[i].column);
	printf("\n");
	}
	*/
	/*
	//输出描述子
	int i, j;
	int count = 0;
	int m = 0;
	for (i = 1; i < KPG->number; i++){
		if (KPG->point[i].flag == false){
			count++;
			continue;
		}
		printf("第%d塔", KPG->point[i].octavenum);
		printf("    ");
		printf("第%d张", KPG->point[i].intervalnum);
		printf("    ");
		printf("sigma=%f", KPG->point[i].sigma);
		printf("    ");
		printf("第%d行", KPG->point[i].point.row);
		printf("    ");
		printf("第%d列", KPG->point[i].point.column);
		printf("    ");
		printf("主方向大小%f", KPG->point[i].grad.norm);
		printf("    ");
		printf("主方向度数%f", KPG->point[i].grad.direction);
		printf("    \n");
		printf("SIFT描述子");
		for (j = 0; j < 128; j++){
			if (j % 8 == 0){
				printf("\n");
			}
			printf("  %f  ", KPG->point[i].descriptor[j]);
		}
		printf("\n");
	}
	printf("一共%d个关键点\n", KPG->number);
	printf(",有%d个关键点计算特征点时越界\n", count);
	*/
	/*输出金字塔图像
	simg = IP->ImageData[0][0];
	img_result = SaveImage(simg);
	bool flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/00result.bmp", img_result);
	simg = IP->ImageData[0][1];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/01result.bmp", img_result);
	simg = IP->ImageData[0][2];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/02result.bmp", img_result);
	simg = IP->ImageData[0][3];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/03result.bmp", img_result);
	simg = IP->ImageData[0][4];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/04result.bmp", img_result);
	simg = IP->ImageData[0][5];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/05result.bmp", img_result);
	simg = IP->ImageData[1][0];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/10result.bmp", img_result);
	simg = IP->ImageData[1][1];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/11result.bmp", img_result);
	simg = IP->ImageData[1][2];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/12result.bmp", img_result);
	simg = IP->ImageData[1][3];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/13result.bmp", img_result);
	simg = IP->ImageData[1][4];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/14result.bmp", img_result);
	simg = IP->ImageData[1][5];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/15result.bmp", img_result);
	simg = IP->ImageData[2][0];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/20result.bmp", img_result);
	simg = IP->ImageData[2][1];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/21result.bmp", img_result);
	simg = IP->ImageData[2][2];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/22result.bmp", img_result);
	simg = IP->ImageData[2][3];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/23result.bmp", img_result);
	simg = IP->ImageData[2][4];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/24result.bmp", img_result);
	simg = IP->ImageData[2][5];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/25result.bmp", img_result);

	simg = DIP->ImageData[0][0];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D00result.bmp", img_result);
	simg = DIP->ImageData[0][1];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D01result.bmp", img_result);
	simg = DIP->ImageData[0][2];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D02result.bmp", img_result);
	simg = DIP->ImageData[0][3];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D03result.bmp", img_result);
	simg = DIP->ImageData[0][4];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D04result.bmp", img_result);
	simg = DIP->ImageData[1][0];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D10result.bmp", img_result);
	simg = DIP->ImageData[1][1];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D11result.bmp", img_result);
	simg = DIP->ImageData[1][2];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D12result.bmp", img_result);
	simg = DIP->ImageData[1][3];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D13result.bmp", img_result);
	simg = DIP->ImageData[1][4];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D14result.bmp", img_result);
	simg = DIP->ImageData[2][0];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D20result.bmp", img_result);
	simg = DIP->ImageData[2][1];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D21result.bmp", img_result);
	simg = DIP->ImageData[2][2];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D22result.bmp", img_result);
	simg = DIP->ImageData[2][3];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D23result.bmp", img_result);
	simg = IP->ImageData[2][4];
	img_result = SaveImage(simg);
	flag = clSaveImage("C:/Users/Administrator/Desktop/SIFT/D24result.bmp", img_result);


	if (flag)
	{
		printf("save ok... \n");
	}
	*/

	while (1);
	return 0;
}