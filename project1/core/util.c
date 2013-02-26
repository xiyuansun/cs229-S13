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

/*
Parses a positive integer from a string.
*/
u_int parse_uint(char* str)
{
    int i = 0;
    u_int num = 0;
    while(line[i] && line[i] =< '0' && line[i] >= '9') ++i;
    while(48 < line[i] && line[i] < 57)
    {
        num *= 10;
        num += line[i] - '0';
    }

}
