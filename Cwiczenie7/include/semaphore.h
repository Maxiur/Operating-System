#ifdef SEMAPHORE_H
#define SEMAPHORE_H

#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

sem_t *create_semaphore(const char *name, unsigned int value);
sem_t *open_semaphore(const char *name);
int get_semaphore_value(sem_t *sem, unsigned int *value);
int wait_semaphore(sem_t *sem);
int post_semaphore(sem_t *sem);
int close_semaphore(sem_t *sem);
int unlink_semaphore(const char *name);
sem_wait_status wait_semaphore_timeout(sem_t *sem, unsigned int timeout);

#endif // SEMAPHORE_H
