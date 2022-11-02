/// @file
/// @brief The implementation of json packer functionality

// system header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// own header files
#include <log.h>
#include <config.h>
#include "json-packer-dict.h"


// @brief parses given line as a json
// @param line to parse
// @param len the line size
yajl_val parse_line(char *line, size_t len)
{
    char errbuf[MAX_LINE_SIZE];

    // we have the whole line in memory.  let's parse it
    yajl_val node = yajl_tree_parse((const char *)line, errbuf, sizeof(errbuf));
    if (node == NULL)
    {
        log_error("Error! Can't parse the input file at %s", INPUT_FILE_PATH);
        if (strlen(errbuf))
        {
            log_error(" %s", errbuf);
        }
        else
        {
            log_error("unknown error");
        }
        exit(EXIT_FAILURE);
    }
    return node;
}


void free_keys(json_key_data_array_t all_keys)
{
    for (size_t i = 0; i < all_keys.size; ++i) {
        free(all_keys.items[i].key);
    }
    free(all_keys.items);
}

void free_values(json_value_data_array_t all_values)
{
    for (size_t i = 0; i < all_values.size; ++i) {
        if (all_values.items[i].type == JSON_STR_TYPE) {
            free(all_values.items[i].data.s);
        }
    }
    free(all_values.items);
}

void save_json_keys(json_key_data_array_t all_keys, FILE* output_keys_fp)
{
    for (size_t i = 0; i < all_keys.size; i++)
    {
        const char* key = all_keys.items[i].key;
        fputc(JSON_STR_TYPE, output_keys_fp);
        fputc(strlen(key), output_keys_fp);
        fputs(key, output_keys_fp);
    }
}

void save_json_values(json_value_data_array_t all_values, FILE* output_values_fp)
{
    for (size_t i = 0; i < all_values.size; i++)
    {
        json_value_data_t* val = all_values.items + i;
        int type = val->type;
        size_t length;
        const void* value;
        
        if (val->type == JSON_DOUBLE_TYPE)
        {
            length = sizeof(double);
            value = &val->data.d;
            log_trace("store: [%d,%d,%f]", type, length, val->data.d);
            
            // type
            fwrite((const void *)&type, sizeof(int), 1, output_values_fp);
            // length
            fwrite((const void *)&length, sizeof(int), 1, output_values_fp); 
            // value
            fwrite(value, length, 1, output_values_fp);   
        }
        else if (val->type == JSON_INT_TYPE)
        {
            length = sizeof(int);
            value = &val->data.i;
            log_trace("store: [%d,%d,%d]", type, length, val->data.i);
            
            // type
            fwrite((const void *)&type, sizeof(int), 1, output_values_fp);
            // length
            fwrite((const void *)&length, sizeof(int), 1, output_values_fp);
            // value
            fwrite(value, length, 1, output_values_fp);
            
        }
        else if (val->type == JSON_STR_TYPE)
        {
            value = val->data.s;
            length = strlen(value);
            log_trace("store: [%d,%d,%s]", type, length, val->data.s);
            
            // type
            fwrite((const void *)&type, sizeof(int), 1, output_values_fp);
            // length
            fwrite((const void *)&length, sizeof(int), 1, output_values_fp);
            // value
            fwrite(value, length, 1, output_values_fp);
        }
        else if (val->type == JSON_BOOL_TYPE_TRUE)
        {
            log_trace("store: [%d]", type);
            // type
            fwrite((const void *)&type, sizeof(int), 1, output_values_fp); 
        }
        else if (val->type == JSON_BOOL_TYPE_FALSE)
        {
            log_trace("store: [%d]", type);
            // type
            fwrite((const void *)&type, sizeof(int), 1, output_values_fp); 
        }
    }
}

// @brief This function processes given input file handler and 
//        produces keys and values output files.
void process_input_file(FILE* input_fp, FILE* output_values_fp, 
                        FILE* output_keys_fp)
{
    log_info("Parsing the input");
    ssize_t read;
    char *line = (char*) malloc(MAX_LINE_SIZE);
    size_t len = MAX_LINE_SIZE;

    while ((read = getline(&line, &len, input_fp)) != -1)
    {
        // parse json
        yajl_val node = parse_line(line, len);

        log_info("Retrieved line of length %zu:\n", read);
        log_info("%s", line);

        // serialize parsed nodes
        json_key_data_array_t all_keys = json_parse_keys(node);
        json_value_data_array_t all_values = json_parse_values(all_keys, node);

        save_json_keys(all_keys, output_keys_fp);
        save_json_values(all_values, output_values_fp);

        // cleanup
        free_keys(all_keys);
        free_values(all_values);

        yajl_tree_free(node);
    }

    // cleanup
    free(line);
}
