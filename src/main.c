#include "main.h"
#include "parser.h"

int main() {
    initSignalHandler();
    int numAirlines = 1;
    int i;
    parseMap("resources/loads/ciudades.txt");
    //Company *company = parseCompany("resources/loads/empresa.txt");
    //map_addCompany(*company);
    for (i = 0; i < numAirlines; i++) {
        switch(fork()) {
            case 0:
                initChildSignalHandler();
                log_debug("Creating airline...\n");
                log_error("Bleh\n");
                Company* company = newCompany("pepe", 3);
                for( int i = 0; i < 3; i++) {
                	addPlane(company, *newPlane(i, 0));
                }
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



