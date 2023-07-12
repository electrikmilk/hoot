//
// Created by Brandon Jordan on 6/10/23.
//

#include "output.h"

/* Print with ANSI escape sequence */
void aprint(const style c, char *str) {
    sprintf(str, "\033[%dm%s\033[0m\n", c, str);
    printf("%s", str);
}

void error(char *str) {
    sprintf(str, "error: %s", str);
    aprint(RED, str);
    exit(1);
}

void warn(char *str) {
    sprintf(str, "warn: %s", str);
    aprint(YELLOW, str);
}

void success(char *str) {
    sprintf(str, "success: %s", str);
    aprint(GREEN, str);
}
