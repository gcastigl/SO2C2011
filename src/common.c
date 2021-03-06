#include "common.h"

void** createMatrix(int elemSize, int rows, int columns);

int** createIntMatrix(int rows, int columns) {
	return (int**) createMatrix(sizeof(int), rows, columns);
}

float** createFloatMatrix(int rows, int columns) {
	int i;
	float** matrix = (float**) malloc(sizeof(float*) * rows);
	for (i = 0; i < rows; i++) {
		matrix[i] = malloc(sizeof(float) * columns); 
	}
	return matrix;
}

void** createMatrix(int elemSize, int rows, int columns) {
	int i;
	char** aux;
	void** matrix = malloc(elemSize * rows);
	aux = (char**) matrix;
	for (i = 0; i < rows; i++) {
		aux[i] = malloc(elemSize * columns);
	}
	return matrix;
}

void fatal(char* err) {
    log_error(err);
    log_debug("isChild %s", isChild == TRUE ? "Yes" : "No");
	if (isChild == TRUE) {
        kill(getppid(), SIGINT);
    } else {
        kill(getpid(), SIGINT);
    }
	exit(1);
}

void printMatrix(int** matrix, int rows, int columns) {
	int i, j;
	printf("\t");
	for (j = 0; j < columns; j++) {
		printf("%3d\t", j);
	}
	printf("\n---\t|");
	for (j = 0; j < columns; j++) {
		printf("---\t");
	}
	printf("\n");
	for (i = 0; i < rows; i++) {
		printf("%3d\t|", i);
		for (j = 0; j < columns; j++) {
			if (matrix[i][j] < 10) 
				printf("%3d\t", matrix[i][j]);
			else if (matrix[i][j] < 100)
				printf("%3d\t", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

