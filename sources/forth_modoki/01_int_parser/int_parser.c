#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static const char* const input = "123 456  1203";


int main() {
    int answer1 = 0;
    int answer2 = 0;
    int answer3 = 0;

    // write something here.
    char *nstr = input;
    char *endptr = input;
    
    nstr = endptr;
    answer1 = strtol(nstr, &endptr, 10);
    printf("endptr: %s\n", endptr);

    nstr = endptr;
    answer2 = strtol(nstr, &endptr, 10);
    printf("endptr: %s\n", endptr);

    nstr = endptr;
    answer3 = strtol(nstr, &endptr, 10);
    printf("endptr: %s\n", endptr);


    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);
    assert(answer3 == 1203);

    return 0;
}