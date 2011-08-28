#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <signal.h>
#include <unistd.h>

void initSignalHandler();
void initChildSignalHandler();

#endif
