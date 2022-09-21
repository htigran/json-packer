
#ifndef CONFIG_H
#define CONFIG_H

#define INPUT_FILE_PATH "input.json"

/* Available log levels are
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

#endif //CONFIG_H
