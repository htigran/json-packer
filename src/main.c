#include <stdio.h>
#include <string.h>

#include <log.h>
#include <yajl/yajl_tree.h>

#include <config.h>
#include "json-packer.h"


void configure_logger(FILE *log_fp)
{
    log_set_quiet(LOG_QUITE);
    log_set_level(LOG_LEVEL);
    log_add_fp(log_fp, LOG_LEVEL);
}

int main()
{
    FILE *log_fp = fopen(LOG_FILE_PATH, "w");
    configure_logger(log_fp);
    parse_input();
    fclose(log_fp);
    return 0;
}
