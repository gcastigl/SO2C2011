#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <signal.h>
#include <unistd.h>
#include "map.h"
#include "common.h"
#include "util/logger.h"

void initSignalHandler();
void initChildSignalHandler();
void signalHandler(int signal);
void childSignalHandler(int signal);
void finishProcess(int sigVal);
void finishMainProcess(int sigVal);

#endif
