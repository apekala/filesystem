//
// Created by antek on 16.05.24.
//

#ifndef FILESYSTEM_FILE_OPERATIONS_H
#define FILESYSTEM_FILE_OPERATIONS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "filesystem.h"

struct Filesystem;

void file_create_fs();

struct Filesystem *file_map_fs();

void file_unmap_fs(struct Filesystem *fs);

int file_delete_fs();


#endif //FILESYSTEM_FILE_OPERATIONS_H
