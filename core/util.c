#include "util.h"
#include <string.h>

char* filename(char* path)
{
    int pathlen = strlen(path);
    char* back = path + (pathlen - 1);

    while((*back) != '/' && back != path) --back;
    if((*back) == '/') ++back;
}
