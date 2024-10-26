

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include <stdint.h>
#include <string.h>

#include "file_operations.h"

#define ERR_FILE_OPEN (-10)

#define ERR_INODE_FULL (-20)
#define ERR_FS_FULL (-21)

#define ERR_FILE_EXISTS (-30)
#define FILE_DOES_NOT_EXIST (-31)

#define MAX_LINUX_PATH_LENGTH 4096

#define MAX_NAME_LENGTH 255
#define INODE_COUNT 10
#define BLOCK_SIZE 1024
#define BLOCK_DATA_SIZE (BLOCK_SIZE - sizeof(int32_t))
#define BLOCK_COUNT (int32_t) 1000

#define EMPTY_BLOCK (int32_t)(-1)
#define LAST_BLOCK (int32_t)(-2)
#define EMPTY_INODE (int32_t)(-1)

struct SuperBlock{

};

struct Inode {
    int32_t first_block_index;
    uint32_t file_size;
    char name[MAX_NAME_LENGTH];
};

struct Block {
    // index of the next block or information about the block (EMPTY_BLOCK, LAST_BLOCK)
    int32_t next_block_index;

    // file data
    uint8_t data[BLOCK_DATA_SIZE];
};

struct Filesystem {
    struct SuperBlock super_block;
    struct Inode inode_array[INODE_COUNT];
    struct Block block_array[BLOCK_COUNT];
};


int fs_create();

int fs_delete();

int fs_save(char existing_file[MAX_LINUX_PATH_LENGTH], char new_file[MAX_NAME_LENGTH]);

int fs_dump(char source_file[MAX_NAME_LENGTH], char target_file[MAX_LINUX_PATH_LENGTH]);

int fs_rm(char file[MAX_NAME_LENGTH]);

int fs_ls();

int fs_map();

#endif //FILESYSTEM_FILESYSTEM_H
