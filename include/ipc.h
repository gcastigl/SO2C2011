#ifndef IPC_H_
#define IPC_H_

#include "common.h"
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/select.h>
#define DATA_SIZE 64
#define READ 0
#define WRITE 1

typedef enum {
    CREATE,
	DESTROY,
	MESSAGE
} packageType;

/*
 * Estructura para los mensajes que se quieren enviar y/o recibir. Deben llevar
 * obligatoriamente como primer campo un long para indicar un identificador
 * del mensaje.
 * Los siguientes campos son la información que se quiera transmitir en el
 * mensaje.
 */
typedef struct {
	long id;
	int numericDataType;
	char data[DATA_SIZE];
} IpcPackage;

#define PACKAGE_SIZE sizeof(IpcPackage)

int ipcSetup(int ammount);
int ipcSetupChild(int index);
int ipcPostChildSetup(int ammount);
int ipcClean();
int ipcIsReady(int index, int channel);
IpcPackage getData(int childIndex);
void sendData(int childIndex, IpcPackage data);

int childIndex;

#endif
