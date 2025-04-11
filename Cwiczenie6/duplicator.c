#include "./include/my_semaphore.h"

const char* sem_name = "/my_semaphore";

void cleanup() {
  CheckError(my_sem_unlink(sem_name));
}

int main(int argc, char *argv[]) {
     if (argc != 4)
     {
          fprintf(stderr, "Za mało argumentów! Proszę wywołać: %s <worker> <liczba procesów> <liczba sekcji krytycznych> \n", argv[0]);
          exit(1);
     }

     atexit(cleanup);

     int critical_sections = atoi(argv[3]);
     int number_of_processes = atoi(argv[2]);
     sem_t* my_sem = my_sem_init(sem_name, 1);
     CheckError(my_sem != NULL);

     int my_svalue;
     CheckError(my_sem_get_value(my_sem, &my_svalue));

     int number = open("numer.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
     CheckError(number != -1);
     char buf[] = "0\n";
     CheckError(write(number, buf, strlen(buf)) != -1);
     CheckError(close(number) != -1 );

     for(int i = 0; i < number_of_processes; i++) {
       switch(fork()) {
           case -1:
                perror("fork error");
                exit(1);

           case 0:
                execlp(argv[1], argv[1], sem_name, argv[3], (char*)NULL);
                perror("execlp error");
                _exit(1);

           default:
                break;
           }
     }

     for(int i = 0; i < number_of_processes; i++)
          wait(NULL);

     number =  open("numer.txt", O_RDONLY, 0644);
     CheckError(number != -1);
     char read_buf[100];
     ssize_t bytes = read(number, read_buf, sizeof(read_buf) - 1);
     CheckError(bytes != -1);
     read_buf[bytes] = '\0';
     CheckError(close(number) != -1);

     int final_value = atoi(read_buf);
     int expected_value = critical_sections * number_of_processes;

     printf("\n numer.txt = %d, oczekiwano %d\n", final_value, expected_value);
     if (final_value == expected_value)
          printf("✅ \033[32mWszystko OK!\033[0m Ostateczna wartość: %d, oczekiwana wartość: %d\n", final_value, expected_value);
     else
          printf("❌ \033[31mBłąd!\033[0m Ostateczna wartość: %d, oczekiwana wartość: %d\n", final_value, expected_value);


     return 0;
}