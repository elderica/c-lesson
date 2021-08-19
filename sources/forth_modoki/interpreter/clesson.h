#ifndef CLESSON_H
#define CLESSON_H

#include <stdio.h>
#include <string.h>

/*
return one character and move cursor.
return EOF if end of file.
*/
int cl_getc();
void cl_getc_set_src(char* str);

int streq(char* s1, char* s2);

#endif
