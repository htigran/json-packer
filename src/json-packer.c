
// includes from our own projects
#include <config.h>

// installed libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// built by us
#include <yajl/yajl_tree.h>

/// @brief 
/// @param line 
/// @param len 
void parse_line(char* line, size_t len) 
{
    yajl_val node;
    char errbuf[1024];

    /* we have the whole config file in memory.  let's parse it ... */
    node = yajl_tree_parse((const char *) line, errbuf, sizeof(errbuf));
    /* parse error handling */
    if (node == NULL) {
        fprintf(stderr, "Error! Can't parse the input file at %s", INPUT_PATH);
        if (strlen(errbuf)) {
            fprintf(stderr, " %s", errbuf);
        } else {
            fprintf(stderr, "unknown error");
        }
        fprintf(stderr, "\n");
        exit(1);             
    }
    if (node != NULL) {
        if ((node)->type == yajl_t_object) {
            printf( "is object\n" );
            size_t nelem = node->u.object.len;
            int ii;
            for ( ii = 0; ii < nelem; ++ii ) {
                // key is just char *
                const char * key = node->u.object.keys[ ii ]; // key
                // values can be different types
                yajl_val val = node->u.object.values[ ii ];   // val
                if ( YAJL_IS_DOUBLE( val ) ) {
                    printf( "double: %s/%f\n", key, val->u.number.d );
                }  else if(YAJL_IS_INTEGER( val )) {
                    printf( "int: %s/%d\n", key, val->u.number.i );
                }  else if(YAJL_IS_STRING( val )) {
                    printf( "string: %s/%s\n", key, val->u.string );
                }  else if(YAJL_IS_TRUE( val )) {
                    printf( "bool: %s/%B\n", key, true );
                }  else if(YAJL_IS_FALSE( val )) {
                    printf( "bool: %s/%B\n", key, false );
                }
            }
        }
    } else {
        printf("Error! Unknown input file format");
        exit(1);
    }
    yajl_tree_free(node);
}


int parse_input()
{
    FILE *fp = fopen(INPUT_PATH, "rb");
    ssize_t read;
    char * line = NULL;
    size_t len = 0;


    if(fp == NULL)
    {
        fprintf(stderr, "Error! opening the config file at %s\n", INPUT_PATH);
        exit(1);             
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        parse_line(line, len);
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }

    fclose(fp);

    return 0;
}
