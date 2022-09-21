
// includes from our own projects
#include <config.h>

// installed libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// built by us
#include <log.h>
#include <yajl/yajl_tree.h>

/// @brief
/// @param line
/// @param len
void parse_line(char *line, size_t len)
{
    yajl_val node;
    char errbuf[1024];

    /* we have the whole config file in memory.  let's parse it ... */
    node = yajl_tree_parse((const char *)line, errbuf, sizeof(errbuf));
    /* parse error handling */
    if (node == NULL)
    {
        log_trace("Error! Can't parse the input file at %s", INPUT_FILE_PATH);
        if (strlen(errbuf))
        {
            log_trace(" %s", errbuf);
        }
        else
        {
            log_trace("unknown error");
        }
        log_trace("\n");
        exit(1);
    }
    if (node != NULL)
    {
        if ((node)->type == yajl_t_object)
        {
            log_trace("is object\n");
            size_t nelem = node->u.object.len;
            int ii;
            for (ii = 0; ii < nelem; ++ii)
            {
                // key is just char *
                const char *key = node->u.object.keys[ii]; // key
                // values can be different types
                yajl_val val = node->u.object.values[ii]; // val
                if (YAJL_IS_DOUBLE(val))
                {
                    log_trace("double: %s/%f\n", key, val->u.number.d);
                }
                else if (YAJL_IS_INTEGER(val))
                {
                    log_trace("int: %s/%d\n", key, val->u.number.i);
                }
                else if (YAJL_IS_STRING(val))
                {
                    log_trace("string: %s/%s\n", key, val->u.string);
                }
                else if (YAJL_IS_TRUE(val))
                {
                    log_trace("bool: %s/%B\n", key, true);
                }
                else if (YAJL_IS_FALSE(val))
                {
                    log_trace("bool: %s/%B\n", key, false);
                }
            }
        }
    }
    else
    {
        log_error("Error! Unknown input file format");
        exit(1);
    }
    yajl_tree_free(node);
}

int parse_input()
{
    log_trace("Parsing the input");
    FILE *fp = fopen(INPUT_FILE_PATH, "rb");
    ssize_t read;
    char *line = NULL;
    size_t len = 0;

    if (fp == NULL)
    {
        log_trace("Error! opening the config file at %s\n", INPUT_FILE_PATH);
        exit(1);
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        parse_line(line, len);
        log_trace("Retrieved line of length %zu:\n", read);
        log_trace("%s", line);
    }

    fclose(fp);

    return 0;
}