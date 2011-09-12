#include "ipcs/sharedMemory.h"

#define MEM_KEY ((key_t) 0x712011)

int shmid;
char *memory;
int count;


#define SHM_LIST_SIZE (30 * DATA_SIZE * sizeof(char) + sizeof(char))
#define GET(X, Y) (memory + (count * SHM_LIST_SIZE * X + Y * SHM_LIST_SIZE))

int ipc_init(int myId, int size) {
	count = size;
	int memsize = size * size * SHM_LIST_SIZE;
	errno = 0;
	if ((shmid = shmget(MEM_KEY, memsize, IPC_CREAT | IPC_EXCL | 0666)) == -1){
		if (errno == EEXIST) {
			if ((shmid = shmget(MEM_KEY, memsize, IPC_CREAT | 0600)) == -1) {
                fatal("Can't allocate memory");
			}
			errno = 0;
		}
	}
    errno = 0;
	memory = (char*)shmat(shmid, NULL, 0);
	if (memory == (void*)-1) {
        log_error("Couldn't bla bla bla memory: %s", strerror(errno));
        fatal("");
	}
	for (int i = 0; i < memsize; ++i) {
		memory[i] = '\0';
	}
	return 1;
}

int msg_push(int from, int to, char *msg) {
	char *message = GET(from, to);
    char *origMessage = message;
    char messageCount;
    memcpy(&messageCount, origMessage, sizeof(char));
	message += sizeof(char);
	if (messageCount >= 0) {
		if(messageCount > 30) {
			log_error("Could not save package. messageCount is 30");
			return -1;
		} else {
			message += ((messageCount == 0 ? 0 : messageCount - 1) * DATA_SIZE * sizeof(char));
			memcpy(message, msg, DATA_SIZE);

			messageCount++;
            memcpy(origMessage, &messageCount, sizeof(char));

            return DATA_SIZE;
		}
	}
	return -1;
}

int msg_pop(int from, int to, char *msg) {
	char *message = GET(from, to);
	char *origMessage = message;
	char messageCount;
    memcpy(&messageCount, origMessage, sizeof(char));
    message += sizeof(char);
	if (messageCount > 0) {
		message += ((messageCount - 1) * DATA_SIZE * sizeof(char));
		memcpy(msg, message, DATA_SIZE);

		messageCount--;
        memcpy(origMessage, &messageCount, sizeof(char));
		
		return DATA_SIZE;
	}
	return -1;
}

int ipc_write(int myId, int toId, char *msg) {
	return msg_push(myId, toId, msg);
}

int ipc_read(int myId, int fromId, char *msg) {
	return msg_pop(fromId, myId, msg);
}

int ipc_close(int id) {
    shmdt(memory);
    shmctl(shmid, IPC_RMID, 0);
	return 1;
}
