#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <log.h>
#include <yajl/yajl_tree.h>

#include <config.h>
#include "json-packer.h"

FILE* configure_logger()
{
    FILE *log_fp = fopen(LOG_FILE_PATH, "w+");
    if (log_fp == NULL)
    {
        log_error("Error! opening the log file at %s\n", LOG_FILE_PATH);
        exit(1);
    }
    log_set_quiet(LOG_QUITE);
    log_set_level(LOG_LEVEL);
    log_add_fp(log_fp, LOG_LEVEL);
    return log_fp;
}

FILE* configure_input()
{
    FILE *input_fp = fopen(INPUT_FILE_PATH, "rb");
    if (input_fp == NULL)
    {
        log_error("Error! opening the config file at %s\n", INPUT_FILE_PATH);
        exit(1);
    }
    return input_fp;
}

FILE* configure_output()
{
    FILE *output_fp = fopen(OUTPUT_FILE_PATH, "wb+");
    if (output_fp == NULL)
    {
        log_error("Error! opening the output file at %s\n", OUTPUT_FILE_PATH);
        exit(1);
    }
    return output_fp;
}

int main()
{
    FILE* log_fp = configure_logger();
    FILE* input_fp = configure_input();
    FILE* output_fp = configure_output();

    parse_input(input_fp, output_fp);


    fclose(log_fp);
    fclose(input_fp);
    fclose(output_fp);
    return 0;
}
