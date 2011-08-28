#include "../include/main.h"
#include "../include/parser.h"

int main() {
    initSignalHandler();
    int numAirlines = 1;
    int i;
    parseMap("resources/loads/ciudades.txt");
    for (i = 0; i < numAirlines; i++) {
        switch(fork()) {
            case 0:
                initChildSignalHandler();
                printf("Creating airline...\n");
                Company* company = newCompany("pepe", 2);
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



