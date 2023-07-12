//
// Created by Brandon Jordan on 6/9/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "args.h"
#include "parser.h"
#include "bool.h"

void check_file(const char *filename);

int main(int argc, char *argv[]) {
    register_arg("help", 'h', "Show usage information.", false);
    parse_args(argc, argv);

    if (argc < 2) {
        printf("usage: hoot [FILE] [OPTIONS]. Use the --help option for more.\n");
        exit(1);
    }

    if (using_arg("help")) {
        print_usage();
        exit(0);
    }

    const char *filename = argv[1];
    check_file(filename);

    FILE *fptr;
    if ((fptr = fopen(filename, "r")) == NULL) {
        printf("error: Unable to open: %s\n", filename);
        exit(1);
    }

    parse(fptr);

    fclose(fptr);
    free(tokens);
    free(args);

    exit(0);
}

/*
 * Check if file exists and validate extension
 */
void check_file(const char *filename) {
    if (access(filename, F_OK) != 0) {
        printf("error: File '%s' does not exist!", filename);
        exit(1);
    }

    char *extensions[] = {".hoot", ".hoo", ".who"};
    const char *extension = strrchr(filename, '.');
    bool contains = false;
    for (int i = 0; i < (sizeof extensions / sizeof extensions[0]); ++i) {
        if (strcmp(extensions[i], extension) != false) {
            contains = true;
        }
    }
    if (!contains) {
        printf("error: Invalid file extension '%s'. Valid file extensions: .hoot, .hoo, .who", extension);
    }
}
