#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include "semaphore.h"
#include "mathUtil.h"

extern int errno;

// Semaphore keys
#define SEM_PLANE_KEY 	50
#define SEM_COMPANY_KEY 51

// Status defines
#define OK		0
#define ERROR	-1

typedef enum {FALSE = 0, TRUE = 1} bool;

void fatal(char* err);

int** createIntMatrix(int rows, int columns);

float** createFloatMatrix(int rows, int columns);

void printMatrix(int** matrix, int rows, int columns);

#endif
