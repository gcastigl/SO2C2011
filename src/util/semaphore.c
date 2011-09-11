#include "util/semaphore.h"

/*
 * Gets the semaphore for the key specified and initializes
 * all places with 0.
 */
sem_t* semaphore_create(char *name) {
    sem_t *sem;
    sem_unlink(name);
    sem = sem_open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    if (sem == SEM_FAILED) {
        if (errno == EEXIST) {
            sem_unlink(name);
            sem = sem_open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
        }
    }
    if (sem == SEM_FAILED) {
        fatal("Error creating semaphore");
    }
    return sem;
}

sem_t *sem_get(char *name) {
    sem_t* sem = sem_open(name, 0, S_IRUSR | S_IWUSR);
    if (sem == SEM_FAILED) {
        log_debug("error getting semaphore %s", name);
        fatal("error getting semaphore");
    }
    return sem;
}
