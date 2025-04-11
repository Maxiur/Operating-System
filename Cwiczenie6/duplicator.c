#include "./include/my_semaphore.h"

int main(int argc, char *argv[]) {
     if (argc != 4)
     {
          fprintf(stderr, "Za mało argumentów! Proszę wywołać: %s <worker> <liczba procesów> <liczba sekcji krytycznych> \n", argv[0]);
          exit(1);
     }

     int number_of_processes = atoi(argv[2]);
     const char* sem_name = "/my_semaphore";
     sem_t my_sem = my_sem_init(sem_name, 1);
     CheckError(my_sem != NULL);

     int my_svalue;
     CheckError(my_sem_get_value(my_sem, &my_svalue));

     int number = open("numer.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

     for(int i = 0; i < number_of_processes; i++) {
       switch(fork())
           case -1:
                perror("fork error");
                exit(1);

           case 0:
                execlp(argv[1], argv[1], argv[3], (char*)NULL);

     }
     return 0;
}