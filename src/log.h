#ifndef LOG_H
#define LOG_H

void log_init(char const *filename);
void log_dispose();
void log_print(char const *fmt, ...);
void log_last_error();

#endif