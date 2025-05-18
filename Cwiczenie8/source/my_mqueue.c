#include "../include/my_mqueue.h"

int my_mq_open(mqd_t *mqdes, const char *name, int oflag, mode_t mode, struct mq_attr *attr) {
    mqd_t desc;

    if(oflag & O_CREAT)
        desc = mq_open(name, oflag, mode, attr);
    else
        desc = mq_open(name, oflag);

    if(desc == (mqd_t)-1) {
        perror("my_mq_open");
        return 0;
    }

    *mqdes = desc;
    return 1;
}

int my_mq_close(mqd_t mqdes) {
    if(mq_close(mqdes) == -1) {
        perror("my_mq_close");
        return 0;
    }
    return 1;
}

int my_mq_unlink(const char *name) {
    if(mq_unlink(name) == -1) {
        perror("my_mq_unlink");
        return 0;
    }
    return 1;
}

int my_mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio) {
    if(mq_send(mqdes, msg_ptr, msg_len, msg_prio) == -1) {
        perror("my_mq_send");
        return 0;
    }
    return 1;
}

int my_mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio) {
    if(mq_receive(mqdes, msg_ptr, msg_len, msg_prio) == -1) {
        perror("my_mq_receive");
        return 0;
    }
    return 1;
}

int my_mq_getattr(mqd_t mqdes, struct mq_attr *mqstat) {
    if(mq_getattr(mqdes, mqstat) == -1) {
        perror("my_mq_getattr");
        return 0;
    }
    return 1;
}

int my_mq_setattr(mqd_t mqdes, struct mq_attr *mqstat, struct mq_attr *omqstat) {
    if(mq_setattr(mqdes, mqstat, omqstat) == -1) {
        perror("my_mq_setattr");
        return 0;
    }
    return 1;
}