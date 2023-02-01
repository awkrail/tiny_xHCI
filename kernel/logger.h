#pragma once

#include "console.h"

enum LogLevel {
  kError = 3,
  kWarn = 4,
  kInfo = 6,
  kDebug = 7,
};

void SetLogLevel(enum LogLevel level);
int Log(enum LogLevel level, struct Console *console,
        const char* format, ...);
