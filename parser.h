//
// Created by Brandon Jordan on 6/10/23.
//

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "bool.h"
#include "token.h"

#ifndef HOOT_PARSER_H
#define HOOT_PARSER_H

#endif //HOOT_PARSER_H

char *buffer;
size_t file_length;

static ssize_t idx = 0;
char current_char;

size_t line_idx;
size_t line_char_idx;

token *tokens;
size_t tokens_size;

void load_file(FILE *);

void parse(FILE *);

void parser_error(char *);

void advance();

void advance_times(size_t);

char get_char(ssize_t);

bool token_ahead(token_type);

bool tokens_ahead(token_type, ...);

bool is_token(token_type);

size_t get_ahead_size(char);

char *look_ahead_until(char);

void collect_until(char **, char);

char seek(const int *, bool);

char next(int mov);

char prev(int mov);

char *current_char_str();

void collect_string(char **);

void add_token(token *);

void print_current_char();

void print_char(char);
