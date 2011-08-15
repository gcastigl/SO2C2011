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
