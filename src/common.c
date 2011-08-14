#include "../include/common.h"

int** createIntMatrix(int rows, int columns) {
	int i;
	int** matrix = (int**) malloc(sizeof(int*) * rows);
	for (i = 0; i < rows; i++) {
		matrix[i] = malloc(sizeof(int) * columns); 
	}
	return matrix;
}

float** createFloatMatrix(int rows, int columns) {
	int i;
	float** matrix = (float**) malloc(sizeof(float*) * rows);
	for (i = 0; i < rows; i++) {
		matrix[i] = malloc(sizeof(float) * columns); 
	}
	return matrix;
}

void fatal(char* err) {
	perror(err);
	exit(1);
}

void printMatrix(float** matrix, int rows, int columns) {
	int i, j;
	printf("\t");
	for (j = 0; j < columns; j++) {
		printf("%d       ", j + 1);
	}
	printf("\n");
	for (i = 0; i < rows; i++) {
		printf("%d\t", i + 1);
		for (j = 0; j < columns; j++) {
			if (matrix[i][j] < 10) 
				printf("%.3f   ", matrix[i][j]);
			else if (matrix[i][j] < 100)
				printf("%.3f  ", matrix[i][j]);
		}
		printf("\n");
	}
}

