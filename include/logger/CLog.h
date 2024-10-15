/*
 * MIT License
 *
 * Copyright (c) 2024 Subhadip Roy Chowdhury
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file CLog.h
 * \brief A simple and lightweight logging library routine provider for C.
 *
 * These logging library macros and functions provides a way to output log
 * messages with different severity levels, such as DEBUG, INFO, WARN, and
 * ERROR. The logs can be printed with or without colors based on the user's
 * preference. The library also includes timestamping for log entries.
 *
 * The logging system is designed to be lightweight and easy to integrate into C
 * projects. It can be easily enabled or disabled for debugging purposes through
 * functions.
 *
 * Example usage:
 * \code
 * #include <cstd/logger/CLog.h> // Make sure that the include path is set.
 *
 * int main() {
 *     enable_colors(); // Enable ANSI-colored output
 *     CLog(INFO, "Application started successfully.");
 *     CLog(DEBUG, "Debugging some variables: x=%d, y=%d", x, y);
 *     CLog(WARN, "This is a warning message.");
 *     CLog(ERROR, "An error occurred: %s", "File not found");
 *     return 0;
 * }
 * \endcode
 */

#ifndef CSTD_LOG_H
#define CSTD_LOG_H

// Logging levels
#define DEBUG 0 ///< Debug level for verbose logging.
#define INFO 1  ///< Info level for general information.
#define WARN 2  ///< Warning level for cautionary messages.
#define ERROR 3 ///< Error level for critical issues.

#include <stdint.h>

/// \brief Logs a message with a specific severity level.
/// \param level The severity level of the log message. It can be one of the
/// following:
///              - DEBUG
///              - INFO
///              - WARN
///              - ERROR
/// \param loc The location of the file.
/// \param line The line number.
/// \param format The format string for the log message (similar to printf).
///               It can include additional arguments to format the message.
///
/// This function prints the log message to the standard output or standard
/// error based on the log level. It also includes a timestamp and uses colors
/// if enabled.
void CLogImpl(int level, char* loc, __uint64_t line, const char *format, ...);

#define CLog(level, format, ...) \
    CLogImpl(level, __FILE__, __LINE__,format, ##__VA_ARGS__)


/// \brief Enables ANSI-colored output for log messages.
///
/// When enabled, log messages will be printed in color according to their
/// severity level.
void enable_colors(void);

/// \brief Disables ANSI-colored output for log messages.
///
/// When disabled, log messages will be printed in the default terminal color.
void disable_colors(void);

/// \brief Enables debug outputs. This is turned off by default.
/// \see disable_debugging
void enable_debugging(void);

/// \brief Disables debug outputs. This is likely what you want to use if you
/// want to suppress debug outputs. Revert these by enabling debug outputs.
/// \see enable_debugging
void disable_debugging(void);

/// \brief Enables file and line number logging for log messages.
/// \see disable_location
void enable_location(void);

/// \brief Disables file and line number logging for log messages.
/// \see enable_location
void disable_location(void);

#endif // CSTD_LOG_H