#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clesson.h"

enum LexicalType {
    NUMBER,
    SPACE,
    EXECUTABLE_NAME,
    LITERAL_NAME,
    OPEN_CURLY,
    CLOSE_CURLY,
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    enum LexicalType ltype;
    union {
        int number;
        char onechar;
        char *name;
    } u;
};

#define NAME_SIZE 256

int parse_one(int prev_ch, struct Token *out_token) {
    if (prev_ch == EOF) {
        prev_ch = cl_getc();
    }

    if (isspace(prev_ch)) {
        while (isspace(prev_ch)) {
            prev_ch = cl_getc();
        }
        out_token->ltype = SPACE;
        return prev_ch;
    }

    if (isdigit(prev_ch)) {
        out_token->ltype = NUMBER;
        int number = prev_ch - '0';
        while (1) {
            prev_ch = cl_getc();
            if (isdigit(prev_ch)) {
                number = number * 10 + prev_ch - '0';
            } else {
                out_token->u.number = number;
                return prev_ch;
            }
        }
    }

    if (prev_ch == '/') {
        prev_ch = cl_getc();
        if (isalpha(prev_ch)) {
            out_token->ltype = LITERAL_NAME;

            char *lit_name = calloc(NAME_SIZE + 1, sizeof(char));
            lit_name[0] = prev_ch;
            int i = 1;

            for (; i < NAME_SIZE; i++) {
                prev_ch = cl_getc();
                if (isalpha(prev_ch)) {
                    lit_name[i] = prev_ch;
                } else {
                    break;
                }
            }
            lit_name[i] = '\0';

            out_token->u.name = lit_name;
            return prev_ch;
        } else {
            out_token->ltype = UNKNOWN;
            return EOF;
        }
    }

    if (isalpha(prev_ch)) {
        out_token->ltype = EXECUTABLE_NAME;

        char *exec_name = calloc(NAME_SIZE + 1, sizeof(char));
        exec_name[0] = prev_ch;
        int i = 1;

        for (; i < NAME_SIZE; i++) {
            prev_ch = cl_getc();
            if (isalpha(prev_ch)) {
                exec_name[i] = prev_ch;
            } else {
                break;
            }
        }
        exec_name[i] = '\0';

        out_token->u.name = exec_name;
        return prev_ch;
    }

    if (prev_ch == EOF) {
        out_token->ltype = END_OF_FILE;
        return prev_ch;
    }

    out_token->ltype = UNKNOWN;
    return EOF;
}

void parser_print_all() {
    int ch = EOF;
    struct Token token = {UNKNOWN, {0}};

    do {
        ch = parse_one(ch, &token);
        if (token.ltype != UNKNOWN) {
            switch (token.ltype) {
                case NUMBER:
                    printf("num: %d\n", token.u.number);
                    break;
                case SPACE:
                    printf("space!\n");
                    break;
                case OPEN_CURLY:
                    printf("Open curly brace '%c'\n", token.u.onechar);
                    break;
                case CLOSE_CURLY:
                    printf("Close curly brace '%c'\n", token.u.onechar);
                    break;
                case EXECUTABLE_NAME:
                    printf("EXECUTABLE_NAME: %s\n", token.u.name);
                    break;
                case LITERAL_NAME:
                    printf("LITERAL_NAME: %s\n", token.u.name);
                    break;

                default:
                    printf("Unknown type %d\n", token.ltype);
                    break;
            }
        }
    } while (ch != EOF);
}

static void test_parse_one_number() {
    char *input = "123";
    int expect = 123;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == NUMBER);
    assert(expect == token.u.number);
}

static void test_parse_one_empty_should_return_END_OF_FILE() {
    char *input = "";
    int expect = END_OF_FILE;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect);
}

static void test_parse_one_executable_name() {
    char *input = "add";
    char *expect_name = "add";
    int expect_type = EXECUTABLE_NAME;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(strcmp(token.u.name, expect_name) == 0);
}

static void test_parse_one_literal_name() {
    char *input = "/add";
    char *expect_name = "add";
    int expect_type = LITERAL_NAME;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(strcmp(token.u.name, expect_name) == 0);
}

static void unit_tests() {
    test_parse_one_empty_should_return_END_OF_FILE();
    test_parse_one_number();
    test_parse_one_executable_name();
    test_parse_one_literal_name();
}

int main() {
    unit_tests();

    cl_getc_set_src("123 45 add /some { 2 3 add } def");
    parser_print_all();
    return 0;
}