#include "ipcs/socket.h"

#define IPC_SOCKET_DIR "./sosocket/"
#define ADDR_SIZE sizeof(struct sockaddr_un)

typedef struct {
	int sockfd;
	struct sockaddr_un *addr;
} socket_t;

static int mySockFd;

socket_t *getSocket(int id1, int id2, int read) {
    if (mySockFd != 0 && read == TRUE) {
        return NULL;
    }
	socket_t *sock = (socket_t*) malloc(sizeof(socket_t));
	char path[MAX_NAME_LENGTH];
	sprintf(path, "%s%d_%d", IPC_SOCKET_DIR, id1, id2);
	sock->sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	sock->addr = (struct sockaddr_un *)malloc(sizeof(struct sockaddr_un));
	sock->addr->sun_family = AF_UNIX;
	strcpy(sock->addr->sun_path, path);
	int ret = bind(sock->sockfd, (struct sockaddr *)sock->addr, ADDR_SIZE);
	if ((read == TRUE)) {
	    if (ret >= 0) {
            mySockFd = sock->sockfd;   
	    }
	}
	int flags = fcntl(sock->sockfd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(sock->sockfd, F_SETFL, flags);
	return sock;
}

int ipc_init(int myId, int size) {
    mySockFd = 0;
	return mkdir(IPC_SOCKET_DIR, 0777);
}

int ipc_write(int myId, int toId, char *msg) {
	socket_t *socket = getSocket(myId, toId, FALSE);
	int ret = sendto(socket->sockfd, msg, DATA_SIZE, 0 , (struct sockaddr *)socket->addr, ADDR_SIZE);
	if (ret < 0) {
        log_warning("Error: %s", strerror(errno));
	}
    return ret;
}

int ipc_read(int myId, int fromId, char *msg) {
	getSocket(fromId, myId, TRUE);
	int n = recvfrom(mySockFd, msg, DATA_SIZE, 0, NULL, NULL);
	if (n==-1) {
		switch (errno) {
		case EBADF:
			//The socket argument is not a valid file descriptor.
			break;
		case ENOTSOCK:
			//The descriptor socket is not a socket.
			break;
		case EWOULDBLOCK:
			//Nonblocking mode has been set on the socket, and the read operation would block.
		 	//(Normally, recv blocks until there is input available to be read.)
			break;
		case EINTR:
			//The operation was interrupted by a signal before any data was read. .
			break;
		case ENOTCONN:
			//You never connected this socket.
			break;
		}
	}
    return n;
}

int ipc_close(int id) {
	char *fileName = malloc(strlen(IPC_SOCKET_DIR) + 50 * sizeof(char));
    sprintf(fileName, "%s%d_%d", IPC_SOCKET_DIR, serverId, id);
	unlink(fileName);
	sprintf(fileName, "%s%d_%d", IPC_SOCKET_DIR, id, serverId);
	unlink(fileName);
	free(fileName);
	rmdir(IPC_SOCKET_DIR);
    return 0;
}
