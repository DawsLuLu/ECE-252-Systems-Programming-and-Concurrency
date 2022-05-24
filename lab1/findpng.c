#include "util.h"

int main (int argc, char *argv[]) {

    if (argc == 1) {
        printf("Usage: %s <directory name>\n", argv[0]);
        exit(1);
    }

    int foundPng = 0;

    char *init_dir = malloc(strlen(argv[1]) * sizeof(char));
    strcpy(init_dir, argv[1]);

    if (init_dir[strlen(init_dir) - 1] == '/') {
        init_dir[strlen(init_dir) - 1] = '\0';
    }

    findPng(argv[1], &foundPng, init_dir);

    if (!foundPng) {
        printf("findpng: No PNG file found\n");
    }

    free(init_dir);

    return 0;

}