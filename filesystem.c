
#include "filesystem.h"

static int32_t find_empty_inode(struct Inode inode_array[INODE_COUNT]) {
    for (int32_t i = 0; i < INODE_COUNT; i++) {
        if (inode_array[i].first_block_index == EMPTY_INODE) {
            return i;
        }
    }
    return ERR_INODE_FULL;
}

static int32_t find_empty_block(struct Block block_array[BLOCK_COUNT]) {
    for (int32_t i = 0; i < BLOCK_COUNT; i++) {
        if (block_array[i].next_block_index == EMPTY_BLOCK) {
            return i;
        }
    }
    return ERR_FS_FULL;
}

static int16_t find_inode_by_name(struct Inode inode_array[INODE_COUNT], char *file_name) {
    for (int16_t i = 0; i < INODE_COUNT; i++) {
        if (inode_array[i].first_block_index != EMPTY_INODE && strcmp(inode_array[i].name, file_name) == 0) {
            return i;
        }
    }
    return FILE_DOES_NOT_EXIST;
}

int fs_create() {
    file_create_fs();
    struct Filesystem *fs = file_map_fs();
    printf("Creating filesystem\n");

    for (int32_t i = 0; i < INODE_COUNT; i++) {
        fs->inode_array[i].first_block_index = EMPTY_INODE;

    }

    for (int32_t i = 0; i < BLOCK_COUNT; i++) {
        fs->block_array[i].next_block_index = EMPTY_BLOCK;
    }

    printf("Filesystem created\n");

    file_unmap_fs(fs);
    return 0;
}

int fs_delete() {
    file_delete_fs();
    printf("Filesystem deleted\n");
    return 0;
}

int fs_save(char *existing_file, char new_file[]) {
    struct Filesystem *fs = file_map_fs();

    if(find_inode_by_name(fs->inode_array, new_file) != FILE_DOES_NOT_EXIST) {
        printf("File %s already exists\n", new_file);
        return ERR_FILE_EXISTS;
    }

    FILE *file = fopen(existing_file, "rb");
    if (file == NULL) {
        printf("File open error\n");
        return ERR_FILE_OPEN;
    }

    int32_t inode_index = find_empty_inode(fs->inode_array);
    if (inode_index == ERR_INODE_FULL) {
        printf("Inode is full\n");
        return ERR_INODE_FULL;
    }

    fseek(file, 0L, SEEK_END);
    fs->inode_array[inode_index].file_size = ftell(file);
    rewind(file);

    uint32_t blocks_saved = 0;
    int32_t previous_block_index;

    do {
        int32_t block_index = find_empty_block(fs->block_array);

        if (block_index == ERR_FS_FULL) {
            printf("Filesystem is full\n");
            return ERR_FS_FULL;
        }

        // save to inode if first block of a file
        if (blocks_saved == 0) {
            fs->inode_array[inode_index].first_block_index = block_index;
            strcpy(fs->inode_array[inode_index].name, new_file);

        } else {
            fs->block_array[previous_block_index].next_block_index = block_index;
        }

        fs->block_array[block_index].next_block_index = LAST_BLOCK;
        fread(fs->block_array[block_index].data, BLOCK_DATA_SIZE, 1, file);

        previous_block_index = block_index;
        blocks_saved++;
    } while (!feof(file));

    printf("File %s saved as %s\n%dB saved in %d blocks of %luB", existing_file, new_file,
           fs->inode_array[inode_index].file_size, blocks_saved, BLOCK_DATA_SIZE);

    file_unmap_fs(fs);
    return 0;
}

int fs_dump(char *source_file, char *target_file) {
    struct Filesystem *fs = file_map_fs();

    int16_t inode_index = find_inode_by_name(fs->inode_array, source_file);
    if (inode_index == FILE_DOES_NOT_EXIST) {
        printf("File %s does not exist\n", source_file);
        return FILE_DOES_NOT_EXIST;
    }

    FILE *file = fopen(target_file, "wb");
    if (file == NULL) {
        printf("File open error\n");
        return ERR_FILE_OPEN;
    }

    uint32_t data_left = fs->inode_array[inode_index].file_size;
    int32_t block_index = fs->inode_array[inode_index].first_block_index;
    do {
        fwrite(fs->block_array[block_index].data,
               data_left >= BLOCK_DATA_SIZE ? BLOCK_DATA_SIZE : data_left,
               1, file);
        data_left -= BLOCK_DATA_SIZE;
    } while (LAST_BLOCK != (block_index = fs->block_array[block_index].next_block_index));

    file_unmap_fs(fs);
    return 0;
}

int fs_rm(char *file) {
    struct Filesystem *fs = file_map_fs();

    int16_t inode_index = find_inode_by_name(fs->inode_array, file);
    if (inode_index == FILE_DOES_NOT_EXIST) {
        printf("File %s does not exist\n", file);
        return FILE_DOES_NOT_EXIST;
    }

    int32_t block_index = fs->inode_array[inode_index].first_block_index;
    int32_t next_block_index;

    do{
        next_block_index = fs->block_array[block_index].next_block_index;
        fs->block_array[block_index].next_block_index = EMPTY_BLOCK;
        block_index = next_block_index;
    }while(block_index != LAST_BLOCK);

    fs->inode_array[inode_index].first_block_index = EMPTY_BLOCK;

    file_unmap_fs(fs);
    return 0;
}

int fs_ls() {
    struct Filesystem *fs = file_map_fs();
    for (int32_t i = 0; i < INODE_COUNT; i++) {
        if (fs->inode_array[i].first_block_index != EMPTY_INODE) {
            printf("%s\t%uB\n", fs->inode_array[i].name, fs->inode_array[i].file_size);
        }
    }

    file_unmap_fs(fs);
    return 0;
}

int fs_map() {
    struct Filesystem *fs = file_map_fs();

    bool inode_map[INODE_COUNT];
    bool block_map[BLOCK_COUNT];

    for(int32_t i = 0; i < INODE_COUNT; i++) {
        inode_map[i] = fs->inode_array[i].first_block_index != EMPTY_INODE;
    }

    for(int32_t i = 0; i < BLOCK_COUNT; i++) {
        block_map[i] = fs->block_array[i].next_block_index != EMPTY_BLOCK;
    }

    printf("Inodes:\n");
    for(int32_t i = 0; i < INODE_COUNT; i++) {
        printf("%c", inode_map[i] ? '=' : '.');
        if (i % 128 == 127) {
            printf("\n");
        }
    }

    printf("\n\nBlocks:\n");
    for(int32_t i = 0; i < BLOCK_COUNT; i++) {
        printf("%c", block_map[i] ? '=' : '.');
        if (i % 128 == 127) {
            printf("\n");
        }
    }

    file_unmap_fs(fs);
    return 0;
}

