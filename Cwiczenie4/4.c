//-------------------------------------------------------------
// Autor: Mateusz Kamiński                   Kraków, 04.04.2025
//
// CWICZENIE 4 - Problem Producenta - Konsumenta
// Wykorzystanie potoków do komunikacji między procesami
//
// Przykładowe uruchomienie: ./producent_konsument input.txt output.txt

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>


#define BUF_SIZE 64 // rozmiar bufora (liczba bajtów na raz)

void error_message(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {

  if(argc != 3) {
    fprintf(stderr, "Za mało argumentów! Proszę wywołać: ./producent_konsument <input> <output> \n");
    exit(1);
  }

  srand(time(NULL));
  int pipe_filedes[2]; // Tablica przechowująca dwa deskryptory plików
  char buf[BUF_SIZE];
  ssize_t bytes;

  if(pipe(pipe_filedes) == -1) {
    error_message("pipe error");
  }

   switch(fork()) {
     case -1:
       error_message("fork error");

     case 0:
       // Konsumentem zostaje proces potomny

       // Zamykamy deksryptor do zapisu, który jest niepotrzebny
       if(close(pipe_filedes[1]) == -1) {
            error_message("close pipe error");
        }

       int output = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
       if(output == -1) {
         error_message("open output error");
       }
       while((bytes = read(pipe_filedes[0], buf, BUF_SIZE)) > 0) {
         if(write(output, buf, bytes) == -1) {
           error_message("write output error");
           }

         printf("\n\033[34m[Konsument]\033[0m Odczytano %ld bajtów:\n", bytes);
         write(STDOUT_FILENO, buf, bytes);
         // printf("\n-----------------------------------------------\n");
         fflush(stdout);

         sleep(rand() % 2); // losowy czas
         }
         if(bytes == -1) {
           error_message("read error");
         }

         if(close(output) == -1) {
           error_message("close output error");
         }
         // Zamykamy deskryptor potoku do odczytu
         if(close(pipe_filedes[0]) == -1) {
              error_message("close pipe error");
            }
         exit(0); // kończymy proces potomny

       default:
         // Producentem zostaje proces macierzysty

         // Zamykamy deksryptor do odczytu, który jest niepotrzebny
         if(close(pipe_filedes[0]) == -1) {
           error_message("close pipe error");
         }

         int input = open("input.txt", O_RDONLY);
         if(input == -1) {
           error_message("open input error");
           }

         while((bytes = read(input, buf, BUF_SIZE)) > 0) {
           if(write(pipe_filedes[1], buf, bytes) == -1) {
             error_message("write pipe error");
           }
           printf("\n------------------------------------------------\n");
           printf("\033[31m[Producent]\033[0m Zapisano %ld bajtów: \n", bytes);
           write(STDOUT_FILENO, buf, bytes);
           fflush(stdout);

           sleep(rand() % 3 + 1); // losowy czas
         }

            if(bytes == -1) {
            error_message("read error");
            }
         if(close(input) == -1) {
           error_message("close input error");
         }
         // Zamykamy deskryptor potoku do zapisu
         if(close(pipe_filedes[1]) == -1) {
           error_message("close pipe error");
         }

         // Czekamy na zakończenie Konsumenta
           if(wait(NULL) == -1) {
             error_message("wait error");
           }
         }
  printf("------------------------------------------------\n");
  return 0;
}