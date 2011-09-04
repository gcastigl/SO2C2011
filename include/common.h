#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include "mathUtil.h"
#include "util/logger.h"

extern int errno;

#define TMP_FOLDER	"/tmp/"
// Semaphore keys
#define SEM_PLANE_KEY 	10220
#define SEM_COMPANY_KEY	20130
#define IPC_KEY			0x12310

// Status defines
#define OP_OK	0
#define ERROR	-1

#define FALSE 0
#define TRUE 1

// Base Ids
#define MIN_PLANE_ID	100
#define MIN_COMPANY_ID	1000

int *childPid;

void fatal(char* err);

int **createIntMatrix(int rows, int columns);

float **createFloatMatrix(int rows, int columns);

void printMatrix(int** matrix, int rows, int columns);

char *mstrcpy(char **to, char *from);

#endif
