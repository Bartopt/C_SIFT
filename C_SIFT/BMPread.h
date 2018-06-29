#ifndef BMPREAD_H  
#define BMPREAD_H  

//文件头信息  14个字节
typedef struct
{
	//unsigned short    bfType;  //在cpp中定义了
	unsigned long    bfSize;//文件大小  4个字节
	unsigned short    bfReserved1;//32位编译器  2个字节
	unsigned short    bfReserved2;
	unsigned long    bfOffBits;//从文件头到实际数据的位置偏移量，sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 根据位数判断有没有调色板
} ClBitMapFileHeader;

//图像头信息
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
	unsigned char rgbBlue; //该颜色的蓝色分量  
	unsigned char rgbGreen; //该颜色的绿色分量  
	unsigned char rgbRed; //该颜色的红色分量  
	unsigned char rgbReserved; //保留值  
} ClRgbQuad;

typedef struct
{
	int width;
	int height;
	int channels;
	unsigned char* imageData;//无符号0~255，用于存放像素值，这里只存存放图像的首指针
}ClImage;

ClImage* clLoadImage(char* path);//返回值为ClImage类型的指针

bool clSaveImage(char* path, ClImage* bmpImg);

#endif  