#include <stdio.h>
#include <string.h>

#include "filesystem.h"

const char helpPage[] = "Filesystem management program\n"
                        "commands:\n"
                        "help - show this page\n"
                        "ls - list files\n"
                        "create [file name] - create Filesystem\n"
                        "save [file] - save file\n"
                        "dump [file] - dump file\n"
                        "rm [file] - remove file\n"
                        "delete - delete Filesystem\n"
                        "map - display graphical representation of Filesystem\n";

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("invalid parameters\n");
        return -1;
    }

    if (!strcmp(argv[1], "help")) {
        printf("%s\n", helpPage);

    } else if (!strcmp(argv[1], "ls")) {
        fs_ls();

    } else if (!strcmp(argv[1], "create")) {
        fs_create();

    } else if (!strcmp(argv[1], "delete")) {
        fs_delete();

    } else if (!strcmp(argv[1], "save")) {
        if (argc < 4) {
            printf("invalid parameters\n");
            return -1;
        }
        fs_save(argv[2], argv[3]);

    } else if (!strcmp(argv[1], "dump")) {
        if (argc < 4) {
            printf("invalid parameters\n");
            return -1;
        }
        fs_dump(argv[2], argv[3]);

    } else if (!strcmp(argv[1], "rm")) {
        if (argc < 3) {
            printf("invalid parameters\n");
            return -1;
        }
        fs_rm(argv[2]);

    } else if (!strcmp(argv[1], "map")) {
        fs_map();

    } else {
        printf("invalid command\n");
    }


}