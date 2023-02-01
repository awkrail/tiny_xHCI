#pragma once

#include <stddef.h>
#include <stdint.h>
#include "graphics.h"

void WriteAscii(const struct FrameBufferConfig *config,
                int x, int y, char c,
                const struct PixelColor *color);
void WriteString(const struct FrameBufferConfig *config,
                int x, int y, const char *s,
                const struct PixelColor *color);
