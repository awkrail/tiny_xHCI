#include "string.h"
#include "console.h"
#include "font.h"

void InitConsole(struct Console *console, 
                 const struct FrameBufferConfig *config,
                 const struct PixelColor *fg_color,
                 const struct PixelColor *bg_color)
{
  console->config = config;
  console->fg_color = fg_color;
  console->bg_color = bg_color;
  console->cursor_row = 0;
  console->cursor_column = 0;
  for(int i=0; i<kRows; ++i) {
    for(int j=0; j<kColumns; ++j) {
      console->buffer[i][j] = 0;
    }
  }
}

void PutString(struct Console *console,
               const char *s)
{
  while(*s) {
    if(*s == '\n') {
      NewLine(console);
    } else if (console->cursor_column < kColumns - 1) {
      WriteAscii(console->config, 8 * console->cursor_column, 
                 16 * console->cursor_row, *s, console->fg_color);
      console->buffer[console->cursor_row][console->cursor_column] = *s;
      ++console->cursor_column;
    }
    ++s;
  }  
}

void NewLine(struct Console *console)
{
  console->cursor_column = 0;
  if(console->cursor_row < kRows - 1) {
    ++console->cursor_row;
  } else {
    for(int y=0; y<16*kRows; ++y) {
      for(int x=0; x<8*kColumns; ++x) {
        WritePixel(console->config, x, y, console->bg_color);
      }
    }
    for(int row=0; row<kRows-1; ++row) {
      memcpy(console->buffer[row], console->buffer[row+1], kColumns+1);
      WriteString(console->config, 0, 16*row, 
                  console->buffer[row], console->fg_color);
    }
    memset(console->buffer[kRows-1], 0, kColumns+1); 
  }
}
