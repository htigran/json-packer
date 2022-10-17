
// includes from our own projects
#include <config.h>

// installed libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// built by us
#include <log.h>
#include <tlv_box.h>

#include "json-packer-dict.h"

void tlv_box_store(tlv_box_t* box, FILE* output_fp)
{
    char* serialized_buffer = tlv_box_get_buffer(box);
    
    int buffer_size = tlv_box_get_size(box);
    size_t written_size = fwrite(serialized_buffer, sizeof(char),
                                buffer_size, output_fp);
    
    if (written_size != buffer_size) 
    {
        log_error(
            "Error! can't write to output file! (%d out of %d is written)", 
            written_size, buffer_size);
        exit(1);
    } else {
        log_info("Written %d bytes", written_size);
    }
    
    fputc('\n', output_fp);
}

/// @brief
/// @param line
/// @param len
yajl_val parse_line(char *line, size_t len)
{
    yajl_val node;
    char errbuf[1024];

    /* we have the whole config file in memory.  let's parse it ... */
    node = yajl_tree_parse((const char *)line, errbuf, sizeof(errbuf));
    /* parse error handling */
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
        exit(1);
    }
    return node;
}

/// @brief
/// @return
int parse_input(FILE* input_fp, FILE* output_fp)
{
    log_info("Parsing the input");
    ssize_t read;
    char *line = (char*) malloc(MAX_LINE_SIZE);
    size_t len = MAX_LINE_SIZE;

    while ((read = getline(&line, &len, input_fp)) != -1)
    {
        yajl_val node = parse_line(line, len);
        log_info("Retrieved line of length %zu:\n", read);
        log_info("%s", line);

        tlv_box_t* box = dict_serialize(node);
        tlv_box_store(box, output_fp);
        tlv_box_destroy(box);
        yajl_tree_free(node);
        dict_reset();
    }
    free(line);
    return 0;
}
