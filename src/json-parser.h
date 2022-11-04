/// @file
/// @brief The header file of json packer dictionary

#ifndef JSON_PACKER_DICT_H
#define JSON_PACKER_DICT_H

// TODO see if we can get rid of this include
#include <yajl/yajl_tree.h>
#include <stdbool.h>

#define JSON_DOUBLE_TYPE 1
#define JSON_INT_TYPE 2
#define JSON_STR_TYPE 3
#define JSON_BOOL_TYPE_TRUE 4
#define JSON_BOOL_TYPE_FALSE 5

typedef struct json_key_data_type
{
    char* key;
    int data;
} json_key_data_t;

typedef struct json_key_data_array_type
{
    json_key_data_t* items;
    size_t size;
} json_key_data_array_t;

typedef struct json_value_data_type
{
    int id;
    int type;
    union {
        char* s;
        int i;
        bool b;
        double d;
    } data;
} json_value_data_t;

typedef struct json_value_data_array_type
{
    json_value_data_t* items;
    size_t size;
} json_value_data_array_t;

// @brief parses a yajl tree root node
// @param node - yajl (json) node
// @return 
json_key_data_array_t json_parse_keys(yajl_val node);

// @brief parses a yajl tree root node
// @param node - yajl (json) node
// @return 
json_value_data_array_t json_parse_values(json_key_data_array_t all_keys, yajl_val node);

#endif // JSON_PACKER_DICT_H
