#ifndef _COMMON_
#define _COMMON_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include "ipc.h"
extern int errno;

#define FALSE	0
#define TRUE	!FALSE
#define ERROR	-1
#define NO_ERROR 0	//FIXME: NO_ERROR shouldn't be OK?

typedef struct {
	int id;
	int amount;
} Item;

typedef struct {
	char** supplyName;
	char** companyName;
	char** cityName;
} Names;

void fatal(char* err);

int** createIntMatrix(int rows, int columns);

float** createFloatMatrix(int rows, int columns);

void printMatrix(float** matrix, int rows, int columns);

#endif
