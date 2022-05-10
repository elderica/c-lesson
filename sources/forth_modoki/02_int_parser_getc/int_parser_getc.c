#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "clesson.h"

/*
cc cl_getc.c int_parser_getc.c
*/

int parse_one(int last_char, int *out_value, token_type_t *out_type);

void test_parse_one_123(void);
void test_parse_one_123_456(void);

int parse_one(int c, int *out_value, token_type_t *out_type) {
  assert(out_value != NULL);
  assert(out_type != NULL);

  if (c == '\0') {
    c = cl_getc();
  }

  if (isspace(c)) {
    *out_type = TOKEN_TYPE_SPACE;
    *out_value = 0;
    while (1) {
      c = cl_getc();
      if (!isspace(c)) {
        return c;
      }
    }
  }

  if (isdigit(c)) {
    *out_type = TOKEN_TYPE_NUMBER;
    *out_value = c - '0';
    while (1) {
      c = cl_getc();
      if (isdigit(c)) {
        *out_value = (*out_value) * 10 + (c - '0');
      } else {
        return c;
      }
    }
  }

  // EOF

  return '\0';
}

void test_parse_one_123(void) {
  cl_getc_set_src("123 456");

  int value;
  token_type_t type;
  int c = parse_one('\0', &value, &type);

  assert(type == TOKEN_TYPE_NUMBER);
  assert(value == 123);
}

void test_parse_one_123_456(void) {
  cl_getc_set_src("123 456");

  int answer = 0;
  int c = '\0';
  token_type_t token_type;

  c = parse_one(c, &answer, &token_type);
  assert(answer == 123);
  assert(token_type == TOKEN_TYPE_NUMBER);

  c = parse_one(c, &answer, &token_type);
  assert(answer == 0);
  assert(token_type == TOKEN_TYPE_SPACE);

  c = parse_one(c, &answer, &token_type);
  assert(answer == 456);
  assert(token_type == TOKEN_TYPE_NUMBER);
}

int main() {
  int answer1 = 0;
  int answer2 = 0;

  test_parse_one_123();
  test_parse_one_123_456();

  return 0;
}