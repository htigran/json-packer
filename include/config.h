/// @file
/// @brief Configuration

#ifndef CONFIG_H
#define CONFIG_H

// input and output files configuration
#define INPUT_FILE_PATH          "input.json"
#define OUTPUT_KEYS_FILE_PATH    "keys.tlv"
#define OUTPUT_VALUES_FILE_PATH  "values.tlv"

// logger configuration
/* possible log levels are
 *     LOG_TRACE
 *     LOG_DEBUG
 *     LOG_INFO
 *     LOG_WARN
 *     LOG_ERROR
 *     LOG_FATAL
 */
#define LOG_LEVEL     LOG_TRACE
#define LOG_FILE_PATH "log.txt"
#define LOG_QUITE     false // only prints in file not in terminal
#define LOG_USE_COLOR       // use colouring for log lines

// Maximum input line size. Each line contains a single Json document.
#define MAX_LINE_SIZE 100

#endif // CONFIG_H
