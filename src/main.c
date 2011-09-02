#include "main.h"
#include "parser.h"

int main() {
    //initSignalHandler();
    parseMap("resources/loads/ciudades.txt");
    map_addCompany(parseCompany("resources/loads/empresa.txt", 123456));
    companyStart(map->company[0]);
/*    for (int i = 0; i < numAirlines; i++) {
        switch(fork()) {
            case 0:
                initChildSignalHandler();
                log_debug("Creating airline...\n");
                log_error("Bleh\n");
                companyStart(map->company[0]);
                break;
            case ERROR:
                fatal("Fork Error");
                break;
            default:
                wait(NULL);
                break;
        }
    }*/
    printf("Simulation Done!\n");
	return 0;
}



