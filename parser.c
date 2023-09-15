//
// Created by Brandon Jordan on 6/10/23.
//

#include "parser.h"
#include "output.c"

#ifndef HOOT_PARSE_H
#define HOOT_PARSE_H

#endif // HOOT_PARSE_H

void parse(FILE *fptr) {
    load_file(fptr);
    idx = -1;
    advance();
    while (current_char != -1) {
        if (current_char == ' ' || current_char == '\t' || current_char == '\n') {
            advance();
        } else if (token_ahead(keyword_var)) {
            advance();
            char *identifier;
            char *ahead = look_ahead_until('\n');
            bool collect_value = false;
            token t;
            if (strstr(ahead, "=")) {
                collect_until(&identifier, ' ');
                collect_value = true;
                advance();
            } else {
                collect_until(&identifier, '\n');
            }
            printf("\nVariable: %s\n", identifier);
            advance_times(2);
            if (collect_value) {
                char *value;
                token_type value_type;
                if (current_char == '"') {
                    advance();
                    value_type = type_string;
                    collect_string(&value);
                } else if (strstr(type_int, current_char_str())) {
                    value_type = type_int;
                    collect_until(&value, '\n');
                } else {
                    parser_error("Unknown data type!");
                }
                printf("Type: %s\n", value_type);
                printf("Value: %s\n", value);

                t.value_type = value_type;
            } else {
                printf("Value: (empty)\n");
            }
            t.identifier = identifier;
            add_token(&t);
        } else if (token_ahead(keyword_const)) {
        } else {
            char *illegal_char_err = "";
            sprintf(illegal_char_err, "Illegal character '%c'", current_char);
            parser_error(illegal_char_err);
        }
    }
    free(buffer);
}

void print_current_char() {
    printf("(%ld:%ld) ", line_idx, line_char_idx);
    print_char(current_char);
}

void print_char(char c) {
    switch(c) {
        case '\n':
            printf("NEW LINE %ld:%ld\n", line_idx, line_char_idx);
            break;
        case '\t':
            printf("TAB %ld:%ld\n", line_idx, line_char_idx);
            break;
        case ' ':
            printf("SPACE %ld:%ld\n", line_idx, line_char_idx);
            break;
        default:
            printf("%c %ld:%ld\n", c, line_idx, line_char_idx);
    }
}

void add_token(token *t) {
    tokens = (token *) malloc(sizeof(*tokens) + sizeof(token));
    tokens[tokens_size] = *t;
    tokens_size++;
}

void load_file(FILE *fptr) {
    fseek(fptr, 0, SEEK_END);
    file_length = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    buffer = malloc(file_length);
    if (!buffer) {
        printf("error: no buffer to read");
    }
    fread(buffer, 1, file_length, fptr);
}

void advance() {
    ++idx;
    if (file_length > idx) {
        current_char = buffer[idx];
        if (current_char == '\n') {
            line_char_idx = 0;
            ++line_idx;
        } else {
            line_char_idx++;
        }
    } else {
        current_char = -1;
    }
}

void advance_times(size_t times) {
    for (int i = 0; i < times; ++i) {
        advance();
    }
}

char get_char(ssize_t i) {
    if (i < 0) {
        return -1;
    }
    if (file_length < i) {
        return -1;
    }
    return buffer[i];
}

void first_char() {
    line_idx = 0;
    line_char_idx = 0;
    idx = -1;
    advance();
}

void parser_error(char *message) {
    sprintf(message, "%s (%ld:%ld)", message, line_idx, line_char_idx);
    error(message);
}

bool token_ahead(token_type t) {
    bool is_ahead = true;
    for (int i = 0; i < strlen(t); i++) {
        char t_char = t[i];
        if (t_char == ' ' || t_char == '\t' || t_char == '\n') {
            continue;
        }
        if (i == 0) {
            if (tolower(current_char) != t_char) {
                is_ahead = false;
                break;
            }
        } else if (next(i) != t_char) {
            is_ahead = false;
            break;
        }
    }
    if (is_ahead) {
        advance_times(strlen(t));
    }
    return is_ahead;
}

bool tokens_ahead(token_type t, ...) {
    va_list list_ptr;
    va_start(list_ptr, t);
    for (int i = 0; i < sizeof t; ++i) {
        if (token_ahead(va_arg(list_ptr, token_type))) {
            return true;
        }
    }
    va_end(list_ptr);
    return false;
}

bool is_token(token_type t) {
    if (tolower(current_char) != tolower(t[0])) {
        return false;
    }
    advance_times(strlen(t));
    return true;
}

size_t get_ahead_size(char until) {
    size_t ahead_size = 0;
    for (size_t i = idx; i < file_length; i++) {
        if (buffer[i] == until) {
            break;
        }
        ahead_size++;
    }
    return ahead_size;
}

char *look_ahead_until(char until) {
    char *collection;
    size_t ahead_size = get_ahead_size(until);
    collection = (char *) malloc(ahead_size * sizeof(char *));
    for (size_t i = 0; i < ahead_size; i++) {
        sprintf(collection, "%s%c", collection, buffer[idx + i]);
    }
    char *collected = strdup(collection);
    free(collection);
    return collected;
}

void collect_until(char **collection, char until) {
    size_t ahead_size = get_ahead_size(until);
    *collection = (char *) malloc(ahead_size * sizeof(char *));
    while (current_char != until && current_char != -1) {
        sprintf(*collection, "%s%c", *collection, current_char);
        advance();
    }
}

char seek(const int *mov, bool reverse) {
    ssize_t next_char = idx;
    char return_char;
    if (reverse) {
        next_char -= *mov;
    } else {
        next_char += *mov;
    }
    return_char = get_char(next_char);
    while (return_char == ' ' || return_char == '\t' || return_char == '\n') {
        if (reverse) {
            --next_char;
        } else {
            ++next_char;
        }
        return_char = get_char(next_char);
    }
    return return_char;
}

char next(int mov) {
    return seek(&mov, false);
}

char prev(int mov) {
    return seek(&mov, true);
}

char *current_char_str() {
    char *current;
    current = (char *) malloc(1 * sizeof(char *));
    sprintf(current, "%c", current_char);
    return current;
}

void collect_string(char **collection) {
    size_t ahead_size = get_ahead_size('"');
    *collection = (char *) malloc(ahead_size * sizeof(char *));
    while (current_char != -1) {
        if (current_char == '"' && prev(1) != '\\') {
            break;
        }
        if (current_char == '\\' && next(1) == '"') {
            advance();
            continue;
        }
        sprintf(*collection, "%s%c", *collection, current_char);
        advance();
    }
    advance();
}
