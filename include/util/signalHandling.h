#ifndef SIGNALHANDLING_H_
#define SIGNALHANDLING_H_

#include <signal.h>
#include <unistd.h>
#include <sys/signal.h>
#include <signal.h>
#include "map.h"
#include "common.h"
#include "util/logger.h"

void signal_handler(int signal);
void signal_childHandler(int signal);
void signal_endChildProcess(int sigVal);
void signal_endSimulation(int sigVal);
void signal_createHandlerThread(int isMainProcess);
void *signal_threadHandler(void* isMainProcess);

#endif
