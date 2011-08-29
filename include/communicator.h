#ifndef MYIPC_H_
#define MYIPC_H_

#include "ipc.h"
#include <sys/msg.h>
#include <errno.h>

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

int ipc_init(int key, int flags);

int ipc_write(int ipcId, IpcPackage* msg);

IpcPackage* ipc_get(int ipcId);

void ipc_close(int ipcId);

#endif

