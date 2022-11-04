
#ifndef JSON_PACKER_TLV_H
#define JSON_PACKER_TLV_H

#include "json-parser.h"

#include <stdio.h>



void encode_keys(json_key_data_array_t all_keys, FILE* output_keys_fp);

void encode_values(json_value_data_array_t all_values, 
                        FILE* output_values_fp);

void free_keys(json_key_data_array_t all_keys);

void free_values(json_value_data_array_t all_values);

void decode_values(char* result, const char* buffer);


#endif // JSON_PACKER_TLV_H
