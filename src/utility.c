
#include "utility.h"

#include <stdlib.h>
#include <string.h>

char* make_str_copy(const char* src)
{
    char* dest = malloc(strlen(src) * sizeof(char));
    strcpy(dest, src);
    return dest;
}
