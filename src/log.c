#include <stdio.h>
#include <stdarg.h>

#include "log.h"

static FILE *_file;

void log_init(char const *filename) {
	if (!_file) {
		_file = fopen(filename, "a+");
		log_print("===== Application started =====");
	}
}

void log_dispose() {
	if (_file) {
		log_print("===== Application closed =====");
		fclose(_file);
	}
}

void log_print(char const *fmt, ...) {
	if(_file) {		
		va_list argp;		
		va_start(argp, fmt);		
		vfprintf(_file, fmt, argp);
		va_end(argp);
		fprintf(_file, "\n");
		fflush(_file);
	}
}

void log_last_error() {
	//TODO: impl
}