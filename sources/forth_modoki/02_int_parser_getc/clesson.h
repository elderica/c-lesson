#include <stdio.h>

/*
return one character and move cursor.
return EOF if end of file.
*/
int cl_getc();

/*
set source string.
*/
void cl_getc_set_src(char* str);

typedef enum token_type {
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_SPACE,
} token_type_t;