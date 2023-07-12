//
// Created by Brandon Jordan on 6/9/23.
//

#include <string.h>
#include "bool.h"

typedef struct {
    const char *name;
    char short_char;
    const char *description;
    bool expects_value;
    bool active;
} argument;

argument *args;
size_t args_size = 0;

void substr(char *str, size_t cutpos, size_t cutlen) {
    size_t len;
    len = strlen(str);
    if (cutpos >= len) return;
    if (cutpos + cutlen > len) cutlen = len - cutpos;
    memmove(str + cutpos, str + cutpos + cutlen, len - cutpos - cutlen);
}

void print_args() {
    for (int i = 0; i < args_size; i++) {
        if (args[i].expects_value) {
            printf("\t-%c= --%s=\t%s\n", args[i].short_char, args[i].name, args[i].description);
        } else {
            printf("\t-%c --%s\t%s\n", args[i].short_char, args[i].name, args[i].description);
        }
    }
}

void print_usage() {
    char available_flags[20];
    for (int i = 0; i < args_size; i++) {
        sprintf(available_flags, "%s -%c ", available_flags, args[i].short_char);
    }
    printf("USAGE: hoot [FILE].(hoot|hoo|who) [%s]\nOptions:\n", available_flags);
    print_args();
}

void parse_args(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        char *arg_value = argv[i];
        if (arg_value == NULL) {
            continue;
        }
        if (strstr(arg_value, "-") == NULL) {
            continue;
        }
        size_t dashes = 1;
        if (strstr(arg_value, "--") != NULL) {
            dashes = 2;
        }
        substr(arg_value, 0, dashes);

        for (int j = 0; j < args_size; j++) {
            if (args[j].name == arg_value || args[j].short_char == arg_value[0]) {
                args[j].active = true;
            }
        }
    }
}

void register_arg(const char *arg_name, const char short_char, const char *description, bool expects_value) {
    args = (argument*)malloc(sizeof(*args) + sizeof(argument));
    if (args == NULL) {
        printf("alloc for new arg failed\n");
        exit(1);
    }
    argument new_arg;
    new_arg.name = arg_name;
    new_arg.short_char = short_char;
    new_arg.description = description;
    new_arg.expects_value = expects_value;
    new_arg.active = false;
    args[args_size] = new_arg;
    args_size++;
}

bool using_arg(const char *arg_name) {
    for (int j = 0; j < args_size; j++) {
        if (args[j].name == arg_name) {
            return args[j].active;
        }
    }
    return false;
}
