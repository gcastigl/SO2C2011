#ifndef _PIPE_
#define _PIPE_

#include "ipc.h"

#define READ	0
#define WRITE	1

int **rdPipes, **wrPipes;

int initPipes(int ammount);

#endif
