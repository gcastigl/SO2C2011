#ifndef _IPC_
#define _IPC_

#include "common.h"

#define ERROR -1

typedef enum {
    CREATE,
	DESTROY,
	MESSAGE
} packageType;

int ipcSetup(int childs);
int ipcClean();
ipcPackage getData(int childIndex);
void sendData(int childIndex, ipcPackage data);

#endif
