#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#define NELE 20
#define NBUF 5

typedef struct {
    char element[NELE];
} Towar; // jednoska towaru (danych)

typedef struct {
    Towar bufor[NBUF];
    int wstaw;
    int wyjmij;
} SegmentPD;

#define ILE_TOWAROW 10

#define SEM_WRITE_TO_SHM "/SEM_WRITE_TO_SHM"
#define SEM_READ_FROM_SHM "/SEM_READ_FROM_SHM"
#define SHM_NAME "/sharred_buffer"
#define SEM_NAME "my_semaphore"
#define SHM_SIZE sizeof(SegmentPD)

#endif // SHARED_BUFFER_H