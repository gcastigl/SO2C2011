#ifndef _COMMON_
#define _COMMON_

#include <stdlib.h>
#include <stdio.h>

#define FALSE	0
#define TRUE	!FALSE
#define ERROR	-1;

// Do NOT change this values!
#define READ	0
#define WRITE	1

// Plane Messages
#define PLANE_IS_CITY_BUSY	1

// Airline Messages
#define AIRLINE_YES			"1"
#define AIRLINE_NO			"0"

typedef struct {
	int id;
	int amount;
} Item;

void fatal(char* err);

int** createIntMatrix(int rows, int columns);

float** createFloatMatrix(int rows, int columns);

void printMatrix(float** matrix, int rows, int columns);

#endif
