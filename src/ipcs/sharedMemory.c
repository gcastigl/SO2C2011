#include "ipcs/sharedMemory.h"

#define MEM_KEY ((key_t) 0x111222)
#define MAX_PACKAGE_QUEUE 50
int shmid;
char *memory;
int count;


#define SHM_LIST_SIZE (MAX_PACKAGE_QUEUE * DATA_SIZE * sizeof(char) + sizeof(char))
#define GET(X, Y) (memory + (count * SHM_LIST_SIZE * X + Y * SHM_LIST_SIZE))

int ipc_init(int myId, int size) {
	count = size;
	size++;
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
        log_error("Couldn't memory: %s", strerror(errno));
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
		if(messageCount > MAX_PACKAGE_QUEUE) {
			log_error("Could not save package. Queue is full");
			return -1;
		} else {
			message += messageCount * DATA_SIZE * sizeof(char);
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
	//toId = toId % serverId;
	//myId = myId % serverId;
	return msg_push(myId, toId, msg);
}

int ipc_read(int myId, int fromId, char *msg) {
	//fromId = fromId % serverId;
	//myId = myId % serverId;
	return msg_pop(fromId, myId, msg);
}

int ipc_close(int id) {
    shmdt(memory);
    shmctl(shmid, IPC_RMID, 0);
	return 1;
}
