#ifndef MY_MQUEUE_H
#define MY_MQUEUE_H

#include <mqueue.h>

struct mq_attr {
    long mq_flags;    // opcje (np. O_NONBLOCK)
    long mq_maxmsg;   // maks. liczba komunikatów w kolejce
    long mq_msgsize;  // maks. rozmiar pojedynczego komunikatu (w bajtach)
    long mq_curmsgs;  // liczba komunikatów aktualnie w kolejce
};

// Tworzenie kolejki komunikatów
int my_mq_open(mqd_t *mqdes, const char *name, int oflag, mode_t mode, struct mq_attr *attr);

// Zamknięcie kolejki komunikatów
int my_mq_close(mqd_t mqdes);

// Usunięcie kolejki komunikatów
int my_mq_unlink(const char *name);

// Wysyłanie komunikatu do kolejki
int my_mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);

// Odbieranie komunikatu z kolejki
int my_mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);

// Sprawdzenie atrybutów kolejki
int my_mq_getattr(mqd_t mqdes, struct mq_attr *mqstat);

// Ustawienie atrybutów kolejki
int my_mq_setattr(mqd_t mqdes, struct mq_attr *mqstat, struct mq_attr *omqstat);

#endif //MY_MQUEUE_H
