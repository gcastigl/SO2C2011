#ifndef IPC_H_
#define IPC_H_

#include "common.h"
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/select.h>
#define DATA_SIZE 64
#define READ 0
#define WRITE 1

#define PACKAGE_SIZE sizeof(IpcPackage)

int childIndex;

#endif
