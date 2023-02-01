#pragma once

#include "graphics.h"

#define kRows 25
#define kColumns 80

struct Console {
  const struct FrameBufferConfig *config;
  const struct PixelColor *fg_color;
  const struct PixelColor *bg_color;
  int cursor_row;
  int cursor_column;
  char buffer[kRows][kColumns+1];
};

void InitConsole(struct Console *console,
                 const struct FrameBufferConfig *config,
                 const struct PixelColor *fg_color,
                 const struct PixelColor *bg_color);
void PutString(struct Console *console, const char *s);
void NewLine(struct Console *console);
