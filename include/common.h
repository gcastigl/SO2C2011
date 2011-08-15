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
extern int errno;

#define FALSE	0
#define TRUE	!FALSE
#define ERROR	-1;

// Plane Messages
#define PLANE_IS_CITY_BUSY	1

// Airline Messages
#define AIRLINE_YES			"1"
#define AIRLINE_NO			"0"

typedef struct {
	int id;
	int amount;
} Item;

typedef struct {
	int id;
	char message[1024];
} ipcPackage;

#define PACKAGE_SIZE sizeof(ipcPackage)

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
