#include<stdio.h>
#include <stdlib.h> 
#include <math.h>
#include "MathFunc.h"

int MaxIndex(double* array, int length){
	int i;
	int max;
	int max_sign;

	max = array[0];
	max_sign = 0;

	for (i = 1; i < length; i++)
	{
		if (array[i] > max)
		{
			max = array[i];
			max_sign = i;
		}
	}

	return max_sign;
}

double* Normalize(double* Array, int length){
	double* NormalizedArray;
	double sum;
	int i;

	NormalizedArray = (double*)malloc(sizeof(double) * length);
	
	sum = 0.0;

	for (i = 0; i < length; i++){
		sum = sum + Array[i];
	}

	for (i = 0; i < length; i++){
		NormalizedArray[i] = Array[i] / sum;
	}

	return NormalizedArray;
}

double Norm(double* p, double* q, int length){
	double Answer;
	double sum;
	int i;

	sum = 0;
	for (i = 0; i < length; i++){
		sum = sum + pow(p[i] - q[i], 2);
	}

	Answer = pow(sum, 0.5);

	return Answer;
}