#pragma once

#include "serial.h"
#include "config.h"

#define ERROR(x) _error(__FILE__, __LINE__, x)
#define WARN(x) _warn(__FILE__, __LINE__, x)
#define LOG(x) _log(__FILE__, __LINE__, x)

void _error(const char* file, unsigned int line, const char* message);
void _warn(const char* file, unsigned int line, const char* message);
void _log(const char* file, unsigned int line, const char* message);
