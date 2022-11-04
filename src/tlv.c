
#include "tlv.h"
#include "json-parser.h"

#include <log.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void encode_keys(json_key_data_array_t all_keys, FILE* output_keys_fp)
{
    for (size_t i = 0; i < all_keys.size; i++)
    {
        const char* key = all_keys.items[i].key;
        fputc(JSON_STR_TYPE, output_keys_fp);
        fputc(strlen(key), output_keys_fp);
        fputs(key, output_keys_fp);
    }
    fputc('\n', output_keys_fp);
}

void encode_values(json_value_data_array_t all_values, 
                        FILE* output_values_fp)
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
    fputc('\n', output_values_fp);
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

void decode_values(char* result, const char* buffer)
{
    size_t offset = 0;
    int type, length;
    char* value;
    char* result_ptr = result;
    while ((char)*(buffer+offset) != '\n')
    {
        memcpy(&type, buffer+offset, sizeof(int));
        offset += sizeof(int);
        if (type != JSON_BOOL_TYPE_TRUE && type != JSON_BOOL_TYPE_FALSE)
        {
            memcpy(&length, buffer+offset, sizeof(int));
            offset += sizeof(int);
            value = malloc(length);
            memcpy(value, buffer+offset, length);
            offset += length;
        }
        
        if (type == JSON_INT_TYPE)
        {
            sprintf(result_ptr, "[%d,%d,%d]", type, length, (int)*value);
        }
        else if (type == JSON_DOUBLE_TYPE)
        {
            sprintf(result_ptr, "[%d,%d,%f]", type, length, (double)*value);
        }
        else if (type == JSON_STR_TYPE)
        {
            sprintf(result_ptr, "[%d,%d,%s]", type, length, (char *)value);
        }
        else if (type == JSON_BOOL_TYPE_TRUE)
        {
            sprintf(result_ptr, "[%d,TRUE]", type);
        }
        else if (type == JSON_BOOL_TYPE_FALSE)
        {
            sprintf(result_ptr, "[%d,FALSE]", type);
        }
        if (type != JSON_BOOL_TYPE_TRUE && type != JSON_BOOL_TYPE_FALSE)
        {
            free(value);
        }
        result_ptr += strlen(result_ptr);
    }
}
