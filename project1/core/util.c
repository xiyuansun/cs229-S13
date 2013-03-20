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

/*
* Reads bytes bytes from file and
* returns them in little-endian order
*/
u_int read_little_dat(FILE* file, int bytes)
{   
    u_char c;
    int i = 0;
    u_int ret = 0;
    u_int not_ret = 0;
    while(i < bytes)
    {
        c = fgetc(file);

        not_ret = not_ret << 8;
        not_ret |= c;
        ret |= ((u_int) c) << (i * 8);
        ++i;
    }
    return ret;
}

/*
* Writes bytes bytes from dat to out
*/
void write_bytes(FILE* out, char* dat, int bytes, char should_free)
{
    int i;
    for(i = 0; i < bytes; ++i)
    {
        fputc(dat[i], out);
    }
    
    if(should_free)
    {
        free(dat);
    }
}

char* to_little_char_arr(int dat, int bytes)
{
    char* ret = malloc(sizeof(char) * bytes);
    check_malloc(ret);
    int i;
    for(i = 0; i < bytes; ++i)
    {
        ret[i] = (char) dat;
        dat = dat >> 8;
    }

    return ret;
}
