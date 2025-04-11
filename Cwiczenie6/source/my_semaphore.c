#include "../include/my_semaphore.h"

sem_t* my_sem_init(const char* name, int init_value) {
      sem_t* my_sem =  sem_open(name, O_CREAT | O_EXCL, 0644, init_value);
      if(my_sem == SEM_FAILED) {
        perror("Sem init (open) error");
        return NULL;
      }
      return my_sem;
}

sem_t* my_sem_open(const char* name) {
  sem_t* my_sem = sem_open(name, 0);
      if(my_sem == SEM_FAILED) {
        perror("Sem open error");
        return NULL;
      }
      return my_sem;
}

int my_sem_close(sem_t* sem) {
  int my_sem = sem_close(sem);
  if(my_sem == -1) {
    perror("Sem close error");
    return 0;
  }
  return 1;
}

int my_sem_unlink(const char* name) {
  if(sem_unlink(name) == -1) {
    perror("Sem unlink error");
    return 0;
  }
  return 1;
}

int my_sem_wait(sem_t* sem) {
  if(sem_wait(sem) == -1) {
    perror("Sem wait error");
    return 0;
  }
  return 1;
}

int my_sem_post(sem_t* sem) {
  if(sem_post(sem) == -1) {
    perror("Sem post error");
    return 0;
  }
    return 1;
}

int my_sem_get_value(sem_t* sem, int *svalue) {
  if(sem_getvalue(sem, svalue) == -1) {
    perror("Sem get value error");
    return 0;
  }
  return 1;
}