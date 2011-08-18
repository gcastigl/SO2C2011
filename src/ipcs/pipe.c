#include "../../include/pipe.h"

/*Private functions, don't use outside pipe.c unless certain 
of what you are doing*/
void closePipes(int** pipes, int channel, int totalPipes);
int initPipes(int ammount);
int getFd(int index, int channel);

//Idea: Call ipcSetup for each child
int ipcSetup(int ammount) {
    return initPipes(ammount);
}

int ipcSetupChild(int index) {
    int **temp;
    temp = wrPipes;
    wrPipes = rdPipes;
    rdPipes = temp;
    close(rdPipes[index][WRITE]);
    close(wrPipes[index][READ]);
    return NO_ERROR;
}

int ipcPostChildSetup(int ammount) {
    int i;
    for (i = 0; i < ammount; i++) {
        close(rdPipes[i][WRITE]);
        close(wrPipes[i][READ]);
    }
    return TRUE;
}

int ipcIsReady(int index, int channel) {
    int fd = getFd(index, channel);
    int rc;
    fd_set fds;
    struct timeval tv;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    tv.tv_sec = tv.tv_usec = 0;
    
    if (channel == READ) {
        rc = select(fd+1, &fds, NULL, NULL, &tv);
    } else if (channel == WRITE) {
        rc = select(fd+1, NULL, &fds, NULL, &tv);
    }
    if (rc < 0) {
        return -1;
    }
    return FD_ISSET(fd, &fds) ? TRUE : FALSE;
}

int initPipes(int ammount) {
    int i;
    // Initialize rdPipes
    rdPipes = malloc(ammount * sizeof(int*));
    wrPipes = malloc(ammount * sizeof(int*));
    for (i = 0; i < ammount; i++) {
        rdPipes[i] = malloc(2 * sizeof(int));
        wrPipes[i] = malloc(2 * sizeof(int));
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
    fflush(stdout);
    ipcPackage data;
    read(rdPipes[index][READ], &data, PACKAGE_SIZE);
    return data;
}

int getFd(int index, int channel) {
    int fd;
    switch (channel) {
        case READ:
            fd = rdPipes[index][READ];
            break;
        
        case WRITE:
            fd = wrPipes[index][WRITE];
            break;
        
        default:
            fd = ERROR;
    }
    return fd;
}