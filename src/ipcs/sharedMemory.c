#include "ipcs/sharedMemory.h"
#include <mm.h>

#define MEM_KEY ((key_t) 0x0303456)

typedef struct item {
	int from;
	int to;
	char data[DATA_SIZE];
	struct item *next;
} item_t;

int shmid;
item_t **array;
int count;

int ipc_init(int myId, int size) {
	MM_create()
	errno = 0;
	if ((shmid = shmget(MEM_KEY, (size*sizeof(item_t*)), IPC_CREAT | IPC_EXCL | 0666)) == -1){
		if (errno == EEXIST) {
			if ((shmid = shmget(MEM_KEY, (size*sizeof(item_t*)), IPC_CREAT | 0600)) == -1) {
				return -1;
			}
			errno = 0;
		}
	}
	array = (item_t**)shmat(shmid, NULL, 0);
	if (array == (item_t**)-1) {
		return -1;
	}
	count = size;
	for (int i = 0; i < size; ++i) {
		array[i] = NULL;
	}
	return 1;
}

int ipc_write(int myId, int toId, char *msg) {
	log_error("Printing %d ON: %p", myId, array);
	item_t *item = (item_t*)malloc(sizeof(item_t));
	item->from = myId;
	item->to = toId;
	memcpy(item->data, msg, DATA_SIZE);
	int empty = -1;
	for (int i = 0; i < count; ++i) {
		if (array[i] != NULL) {
			if (array[i]->from == myId) {
				item->next = array[i];
				array[i] = item;
				return DATA_SIZE;
			}
		} else if (empty == -1) {
			empty = i;
		}
	}
	item->next = array[empty];
	array[empty] = item;
	return DATA_SIZE;
}

int ipc_read(int myId, int fromId, char *msg) {
	for (int i = 0; i < count; ++i) {
		if (array[i] != NULL) {
			if (array[i]->from == myId) {
				item_t *parent = NULL;
				item_t *head = array[i];
				while (head != NULL) {
					if (head->to == fromId) {
						memcpy(msg, head->data, DATA_SIZE);
						if (parent == NULL) {
							array[i] = head->next;
						} else {
							parent->next = head->next;
						}
						return DATA_SIZE;
					}
					parent = head;
					head = parent->next;
				}

			}
		}
	}
	return -1;
}

int ipc_close(int id) {
    shmdt(array);
    shmctl(shmid, IPC_RMID, 0);
	return 1;
}
