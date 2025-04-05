//-------------------------------------------------------------
// Autor: Mateusz Kamiński                   Kraków, 05.04.2025
//
// Plik: write_fifo.c
// Rola: Producent – odczytuje dane z pliku wejściowego i
//       przesyła je do potoku FIFO.
//
// Wywołanie: ./write_fifo <input.txt> <fifo_path>
//-------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 80 // rozmiar bufora (liczba bajtów na raz)

void error_message(const char* msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Za mało argumentów! Proszę wywołać: ./%s <input> <fifo_path> \n", argv[0]);
        exit(1);
    }

    char buf[BUF_SIZE];
    ssize_t bytes;

    int file_to_read = open(argv[1], O_RDONLY);
    if (file_to_read == -1)
    {
        error_message("open input error");
    }

    int fifo = open(argv[2], O_WRONLY, 0644);
    if (fifo == -1)
    {
        error_message("open fifo error");
    }

    while ((bytes = read(file_to_read, buf, BUF_SIZE)) > 0)
    {
        if (write(fifo, buf, bytes) == -1)
        {
            error_message("write output error");
        }

        printf("\033[31m[Producent]\033[0m Zapisano %ld bajtów: \n", bytes);
        write(STDOUT_FILENO, buf, bytes);
        fflush(stdout);

        sleep((rand() % 2 + 1)); // losowy czas
    }
    if (bytes == -1)
    {
        error_message("read error");
    }

    // Zamykamy deskryptor pliku do odczytu
    if (close(file_to_read) == -1)
    {
        error_message("close file_to_read error");
    }
    // Zamykamy deskryptor potoku do odczytu
    if (close(fifo) == -1)
    {
        error_message("close fifo error");
    }
}
