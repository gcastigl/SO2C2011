#ifndef _IPC_H_
#define _IPC_H_

#include "common.h"

#define DATA_SIZE 2048
#define READ 0
#define WRITE 1

typedef enum {
    CREATE,
	DESTROY,
	MESSAGE
} packageType;

typedef struct {
	int id;
	char data[DATA_SIZE];
} ipcPackage;

#define PACKAGE_SIZE sizeof(ipcPackage)

int ipcSetup(int ammount);
int ipcSetupChild(int index);
int ipcPostChildSetup(int ammount);
int ipcClean();
int ipcIsReady(int index, int channel);
ipcPackage getData(int childIndex);
void sendData(int childIndex, ipcPackage data);

int childIndex;

#endif
