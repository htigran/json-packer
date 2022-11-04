// system headers
#include <unity.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>

// own header files
#include <log.h>
#include <yajl/yajl_tree.h>
#include <json-packer.h>
#include <config.h>

// for testing only
#include "json-packer.c"
#include "json-packer-dict.c"

// functions array of two.
void (*test_by_line[2])(char*, int);

void parse_test_line(char* result, const char* buffer)
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

// ====================== TEST VALUES ========================

// verify 1 line of the input (Values)
void test_values_line_1(char* buffer, int lenght)
{
    printf("values 1: %s\n", buffer);
    const char expected[] = "[3,5,value][2,4,42][4,TRUE]";
    char* actual = malloc(sizeof(expected));
    parse_test_line(actual, buffer);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

// verify 2nd line of input (Values)
void test_values_line_2(char* buffer, int lenght)
{
    printf("values 1: %s\n", buffer);
    const char expected[] = "[3,7,dsewtew][2,4,-107][3,7,dsfewew]";
    char* actual = malloc(sizeof(expected));
    parse_test_line(actual, buffer);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

// open output file and check line by line (Values)
void test_verify_values()
{
    test_by_line[0] = test_values_line_1;
    test_by_line[1] = test_values_line_2;
    
    FILE *output_values_fp = fopen(OUTPUT_VALUES_FILE_PATH, "rb");
    if (output_values_fp == NULL) {
        printf("=== > ERROR opening the file at %s", OUTPUT_VALUES_FILE_PATH);
        exit(EXIT_FAILURE);
    }

    int lenght = 0;
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof(char));
    size_t len = MAX_LINE_SIZE;
    int test_id = 0;
    while ((lenght = getline(&line, &len, output_values_fp)) != -1)
    {
        printf("Running test #%d ", test_id + 1);
        (*test_by_line[test_id])(line, lenght);
        test_id++;
    }

    free(line);
    fclose(output_values_fp);
}

// ====================== TEST KEYS ========================

// verify 1 line of the input (Keys)
void test_keys_line_1(char* buffer, int lenght)
{
    printf("keys 1: %s\n", buffer);
}

// verify 2nd line of the input (Keys)
void test_keys_line_2(char* buffer, int lenght)
{
    printf("keys 2: %s\n", buffer);
}

// open output file and check line by line (Keys)
void test_verify_keys()
{
    test_by_line[0] = test_keys_line_1;
    test_by_line[1] = test_keys_line_2;
    
    FILE *output_keys_fp = fopen(OUTPUT_KEYS_FILE_PATH, "rb");
    if (output_keys_fp == NULL) {
        printf("=== > ERROR opening the file at %s", OUTPUT_KEYS_FILE_PATH);
        exit(EXIT_FAILURE);
    }

    int lenght = 0;
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof(char));
    size_t len = MAX_LINE_SIZE;
    int test_id = 0;
    while ((lenght = getline(&line, &len, output_keys_fp)) != -1)
    {
        printf("Running test #%d ", test_id + 1);
        (*test_by_line[test_id])(line, lenght);
        test_id++;
    }

    free(line);
    fclose(output_keys_fp);
}

// automatically called before each test
void setUp(void)
{
}

// automatically called after each test
void tearDown(void)
{
}

// not needed when using generate_test_runner.rb
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_verify_values);
    RUN_TEST(test_verify_keys);
    return UNITY_END();
}
