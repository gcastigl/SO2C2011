#ifndef _IPC_
#define _IPC_

#include "common.h"

#define PACKAGE_SIZE sizeof(ipcPackage)
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

int childIndex;

int ipcSetup(int ammount);
int ipcSetupChild(int index);
int ipcClean();
ipcPackage getData(int childIndex);
void sendData(int childIndex, ipcPackage data);

#endif
