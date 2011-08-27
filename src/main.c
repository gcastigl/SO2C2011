#include "../include/main.h"

int main() {
    int numAirlines = 1;
    int i;
    map = newMap(1);
    for (i = 0; i < numAirlines; i++) {
        switch(fork()) {
            case 0:
                printf("Creating airline...\n");
                Company* company = newCompany('pepe', 2);
                companyStart(company);
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



