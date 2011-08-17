#include "../../include/pipe.h"

/*Private functions, don't use outside pipe.c unless certain 
of what you are doing*/
void closePipes(int** pipes, int channel, int totalPipes);
int initPipes(int ammount);

//Idea: Call ipcSetup for each child
int ipcSetup(int ammount) {
    return initPipes(ammount);
}

int initPipes(int ammount) {
    int i;
    // Initialize rdPipes
    rdPipes = malloc(ammount * sizeof(int) * 2);
    wrPipes = malloc(ammount * sizeof(int) * 2);
    
    for (i = 0; i < ammount; i++) {
        if (pipe(rdPipes[i]) == ERROR || pipe(wrPipes[i]) == ERROR) {
            return ERROR;
        }
    }
    
    return NO_ERROR;
}

void closePipes(int** pipes, int channel, int totalPipes) {
    int i;
    for (i = 0; i < totalPipes; ++i) {
        close(pipes[i][channel]);
    }
}

void sendData(int index, ipcPackage data) {
    write(wrPipes[index][WRITE], &data, PACKAGE_SIZE);
}

ipcPackage getData(int index) {
    ipcPackage data;
    read(rdPipes[index][READ], &data, PACKAGE_SIZE);
    return data;
}
