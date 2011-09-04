#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <signal.h>
#include <unistd.h>
#include <sys/signal.h>
#include <signal.h>
#include "map.h"
#include "common.h"
#include "util/logger.h"

void initSignalHandler();
void signalHandler(int signal);
void childSignalHandler(int signal);
void finishProcess(int sigVal);
void finishMainProcess(int sigVal);
void createSignalHandlingThread();
void *sig_threadHandler(void* args);

#endif
