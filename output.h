//
// Created by Brandon Jordan on 6/9/23.
//

#ifndef HOOT_OUTPUT_H
#define HOOT_OUTPUT_H

#endif //HOOT_OUTPUT_H

#include <stdio.h>
#include <stdlib.h>

typedef int style;

static const style BOLD = 1;
static const style DIM = 2;
static const style RED = 31;
static const style GREEN = 32;
static const style YELLOW = 33;

static void aprint(style, char *);

static void error(char *);

static void warn(char *);

static void success(char *);
