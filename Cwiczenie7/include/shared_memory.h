#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

// Funkcja tworząca pamięć dzieloną
int create_shared_memory(const char *name, size_t size);

// Funkcja otwierająca pamięć dzieloną
int open_shared_memory(const char *name, size_t size);

// Funkcja mapująca pamięć dzieloną do przestrzeni adresowej
void* map_shared_memory(int shm_fd, size_t size);

// Funkcja odmapowująca pamięć dzieloną
int unmap_shared_memory(void* addr, size_t size);

// Funkcja zamykająca deskryptor pamięci dzielonej
int close_shared_memory(int shm_fd);

// Funkcja usuwająca pamięć dzieloną
int unlink_shared_memory(const char *name);

#endif // SHARED_MEMORY_H