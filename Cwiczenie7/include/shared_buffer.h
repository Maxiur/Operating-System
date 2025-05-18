#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#define NELE 60
#define NBUF 5

typedef struct {
    char element[NELE];
} Towar; // jednoska towaru (danych)

typedef struct {
    Towar bufor[NBUF];
    int wstaw;
    int wyjmij;
} SegmentPD;

#define SEM_WRITE_TO_SHM "/SEM_WRITE_TO_SHM"
#define SEM_READ_FROM_SHM "/SEM_READ_FROM_SHM"

#define SHM_NAME "/sharred_buffer"
#define SHM_SIZE sizeof(SegmentPD)

#endif // SHARED_BUFFER_H