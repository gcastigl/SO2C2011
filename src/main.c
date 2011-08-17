#include "../include/main.h"

int main() {
    int numAirlines = 1;
    int i;
    for (i = 0; i < numAirlines; i++) {
        switch(fork()) {
            case 0:
                airlineStart(createAirline(12345678, 3));
                break;

            case ERROR:
                fatal("Fork Error");
                break;
        }
    }
	return 0;
}



