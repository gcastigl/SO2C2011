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
#include "util/logger.h"

extern int errno;

#define DEFAULT_FOLDER	"/tmp/"
// Semaphore keys
#define SEM_PLANE_KEY 	0x8512154
#define SEM_COMPANY_KEY 0x1233468

// Status defines
#define OP_OK	0
#define ERROR	-1

#define FALSE 0
#define TRUE 1

void fatal(char* err);

int** createIntMatrix(int rows, int columns);

float** createFloatMatrix(int rows, int columns);

void printMatrix(int** matrix, int rows, int columns);

#endif
