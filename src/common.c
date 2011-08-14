#include "../include/common.h"

int** createIntMatrix(int rows, int columns) {
	int i;
	int** matrix = (int**) malloc(sizeof(int*) * rows);
	for (i = 0; i < rows; i++) {
		matrix[i] = malloc(sizeof(int) * columns); 
	}
	return matrix;
}

void fatal(char* err) {
	perror(err);
	exit(1);
}

