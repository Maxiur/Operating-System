#include "../include/shared_memory.h"

int create_shared_memory(const char *name, size_t size)
{
    int shm_fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open error");
        return 0;
    }

    if (ftruncate(shm_fd, size) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return 0;
    }

    return shm_fd;
}
int open_shared_memory(const char *name, size_t size)
{
    int shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open error");
        return 0;
    }

    if (ftruncate(shm_fd, size) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return 0;
    }

    return shm_fd;
}
void* map_shared_memory(int shm_fd, size_t size)
{
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap error");
        close(shm_fd);
        return NULL;
    }

    return addr;
}
int unmap_shared_memory(void* addr, size_t size)
{
    if (munmap(addr, size) == -1) {
        perror("munmap error");
		return 0;
    }
	return 1;
}

int close_shared_memory(int shm_fd)
{
    if (close(shm_fd) == -1) {
        perror("close error");
		return 0;
    }
	return 1;
}
int unlink_shared_memory(const char *name)
{
    if (shm_unlink(name) == -1) {
        if(errno == ENOENT)
            return 1;
        perror("shm_unlink error");
        return 0;
    }
    return 1;
}
