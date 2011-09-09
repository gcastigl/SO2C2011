#ifndef COMPANYLOGIC_H_
#define COMPANYLOGIC_H_

#include "common.h"
#include "map.h"
#include "plane.h"
#include "planeLogic.h"
#include "communicator.h"
#include "semaphore.h"
#include "serializer.h"
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void companyStart(Map* initialMap, Company* cmp);

typedef struct {
    int amount;
    CityUpdatePackage **update;
} ItemChange;

#endif
