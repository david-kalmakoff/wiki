/**
 * @file
 * @brief Logging library
 *
 * This library provides a simple logging system that can be used to print
 * messages to stdout with colors depending on the log level.
 *
 * The log level can be set by defining the LOG_LEVEL macro before including
 * this file.
 *
 * The log level can be one of the following:
 *   - VERBOSE (-8)
 *   - DEBUG (-4)
 *   - INFO (0)
 *   - WARN (4)
 *   - ERROR (8)
 *   - TODO (12)
 *
 * The log level can be set to any value between -8 and 12.
 *
 * The default log level is INFO.
 *
 * @author David Kalmakoff
 */

#include <stdio.h>
#include <time.h>

#ifndef LOGGING_H
#define LOGGING_H

/**
 * @brief Print a log level to stdout with colors before a log message
 *
 * @param level
 * @param file
 * @param function
 * @param line
 */
void log_pre_print(int level, const char *file, const char *function, int line);

/**
 * @brief Print to stdout after a log message
 */
void log_post_print();

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define VERBOSE -8
#define DEBUG -4
#define INFO 0
#define WARN 4
#define ERROR 8
#define TODO 12

#ifndef LOG_LEVEL
#define LOG_LEVEL INFO
#endif

#define LOG(level) ((level) >= (LOG_LEVEL))

#define PRINTF(level, format, ...)                                             \
  ((level) >= (LOG_LEVEL) ? ({                                                 \
    log_pre_print(level, __FILE__, __FUNCTION__, __LINE__);                    \
    printf(format, __VA_ARGS__);                                               \
    log_post_print();                                                          \
  })                                                                           \
                          : ({}))
#define PRINT(level, format)                                                   \
  ((level) >= (LOG_LEVEL) ? ({                                                 \
    log_pre_print(level, __FILE__, __FUNCTION__, __LINE__);                    \
    printf(format);                                                            \
    log_post_print();                                                          \
  })                                                                           \
                          : ({}))

#define ASSERTF(check, format, ...)                                            \
  ({                                                                           \
    if (!(check)) {                                                            \
      PRINTF(ERROR, format, __VA_ARGS__);                                      \
      assert(0);                                                               \
    }                                                                          \
  })

#define ASSERT(format)                                                         \
  ({                                                                           \
    if (!(check)) {                                                            \
      PRINTF(ERROR, "ASSERT: %s", format);                                     \
      assert(0);                                                               \
    }                                                                          \
  })

#define TODO_ASSERT(format)                                                    \
  ({                                                                           \
    PRINTF(TODO, "TODO: %s", format);                                          \
    assert(0);                                                                 \
  })

#define TODO_RETURN(format)                                                    \
  ({                                                                           \
    PRINTF(TODO, "TODO: %s", format);                                          \
    return FALSE;                                                              \
  })

#endif
