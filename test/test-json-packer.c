#include <unity.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include <log.h>
#include <yajl/yajl_tree.h>
#include <tlv_box.h>
#include <json-packer.h>
#include <config.h>

#include "json-packer.c"
#include "json-packer-dict.c"


void (*test_by_line[2])(char*, int);

// ====================== TEST VALUES ========================

void test_values_line_1(char* line, int read)
{
    tlv_box_t* box = tlv_box_parse(line, read - 1);
    double read_double = (double)0.0;
    tlv_box_get_double(box, TLV_TYPE_DOUBLE, &read_double);
    TEST_ASSERT_EQUAL_DOUBLE(42.0, read_double);
    
    int size = 6;
    char* read_str = malloc(size);
    tlv_box_get_string(box, TLV_TYPE_STR, read_str, &size);
    TEST_ASSERT_EQUAL_STRING_LEN("value", read_str, size);
    TEST_ASSERT_EQUAL_STRING("value", read_str);
    
    bool test_bool;
    tlv_box_get_bool(box, TLV_TYPE_BOOL, &test_bool);
    TEST_ASSERT_EQUAL(true, test_bool);
    
    log_info("Retrieved line of length %zu:\n", read);
    
    tlv_box_destroy(box);
}

void test_values_line_2(char* line, int read)
{
    tlv_box_t* box = tlv_box_parse(line, read - 1);
    double read_double = (double)0.0;
    tlv_box_get_double(box, TLV_TYPE_DOUBLE, &read_double);
    TEST_ASSERT_EQUAL_DOUBLE(3221, read_double);
    
    int size = 8;
    char* read_str = malloc(size);
    tlv_box_get_string(box, TLV_TYPE_STR, read_str, &size);
    TEST_ASSERT_EQUAL_STRING_LEN("dsewtew", read_str, size);
    
    log_info("Retrieved line of length %zu:\n", read);
    
    tlv_box_destroy(box);
}

void test_verify_values()
{
    test_by_line[0] = test_values_line_1;
    test_by_line[1] = test_values_line_2;
    
    FILE *output_values_fp = fopen(OUTPUT_VALUES_FILE_PATH, "rb");
    if (output_values_fp == NULL) {
        printf("=== > ERROR opening the file at %s", OUTPUT_VALUES_FILE_PATH);
        exit(1);
    }

    int read = 0;
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof(char));
    size_t len = MAX_LINE_SIZE;
    int test_id = 0;
    while ((read = getline(&line, &len, output_values_fp)) != -1)
    {
        printf("Running test #%d ", test_id + 1);
        (*test_by_line[test_id])(line, read);
        test_id++;
    }

    free(line);
    fclose(output_values_fp);
}

// ====================== TEST KEYS ========================

void test_keys_line_1(char* line, int read)
{
    tlv_box_t* box = tlv_box_parse(line, read - 1);
    int size = 5;
    char* read_str = malloc(size);
    tlv_box_get_string(box, TLV_TYPE_STR, read_str, &size);
    TEST_ASSERT_EQUAL_STRING_LEN("key1", read_str, size);
    
    log_info("Retrieved line of length %zu:\n", read);
    
    tlv_box_destroy(box);
}

void test_keys_line_2(char* line, int read)
{
    tlv_box_t* box = tlv_box_parse(line, read - 1);
    int size = 6;
    char* read_str = malloc(size);
    tlv_box_get_string(box, TLV_TYPE_STR, read_str, &size);
    TEST_ASSERT_EQUAL_STRING_LEN("sadsf", read_str, size);
    
    log_info("Retrieved line of length %zu:\n", read);
    
    tlv_box_destroy(box);
}

void test_verify_keys()
{
    test_by_line[0] = test_keys_line_1;
    test_by_line[1] = test_keys_line_2;
    
    FILE *output_keys_fp = fopen(OUTPUT_KEYS_FILE_PATH, "rb");
    if (output_keys_fp == NULL) {
        printf("=== > ERROR opening the file at %s", OUTPUT_KEYS_FILE_PATH);
        exit(1);
    }

    int read = 0;
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof(char));
    size_t len = MAX_LINE_SIZE;
    int test_id = 0;
    while ((read = getline(&line, &len, output_keys_fp)) != -1)
    {
        printf("Running test #%d ", test_id + 1);
        (*test_by_line[test_id])(line, read);
        test_id++;
    }

    free(line);
    fclose(output_keys_fp);
}

void setUp(void) {

}

void tearDown(void) {
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_verify_values);
    RUN_TEST(test_verify_keys);
    return UNITY_END();
}
