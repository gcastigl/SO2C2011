#include "communicator.h"

typedef struct {
	long fromId;
	char data[DATA_SIZE];
} MsgQueuePackage;

#define MSG_SIZE (sizeof(MsgQueuePackage) - sizeof(long int))

int ipc_get(int id);

MsgQueuePackage *newMsgQueuePackage(int id, char* data);

void ipc_init(int id) {
	// TODO: ???
}

int ipc_get(int key) {
	key_t ipcId;
	ipcId = ftok(TMP_FOLDER, key);
	if (ipcId == (key_t)-1) {
		fatal("Error writing msg - ftok");
		return -1;
	}
	int id = msgget(ipcId, IPC_CREAT | FLAGS);
	if (id < 0) {
		fatal("Error writing msg - msgget");
		return -1;
	}
	return id;
}

int ipc_write(int myId, int toId, char *data) {
	int fromIpcId = ipc_get(myId);
	MsgQueuePackage *msg = newMsgQueuePackage(myId, data);
	return msgsnd(fromIpcId, (void*) msg, MSG_SIZE, IPC_NOWAIT);
}

int ipc_read(int myId, int fromId, char *data) {
	int toIdIpc = ipc_get(myId);
	MsgQueuePackage msg;
	int result = msgrcv(toIdIpc, (void *)&msg, MSG_SIZE, fromId, IPC_NOWAIT);
	if (result == -1) {
		return -1;
	}
	strcpy(data, msg.data);
	return result;
}

int ipc_close(int id){
	int ipcId = ipc_get(id);
	return msgctl(ipcId, IPC_RMID, (struct msqid_ds *) NULL);
}

MsgQueuePackage *newMsgQueuePackage(int id, char* data) {
	MsgQueuePackage* msg = malloc(sizeof(MsgQueuePackage));
	msg->fromId = id;
	strcpy(msg->data, data);
	return msg;
}
