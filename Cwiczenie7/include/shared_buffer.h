#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#define NELE 20
#define NBUF 5

typedef struct {
    int buffer[NBUF][NELE];
    int wstaw;
    int wyjmij;
} SegmentPD;

#define SEM_WRITE_TO_SHM "/SEM_WRITE_TO_SHM"
#define SEM_READ_FROM_SHM "/SEM_READ_FROM_SHM"
#define SHM_NAME "/sharred_buffer"
#define SEM_NAME "my_semaphore"

#endif // SHARED_BUFFER_H