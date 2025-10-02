#pragma once
#include "shared.h"
#include <stdarg.h>

typedef enum _logging_Level {
    LOGGING_LEVEL_PANIC,
    LOGGING_LEVEL_ERROR,
    LOGGING_LEVEL_WARN,
    LOGGING_LEVEL_INFO,
    LOGGING_LEVEL_DEBUG
} logging_Level;

typedef void (*logging_log_fn)(u8 c);

#define LOG_PANIC(s, ...)                                                                          \
    logging_log(LOGGING_LEVEL_PANIC, STATIC_STRING(s), __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(s, ...)                                                                          \
    logging_log(LOGGING_LEVEL_ERROR, STATIC_STRING(s), __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(s, ...)                                                                           \
    logging_log(LOGGING_LEVEL_WARN, STATIC_STRING(s), __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(s, ...)                                                                           \
    logging_log(LOGGING_LEVEL_INFO, STATIC_STRING(s), __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(s, ...)                                                                          \
    logging_log(LOGGING_LEVEL_DEBUG, STATIC_STRING(s), __FILE__, __LINE__, ##__VA_ARGS__)

void logging_log(logging_Level level, shared_StaticString fmt, const char *file, int line, ...);
void logging_set_log_fn(logging_log_fn fn);
