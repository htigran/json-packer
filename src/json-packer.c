/// @file
/// @brief The implementation of json packer functionality

// system header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// own header files
#include <log.h>
#include <tlv_box.h>
#include <config.h>
#include "json-packer-dict.h"


// @brief Stores given serialized tlv box in a file
// @param box  tlv box to store
// @param output_fp filepointer where to store
void tlv_box_store(tlv_box_t* box, FILE* output_fp)
{
    char* serialized_buffer = tlv_box_get_buffer(box);
    int buffer_size = tlv_box_get_size(box);
    // write into file
    size_t written_size = fwrite(serialized_buffer, sizeof(char),
                                 buffer_size, output_fp);
    if (written_size != buffer_size) 
    {
        log_error(
            "Error! can't write to output file! (%d out of %d is written)", 
            written_size, buffer_size);
        exit(EXIT_FAILURE);
    } else {
        log_info("Written %d bytes", written_size);
    }
    // we add newline to distinguish lines
    fputc('\n', output_fp);
}

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

        tlv_box_t* box_keys = box_serialize_keys(node);
        tlv_box_t* box_values = box_serialize_values(node);

        // store serialized boxes in files
        tlv_box_store(box_values, output_values_fp);
        tlv_box_store(box_keys, output_keys_fp);

        // cleanup
        free_keys(all_keys);
        free_values(all_values);

        tlv_box_destroy(box_values);
        tlv_box_destroy(box_keys);
        yajl_tree_free(node);
    }

    // cleanup
    free(line);
}
