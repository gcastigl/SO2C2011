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
	perror(err);
	exit(1);
}

void printMatrix(int** matrix, int rows, int columns) {
	int i, j;
	printf("\t");
	for (j = 0; j < columns; j++) {
		printf("%d    ", j + 1);
	}
	printf("\n");
	for (i = 0; i < rows; i++) {
		printf("%d\t", i + 1);
		for (j = 0; j < columns; j++) {
			if (matrix[i][j] < 10) 
				printf("%d    ", matrix[i][j]);
			else if (matrix[i][j] < 100)
				printf("%d   ", matrix[i][j]);
		}
		printf("\n");
	}
}
