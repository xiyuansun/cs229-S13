#ifndef UTIL_H
#define UTIL_H

#include "types.h"

#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a > b) ? b : a
#define LIMIT(a, b, c) MAX(MIN(a, b), c)

char* get_filename(char* path);
char* to_upper(char* str);
u_int parse_uint(char* str);
char is_alpha(char c);
void check_malloc(void* ptr);
u_int read_little_dat(FILE* file, int bytes);
void write_bytes(FILE* out, char* dat, int bytes, char should_free);
char* to_little_char_arr(int dat, int bytes);
u_int get_int(char* arg, const char* err_prefix);
double get_real(char* arg, const char* err_prefix);
void check_bounds(int argc, int i, char* opt, const char* err_prefix);

#endif
