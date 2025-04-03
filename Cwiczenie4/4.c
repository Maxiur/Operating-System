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
       close(pipe_filedes[1]); // zamykamy deksryptor do zapisu, który jest niepotrzebny

       int output = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
       if(output == -1) {
         error_message("open output error");
       }
       while((bytes = read(pipe_filedes[0], buf, BUF_SIZE)) > 0) {
         if(write(output, buf, bytes) == -1) {
           error_message("write output error");
           }
         buf[bytes] = '\0'; // null-terminated
         printf("[Konsument] Odczytano %ld bajtów: %s\n", bytes, buf);
         fflush(stdout);

         sleep(rand() % 3 + 1); // losowy czas
         }
         if(bytes == -1) {
           error_message("read error");
         }

         close(output);
         close(pipe_filedes[0]); // zamykamy deskryptor do odczytu
         exit(0); // kończymy proces potomny

       default:
         // Producentem zostaje proces macierzysty
         close(pipe_filedes[0]); // zamykamy deksryptor do odczytu, który jest niepotrzebny

         int input = open("input.txt", O_RDONLY);
         if(input == -1) {
           error_message("open input error");
           }

         while((bytes = read(input, buf, BUF_SIZE)) > 0) {
           if(write(pipe_filedes[1], buf, bytes) == -1) {
             error_message("write pipe error");
           }
           buf[bytes] = '\0'; // null-terminated
           printf("[Producent] Zapisano %ld bajtów: %s\n", bytes, buf);
           fflush(stdout);

           sleep(rand() % 3 + 1); // losowy czas
         }

            if(bytes == -1) {
            error_message("read error");
            }
         close(input);
         close(pipe_filedes[1]); // zamykamy deskryptor do zapisu

         // Czekamy na zakończenie Konsumenta
           if(wait(NULL) == -1) {
             error_message("wait error");
           }
         }

  return 0;
}