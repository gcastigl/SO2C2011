#include "../include/main.h"

int main() {
	Airline* airline = createAirline(12345678, 3);
	airlineProcess(airline);
	printf("%ld\n", airline->id);
}



