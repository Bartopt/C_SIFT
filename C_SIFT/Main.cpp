#include<stdio.h>
#include <stdlib.h> 
#include <tchar.h>
#include<string.h>
#include "BMPread.h"  
#include "ImageProcess.h" 
#include "SIFT.h"

int _tmain(int argc, _TCHAR* argv[]){
	printf("�����һ��ͼ��\n");
	ClImage* Img1 = clLoadImage("../Image/1.bmp");
	Image* GetImg1 = GetImage(Img1);
	KeyPointGroup* KPG1;
	KPG1 = GetSIFT(GetImg1);
	printf("��һ��ͼ������ɣ�");
	printf("��%d���ؼ���\n", KPG1->number);

	printf("����ڶ���ͼ��\n");
	ClImage* Img2 = clLoadImage("../Image/2.bmp");
	Image* GetImg2 = GetImage(Img2);
	KeyPointGroup* KPG2;
	KPG2 = GetSIFT(GetImg2);
	printf("�ڶ���ͼ������ɣ�");
	printf("��%d���ؼ���\n", KPG2->number);

	printf("����ͼ���������...\n");
	Image* KPImg1 = DrawKeyPoint(GetImg1, KPG1);
	ClImage* KPImg1_result = SaveImage(KPImg1);
	bool flag = clSaveImage("../Image/KP1.bmp", KPImg1_result);
	Image* KPImg2 = DrawKeyPoint(GetImg2, KPG2);
	ClImage* KPImg2_result = SaveImage(KPImg2);
	flag = clSaveImage("../Image/KP2.bmp", KPImg2_result);
	printf("�������\n");

	printf("ƥ������ͼ���������...\n");
	MatchPoint* Matchpoint;
	Matchpoint = GetMatchPoint(KPG1, KPG2);
	printf("ƥ����ɣ�");
	
	printf("һ����%d��ƥ���\n", Matchpoint->Matchnumber);
	printf("����ƥ��ͼ��...\n");
	Image* MatchImg = DrawMatchImage(GetImg1, GetImg2, Matchpoint);//Ҫ������ͼ��һ����
	ClImage* Img_result = SaveImage(MatchImg);
	flag = clSaveImage("../Image/matchimg.bmp", Img_result);
	printf("���...\n");

	/*���ƥ�������
	int i;
	for (i = 0; i < matchpoint->Matchnumber; i++){
	printf("��%d��ƥ���", i);
	printf("Img1:   %d  ", matchpoint->Img1[i].row);
	printf("   %d  ", matchpoint->Img1[i].column);
	printf("Img2:   %d  ", matchpoint->Img2[i].row);
	printf("   %d  ", matchpoint->Img2[i].column);
	printf("\n");
	}
	*/
	/*
	//���������
	int i, j;
	int count = 0;
	int m = 0;
	for (i = 1; i < KPG->number; i++){
		if (KPG->point[i].flag == false){
			count++;
			continue;
		}
		printf("��%d��", KPG->point[i].octavenum);
		printf("    ");
		printf("��%d��", KPG->point[i].intervalnum);
		printf("    ");
		printf("sigma=%f", KPG->point[i].sigma);
		printf("    ");
		printf("��%d��", KPG->point[i].point.row);
		printf("    ");
		printf("��%d��", KPG->point[i].point.column);
		printf("    ");
		printf("�������С%f", KPG->point[i].grad.norm);
		printf("    ");
		printf("���������%f", KPG->point[i].grad.direction);
		printf("    \n");
		printf("SIFT������");
		for (j = 0; j < 128; j++){
			if (j % 8 == 0){
				printf("\n");
			}
			printf("  %f  ", KPG->point[i].descriptor[j]);
		}
		printf("\n");
	}
	printf("һ��%d���ؼ���\n", KPG->number);
	printf(",��%d���ؼ������������ʱԽ��\n", count);
	*/
	/*���������ͼ��
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