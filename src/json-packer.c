/// @file
/// @brief The implementation of json packer functionality

// system header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// own header files
#include <log.h>
#include <config.h>
#include "json-parser.h"
#include "tlv.h"
#include "utility.h"


// @brief parses given line as a json
// @param line to parse
// @param len the line size
yajl_val parse_json_string(char *line, size_t len)
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
        yajl_val node = parse_json_string(line, len);

        log_info("Retrieved line of length %zu:\n", read);
        log_info("%s", line);

        // serialize parsed nodes
        json_key_data_array_t all_keys = json_parse_keys(node);
        json_value_data_array_t all_values = json_parse_values(all_keys, node);

        encode_keys(all_keys, output_keys_fp);
        encode_values(all_values, output_values_fp);

        // cleanup
        free_keys(all_keys);
        free_values(all_values);

        yajl_tree_free(node);
    }

    // cleanup
    free(line);
}
