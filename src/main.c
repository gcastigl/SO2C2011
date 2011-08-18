#include "../include/main.h"

int main() {
    int numAirlines = 1;
    int i;
    for (i = 0; i < numAirlines; i++) {
        switch(fork()) {
            case 0:
                printf("Creating airline...\n");
                Airline* airline = createAirline(12345678, 3);
                airlineStart(airline);
                break;

            case ERROR:
                fatal("Fork Error");
                break;
            default:
                wait(NULL);
                break;
        }
    }
    printf("Simulation Done!\n");
	return 0;
}



