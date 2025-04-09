//-------------------------------------------------------------
// Autor: Mateusz Kamiński                   Kraków, 05.04.2025
//
// Plik: read_fifo.c
// Rola: Konsument – odbiera dane z potoku FIFO i zapisuje je
//       do pliku wyjściowego.
//
// Wywołanie: ./read_fifo <output.txt> <fifo_path>
//-------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 60 // rozmiar bufora (liczba bajtów na raz)

void error_message(const char* msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Za mało argumentów! Proszę wywołać: %s <output> <fifo_path> \n", argv[0]);
        exit(1);
    }

    char buf[BUF_SIZE];
    ssize_t bytes;

    int file_to_write = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_to_write == -1)
    {
        error_message("open output error");
    }

    int fifo = open(argv[2], O_RDONLY, 0644);
    if (fifo == -1)
    {
        error_message("open fifo error");
    }

    while ((bytes = read(fifo, buf, BUF_SIZE)) > 0)
    {
        if (write(file_to_write, buf, bytes) == -1)
        {
            error_message("write output error");
        }

        printf("\n\033[34m[Konsument]\033[0m Odczytano %ld bajtów:\n", bytes);
        fflush(stdout);
        write(STDOUT_FILENO, buf, bytes);
        printf("\n------------------------------------------------\n");

        sleep((rand() % 2 + 1)); // losowy czas
    }
    if (bytes == -1)
    {
        error_message("read error");
    }

    // Zamykamy deskryptor pliku do odczytu
    if (close(file_to_write) == -1)
    {
        error_message("close file_to_read error");
    }
    // Zamykamy deskryptor potoku do odczytu
    if (close(fifo) == -1)
    {
        error_message("close fifo error");
    }
}
