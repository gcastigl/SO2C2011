#ifndef _COMMON_
#define _COMMON_

#include <stdlib.h>
#include <stdio.h>

#define FALSE	0
#define TRUE	!FALSE

typedef struct {
	char* name;
	int amount;
	int id;
} Item;

void fatal(char* err);

int** createIntMatrix(int rows, int columns);

float** createFloatMatrix(int rows, int columns);

void printMatrix(float** matrix, int rows, int columns);

#endif
