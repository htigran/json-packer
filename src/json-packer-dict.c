/// @file
/// @brief The source file of json packer dictionary

// system header files
#include <string.h>
#include <stdlib.h>

// own header files
#include <log.h>
#include <config.h>
#include "json-packer-dict.h"


char* make_str_copy(const char* src)
{
    char* dest = malloc(strlen(src) * sizeof(char));
    strcpy(dest, src);
    return dest;
}

// @brief parses a yajl tree root node
// @param node - yajl (json) node
// @return 
json_key_data_array_t json_parse_keys(yajl_val node)
{
    if ((node)->type != yajl_t_object)
    {
        log_error("Error! Unknown input format !");
        exit(EXIT_FAILURE);
    }
    log_trace("Parsing keys from json object");
    
    json_key_data_array_t result;
    size_t nelem = node->u.object.len;
    json_key_data_t* all_keys = malloc(nelem * sizeof(json_key_data_t));
    for (int i = 0; i < nelem; ++i)
    {
        all_keys[i].key = make_str_copy(node->u.object.keys[i]); // key
        all_keys[i].data = i + 1;
    }
    
    result.size = nelem;
    result.items = all_keys;
    log_info("All json keys are successfully parsed.");
    return result;
}


int json_key_find_idx(const char *key, json_key_data_array_t all_keys)
{
    for (size_t i = 0; i < all_keys.size; i++)
    {
        if (strcmp(key, all_keys.items[i].key) == 0)
        {
            return all_keys.items[i].data;
        }
    }
    return -1;
}

// @brief parses a yajl tree root node
// @param node - yajl (json) node
// @return
json_value_data_array_t json_parse_values(json_key_data_array_t all_keys, yajl_val node)
{
    if ((node)->type != yajl_t_object)
    {
        log_error("Error! Unknown input format!");
        exit(EXIT_FAILURE);
    }
    log_trace("Parsing values from json object.");
    
    json_value_data_array_t result;
    size_t nelem = node->u.object.len;
    json_value_data_t* all_values = malloc(nelem * sizeof(json_value_data_t));
    for (int i = 0; i < nelem; ++i)
    {
        const char *key = node->u.object.keys[i]; // key
        int key_idx = json_key_find_idx(key, all_keys);
        if (-1 == key_idx) {
            log_error("Internal Error. Cannot find key in json %s!", key);
            exit(EXIT_FAILURE);
        }
        all_values[i].id = key_idx;
        // values can be different types
        yajl_val val = node->u.object.values[i]; // val
        if (YAJL_IS_INTEGER(val))
        {
            log_trace("int: {%s:%d}", key, val->u.number.i);
            all_values[i].type = JSON_INT_TYPE;
            all_values[i].data.i = val->u.number.i;
        }
        else if (YAJL_IS_DOUBLE(val))
        {
            log_trace("double: {%s:%f}", key, val->u.number.d);
            all_values[i].type = JSON_DOUBLE_TYPE;
            all_values[i].data.d = val->u.number.d;
        }
        else if (YAJL_IS_STRING(val))
        {
            log_trace("string: {%s:%s}", key, val->u.string);
            all_values[i].type = JSON_STR_TYPE;
            all_values[i].data.s = make_str_copy(val->u.string);
        }
        else if (YAJL_IS_TRUE(val))
        {
            log_trace("bool: {%s:TRUE}", key, true);
            all_values[i].type = JSON_BOOL_TYPE_TRUE;
        }
        else if (YAJL_IS_FALSE(val))
        {
            log_trace("bool: {%s:FALSE}", key, false);
            all_values[i].type = JSON_BOOL_TYPE_FALSE;
        }
    }
    log_info("All json keys are successfully parsed.");
    result.size = nelem;
    result.items = all_values;
    return result;
}
