#ifndef COMPANYLOGIC_H_
#define COMPANYLOGIC_H_

#include "../common.h"
#include "../map.h"
#include "../plane.h"
#include "../ipc.h"
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include "../semaphore.h"

void companyStart(Company* company);


#endif
