/// @file
/// @brief The json packer public header file

#ifndef JSON_PACKER_H
#define JSON_PACKER_H

/**
 * The input will contain records in JSON format. Each record is separated by a
 * new line (LF). The input will only contain simple types, complex and nested
 * data structures will not be present (i.e. map and array).
 * Input example:
 * { “key1”:“value”, “key2”:42, “key3”:TRUE}
 * {“sadsf”:”dsewtew”, “dsre”:3221, “sdfds”:”dsfewew”}
 * Basically a record is a list of KVPs (key-value pairs).
 * 
 * Once the input is read, the program should encode the input so that the keys
 * are put into a dictionary. The dictionary maps the keys (strings) to an
 * integer. The key strings are then replaced with a number.
 * The dictionary would be as follows for the first record:
 * “key1”:1, “key2”:2, “key3”:3
 * This record is then represented as:
 * { 1:“value”, 2:42, 3:TRUE}
 * 
 * Types supported by JSON must be handled and preserved (i.e. integer, string,
 * boolean).  The output should be dumped to a file in a binary form using TLV
 * encoding instead of the above text-based representation. This binary TLV
 * encoding is more compact, e.g. a boolean value can be encoded in two bytes
 * (or even less).
 * At the end of the process the dictionary must be also written to a file
 * using the same encoding method for the string-integer pairs.
 * 
 * @param input_fp  input file handler
 * @param output_values_fp  values output file handler
 * @param output_keys_fp  keys output file handler
 */
void process_input_file(FILE* input_fp, 
                        FILE* output_values_fp, 
                        FILE* output_keys_fp);

#endif // JSON_PACKER_H
