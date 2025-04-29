#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/mman.h>

int create_shared_memory(const char *name, size_t size);
int open_shared_memory(const char *name, size_t size);
void* map_shared_memory(int shm_fd, size_t size);
void unmap_shared_memory(void* addr, size_t size);
void close_shared_memory(int shm_fd);
void unlink_shared_memory(const char *name);

#endif // SHARED_MEMORY_H