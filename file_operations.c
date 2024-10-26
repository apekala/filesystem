#include <sys/types.h>
#include <sys/mman.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file_operations.h"

char filename[] = "filesystem.bin";


void file_create_fs() {
    int fd = open(filename, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }

    // Ensure the file is the correct size
    if (ftruncate(fd, sizeof(struct Filesystem)) == -1) {
        perror("Error setting file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
}

struct Filesystem *file_map_fs() {
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Map the file into memory
    struct Filesystem *fs = mmap(NULL, sizeof(struct Filesystem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fs == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Close the file descriptor as it's no longer needed
    close(fd);
    return fs;
}


int file_delete_fs() {
    if (remove(filename) == 0) {
        return 0;
    } else {
        printf("Failed to delete the file\n");
        return -1;
    }
}

void file_unmap_fs(struct Filesystem *fs) {
    if (munmap(fs, sizeof(struct Filesystem)) == -1) {
        perror("Error unmapping memory");
        exit(EXIT_FAILURE);
    }
}
