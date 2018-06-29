#ifndef BMPREAD_H  
#define BMPREAD_H  

//�ļ�ͷ��Ϣ  14���ֽ�
typedef struct
{
	//unsigned short    bfType;  //��cpp�ж�����
	unsigned long    bfSize;//�ļ���С  4���ֽ�
	unsigned short    bfReserved1;//32λ������  2���ֽ�
	unsigned short    bfReserved2;
	unsigned long    bfOffBits;//���ļ�ͷ��ʵ�����ݵ�λ��ƫ������sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + ����λ���ж���û�е�ɫ��
} ClBitMapFileHeader;

//ͼ��ͷ��Ϣ
typedef struct
{
	unsigned long  biSize;
	long   biWidth;
	long   biHeight;
	unsigned short   biPlanes;
	unsigned short   biBitCount;
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	long   biXPelsPerMeter;
	long   biYPelsPerMeter;
	unsigned long   biClrUsed;
	unsigned long   biClrImportant;
} ClBitMapInfoHeader;

typedef struct
{
	unsigned char rgbBlue; //����ɫ����ɫ����  
	unsigned char rgbGreen; //����ɫ����ɫ����  
	unsigned char rgbRed; //����ɫ�ĺ�ɫ����  
	unsigned char rgbReserved; //����ֵ  
} ClRgbQuad;

typedef struct
{
	int width;
	int height;
	int channels;
	unsigned char* imageData;//�޷���0~255�����ڴ������ֵ������ֻ����ͼ�����ָ��
}ClImage;

ClImage* clLoadImage(char* path);//����ֵΪClImage���͵�ָ��

bool clSaveImage(char* path, ClImage* bmpImg);

#endif  