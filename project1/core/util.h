#ifndef UTIL_H
#define UTIL_H

#include "types.h"

/*
* Macros for limiting values
*/
#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a > b) ? b : a
#define LIMIT(a, b, c) MAX(MIN(a, b), c)

/*
Parses the file name from a path string
*/
char* get_filename(char* path);

/*
Converts a string to all upper-case characters
*/
char* to_upper(char* str);

/*
* Returns true if c is a letter
*/
char is_alpha(char c);

/*
* Checks if a pointer is null. If it is,
* prints an error message and terminates.
*/
void check_malloc(void* ptr);

/*
* Reads bytes bytes from file and
* returns them in little-endian order
*/
u_int read_little_dat(FILE* file, int bytes);

/*
* Writes bytes bytes from dat to out
*/
void write_bytes(FILE* out, char* dat, int bytes, char should_free);

/*
* Fills a character array with bytes from dat
*/
char* to_little_char_arr(int dat, int bytes);

/*
* Parses an integer from a string, and checks for errors
*/
u_int get_int(char* arg, const char* err_prefix);

/*
* Parses a real value from a string, and checks for errors
*/
double get_real(char* arg, const char* err_prefix);

/*
* Prints a detailed error message if i + 1 >= argc
*/
void check_bounds(int argc, int i, char* opt, const char* err_prefix);

#endif
