#include "communicator.h"

int ipc_init(int key, int flags) {
	key_t ipdId;
	ipdId = ftok(DEFAULT_FOLDER, key);
	if (ipdId == (key_t)-1) {
		return -1;
	}
	int id = msgget(ipdId, flags | IPC_CREAT);
	if (id == -1) {
		return -1;
	}
	return id;
}

int ipc_write(int ipcId, IpcPackage* msg) {
	msg->id = 0;
	return msgsnd(ipcId, (struct msgbuf *) msg,
		sizeof(msg->numericDataType) + sizeof(msg->data), IPC_NOWAIT);
}

// - Identificador del tipo de mensaje que queremos recibir. En este caso
// se quiere un mensaje de tipo 2. Si ponemos tipo 1, se extrae el mensaje
// que se acaba de enviar en la llamada anterior a msgsnd().
IpcPackage* ipc_get(int ipcId) {
	IpcPackage* msg = malloc(sizeof(IpcPackage));
	int result = msgrcv(ipcId, (struct msgbuf *) msg,
		sizeof(msg->numericDataType) + sizeof(msg->data),
		2, IPC_NOWAIT);
	if (result <= 0) {
		return NULL;
	}
	return msg;
}

void ipc_close(int ipcId) {
	msgctl(ipcId, IPC_RMID, (struct msqid_ds *) NULL);
}
