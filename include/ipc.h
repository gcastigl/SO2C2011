#ifndef _IPC_
#define _IPC_

#include "common.h"

typedef enum {
    CREATE,
	DESTROY,
	MESSAGE
} packageType;

int ipcSetup();
int ipcClean();
ipcPackage getData(int childIndex);
void sendData(int childIndex, ipcPackage data);

#endif
