#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#define NELE 20
#define NBUF 5

typedef struct {
    int buffer[NBUF][NELE];
    int in;
    int out;
} SegmentPD;

#endif // SHARED_BUFFER_H