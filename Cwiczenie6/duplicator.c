//-------------------------------------------------------------
// Autor: Mateusz Kamiński                   Kraków, 12.04.2025
//
// CWICZENIE 6 - Wzajemne wykluczanie dla procesów
// Program: duplicator.c
//
// Tworzy semafor nazwany POSIX, plik numer.txt z wartością 0,
// uruchamia zadane procesy potomne (worker), a po ich zakończeniu
// porównuje wartość końcową z oczekiwaną.
// Semafor usuwany przez atexit() lub po sygnale SIGINT.
//
// Przykładowe uruchomienie: ./duplicator ./critical_worker 5 10


#include "./include/my_semaphore.h"

const char* sem_name = "/my_semaphore";

void cleanup() {
  CheckError(my_sem_unlink(sem_name));
}

void signal_handler(int sig)
{
     if (sig == SIGINT)
     {
          printf("\nOtrzymano sygnał SIGINT. Kończę program i usuwam Semafor.\n");
          cleanup();
          _exit(1); // Kończenie programu bez wywoływania atexit
     }
}

int main(int argc, char *argv[]) {
     if (argc != 4)
     {
          fprintf(stderr, "Za mało argumentów! Proszę wywołać: %s <worker> <liczba procesów> <liczba sekcji krytycznych> \n", argv[0]);
          exit(1);
     }

     atexit(cleanup);

     if(signal(SIGINT, signal_handler) == SIG_ERR) {
          perror("signal error");
          exit(1);
     }

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

     if (final_value == expected_value)
          printf("✅  \033[32mWszystko OK!\033[0m Ostateczna wartość: %d, oczekiwana wartość: %d\n", final_value, expected_value);
     else
          printf("❌  \033[31mBłąd!\033[0m Ostateczna wartość: %d, oczekiwana wartość: %d\n", final_value, expected_value);


     return 0;
}