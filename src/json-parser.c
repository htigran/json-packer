
#include "json-parser.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <yajl/yajl_tree.h>

const char* config_file_path = "config.json";

struct config_t {
    char input_file_path[255];
} config;

const char* read_config_file()
{
    FILE *f = fopen(config_file_path, "rb");
    if(f == NULL)
    {
        fprintf(stderr, "Error! opening the config file at %s\n", config_file_path);   
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *fileData = malloc(fsize + 1);
    fread(fileData, fsize, 1, f);
    fclose(f);

    fileData[fsize] = 0;
    return fileData;
}

void parse_config_file() 
{
    const char* fileData = read_config_file();
    size_t rd;
    yajl_val node;
    char errbuf[1024];

    /* we have the whole config file in memory.  let's parse it ... */
    node = yajl_tree_parse((const char *) fileData, errbuf, sizeof(errbuf));
    /* parse error handling */
    if (node == NULL) {
        fprintf(stderr, "Error! Can't parse the config file at %s", config_file_path);
        if (strlen(errbuf)) fprintf(stderr, " %s", errbuf);
        else fprintf(stderr, "unknown error");
        fprintf(stderr, "\n");
        exit(1);             
    }
    /* ... and extract a value from the config file */
    {
        const char * path[] = { "input-file-path", (const char *) 0 };
        yajl_val v = yajl_tree_get(node, path, yajl_t_string);
        if (! v) {
            printf("Error! Config file has no node: %s\n", path[0]);
            exit(1);
        }
        strcpy(config.input_file_path, YAJL_GET_STRING(v));
    }
    yajl_tree_free(node);
    free(fileData);
}

const char* get_input_file_path()
{
    parse_config_file();
    return config.input_file_path;
}
