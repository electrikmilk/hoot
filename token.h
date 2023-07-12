//
// Created by Brandon Jordan on 6/10/23.
//

#ifndef HOOT_TOKEN_H
#define HOOT_TOKEN_H

#endif // HOOT_TOKEN_H

typedef const char *token_type;

static const token_type keyword_var = "var";
static const token_type keyword_const = "const";
static const token_type keyword_funct = "function";
static const token_type keyword_if = "if";
static const token_type keyword_else = "else";
static const token_type keyword_elseif = "elseif";
static const token_type keyword_for = "for";
static const token_type keyword_foreach = "foreach";
static const token_type operator_equals = "=";

/* Types */
static const token_type type_string = "\"";
static const token_type type_int = "123456789.-";

typedef struct {
    token_type type;
    char *identifier;
    token_type value_type;
    void *value_ptr;
} token;
