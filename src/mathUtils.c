#include "../include/mathUtil.h"

int min(int n1, int n2) {
	return n1 < n2 ? n1 : n2;
}

int randomInt(int min, int max) {
	int mod = max - min + 1;
	return (rand() % mod) + min;
}
