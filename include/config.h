
#ifndef CONFIG_H
#define CONFIG_H

#define INPUT_FILE_PATH "input.json"
#define OUTPUT_FILE_PATH "output.tlv"

/* One of the following
 * LOG_TRACE
 * LOG_DEBUG
 * LOG_INFO
 * LOG_WARN
 * LOG_ERROR
 * LOG_FATAL
 */
#define LOG_LEVEL LOG_TRACE
#define LOG_FILE_PATH "log.txt"
#define LOG_QUITE false
// to disable coloring comment out the following line
#define LOG_USE_COLOR


#define TLV_TYPE_DOUBLE 0x00
#define TLV_TYPE_INT 0x01
#define TLV_TYPE_STR 0x02
#define TLV_TYPE_BOOL 0x03

#define MAX_LINE_SIZE 100

#endif // CONFIG_H
