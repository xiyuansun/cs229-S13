#include "util.h"
#include <stdlib.h>
#include <string.h>

/*
Parses the file name from a path string
*/
char* get_filename(char* path)
{
    int pathlen = strlen(path);
    char* begin_name = path + (pathlen - 1);
    char* end_name = path + (pathlen - 1);

    while((*begin_name) != '/' && begin_name != path) --begin_name;
    if((*begin_name) == '/') ++begin_name;

    while((*end_name) != '.' && end_name != begin_name) --end_name;
    if((*end_name) != '.') end_name = path + pathlen;
    
    int size = end_name - begin_name + 1;
    char* ret = malloc(size * sizeof(char));

    return strncpy(ret, begin_name, size-1);
}

/*
Converts a string to all upper-case characters
*/
char* to_upper(char* str)
{
    char* cptr = str;
    while(cptr)
    {
        if('a' <= (*cptr) && (*cptr) <= 'z')
            (*cptr) -= 32;

        ++cptr;
    }

    return str;
}
