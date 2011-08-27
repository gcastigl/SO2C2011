#ifndef COMMON_H_
#define COMMON_H_

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

 //TODO: this is for testing while the IPC are being programmed
typedef struct {
	int counter;
	pthread_mutex_t mutexCounter;
	pthread_mutex_t finished_thinking_mutex;
	int bufferSize;
	int* buffer;
} FakeIPC;

void fatal(char* err);

int** createIntMatrix(int rows, int columns);

float** createFloatMatrix(int rows, int columns);

void printMatrix(int** matrix, int rows, int columns);

#endif
