#ifndef MATHFUNC_H  
#define MATHFUNC_H

typedef struct
{
	int row;
	int column;
}Point;

typedef struct
{
	int row;
	int column;
	double** Data;
}Mat;

typedef struct
{
	double norm;
	double direction;
}Gradient;

typedef struct
{
	Point LefttopPoint;
	int width;
	int height;
}Rect;

int MaxIndex(double* array, int length);

double* Normalize(double* Array, int length);

double Norm(double* p, double* q, int length);

#endif