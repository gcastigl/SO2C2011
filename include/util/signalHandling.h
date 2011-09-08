#ifndef SIGNALHANDLING_H_
#define SIGNALHANDLING_H_

#include <unistd.h>
#include "map.h"
#include "common.h"
#include "util/logger.h"
#include <pthread.h>
#include <signal.h>
#include "view/mapview.h"

void signal_setProcessCount(int count);
void signal_handler(int signal);
void signal_childHandler(int signal);
void signal_endChildProcess(int sigVal);
void signal_abortSimulation(int sigVal);
void signal_createHandlerThread(int isMainProcess);
void *signal_threadHandler(void* isMainProcess);

#endif
