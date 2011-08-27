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
#include <math.h>
#include "ipc.h"
#include "semaphore.h"
#include "mathUtil.h"
extern int errno;

#define OK		0
#define ERROR	-1

typedef enum {FALSE = 0, TRUE = 1} bool;

void fatal(char* err);

int** createIntMatrix(int rows, int columns);

float** createFloatMatrix(int rows, int columns);

void printMatrix(int** matrix, int rows, int columns);

#endif
