#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
#include "mathUtils.h"
#include "util/logger.h"

extern int errno;

#define MAX(x,y)	((x) > (y) ? (x) : (y))

#define TMP_FOLDER	"/tmp/"
//#define SERVER_IPC_KEY	12344

// Status defines
#define OP_OK	0
#define ERROR	-1

#define FALSE 	0
#define TRUE 	1

#define ON 1
#define OFF 0

#define BLOCK_SIZE	10
#define MAX_NAME_LENGTH 100

//
#define PLANE_ID(companyId, index)		((companyId << 16) + index)
#define PLANE_INDEX(planeId)			(planeId & 0xFFFF)
#define PLANE_COMPANY_ID(planeId)		(planeId >> 16)

#define S_WAIT(X) (sem_wait(sem_get(X)))
#define S_POST(X) (sem_post(sem_get(X)))
#define S_GETVAL(X, Y) (sem_getvalue(sem_get(X), Y))
int *childPid;
int isChild;

typedef struct {
	int id;
	int cityIdFrom;
	int cityIdTo;
	int distanceLeft;
	int itemCount;
	int *itemStock;
} Plane;

typedef struct {
	int id;
	char name[MAX_NAME_LENGTH];
	int planeCount;
	Plane **plane;
} Company;

void fatal(char* err);

int **createIntMatrix(int rows, int columns);

float **createFloatMatrix(int rows, int columns);

void printMatrix(int** matrix, int rows, int columns);

int serverId;

#endif
