
#include <stdio.h>
#include <string.h>

#include <yajl/yajl_tree.h>

#include "config-parser.h"


int main()
{
    const char* input_path = get_input_file_path();
    if (NULL == input_path) {
        return 1;
    }
    printf("%s \n", input_path);
    return 0;
}
