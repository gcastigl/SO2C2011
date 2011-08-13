#include "../include/main.h"

int main() {
	Airline* airline = createAirline("Air France", 3);
	airlineProcess(airline);
	printf("%s\n", airline->name);
}



