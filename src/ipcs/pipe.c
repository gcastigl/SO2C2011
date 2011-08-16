#include "../../include/pipe.h"

int ipcSetup(int childs) {
    initPipes(childs);
}

int initPipes(int ammount) {
    int i;
    rdPipes = malloc(ammount * sizeof(int) * 2);
    wrPipes = malloc(ammount * sizeof(int) * 2);
    for (i = 0; i < ammount; i++) {
        if (pipe(rdPipes[i]) == ERROR || pipe(wrPipes[i]) == ERROR) {
            return ERROR;
        }
    }
}

void sendData(int childIndex, ipcPackage data) {
    write(wrPipes[i][WRITE], data, PACKAGE_SIZE);
}

ipcPackage getData(int childIndex) {
   ipcPackage data;
   read(rdPipes[i][READ], &data, PACKAGE_SIZE);
   return data;
}
