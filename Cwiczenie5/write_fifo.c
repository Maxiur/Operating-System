#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

if(close(pipe_filedes[0]) == -1) {
    error_message("close pipe error");
}

int input = open(argv[1], O_RDONLY);
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
}