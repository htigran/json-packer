#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include <log.h>
#include <yajl/yajl_tree.h>

#include <config.h>
#include "json-packer.h"

#include <unistd.h>


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

FILE* configure_output_values()
{
    FILE *output_values_fp = fopen(OUTPUT_VALUES_FILE_PATH, "wb+");
    if (output_values_fp == NULL)
    {
        log_error("Error! opening the output values file at %s\n", OUTPUT_VALUES_FILE_PATH);
        exit(1);
    }
    return output_values_fp;
}

FILE* configure_output_keys()
{
    FILE *output_keys_fp = fopen(OUTPUT_KEYS_FILE_PATH, "wb+");
    if (output_keys_fp == NULL)
    {
        log_error("Error! opening the output keys file at %s\n", OUTPUT_KEYS_FILE_PATH);
        exit(1);
    }
    return output_keys_fp;
}

void print_info()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        log_info("Current working dir: %s\n", cwd);
    } else {
        log_error("getcwd() error");
    }
}

int main()
{
    print_info();
    
    FILE* log_fp = configure_logger();
    FILE* input_fp = configure_input();
    FILE* output_values_fp = configure_output_values();
    FILE* output_keys_fp = configure_output_keys();

    parse_input(input_fp, output_values_fp, output_keys_fp);

    fclose(log_fp);
    fclose(input_fp);
    fclose(output_values_fp);
    return 0;
}
