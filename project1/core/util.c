#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Parses the file name from a path string
*/
char* get_filename(char* path)
{
    int pathlen = strlen(path);
    char* begin_name = path + (pathlen - 1);

    while((*begin_name) != '/' && begin_name != path) --begin_name;
    if((*begin_name) == '/') ++begin_name;

    return begin_name;
}

/*
Converts a string to all upper-case characters
*/
char* to_upper(char* str)
{
    char* cptr = str;
    while(*cptr)
    {
        if('a' <= (*cptr) && (*cptr) <= 'z')
            (*cptr) -= 32;

        ++cptr;
    }

    return str;
}

/*
* Returns true if c is a letter
*/
char is_alpha(char c)
{
    return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
}

/*
* Checks if a pointer is null. If it is,
* prints an error message and terminates.
*/
void check_malloc(void* ptr)
{
    if(!ptr)
    {
        fprintf(stderr, "Malloc failed. Exiting.\n");
        exit(1);
    }
}
