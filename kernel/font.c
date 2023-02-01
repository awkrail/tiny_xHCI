#include "font.h"

extern const uint8_t _binary_hankaku_bin_start;
extern const uint8_t _binary_hankaku_bin_end;
extern const uint8_t _binary_hankaku_bin_size;

const uint8_t *GetFont(char c)
{
  unsigned int index = 16 * (unsigned int)c;
  uintptr_t hankaku_size = (uintptr_t)(&_binary_hankaku_bin_size);
  if(index >= hankaku_size)
    return NULL;

  return &_binary_hankaku_bin_start + index;
}

void WriteAscii(const struct FrameBufferConfig *config,
                int x, int y, char c, 
                const struct PixelColor *color) 
{
  const uint8_t *font = GetFont(c);
  if(font == NULL)
    return;

  for(int dy=0; dy < 16; ++dy) {
    for(int dx=0; dx < 8; ++dx) {
      if((font[dy] << dx) & 0x80u) {
        WritePixel(config, x+dx, y+dy, color);
      }
    }
  }
}

void WriteString(const struct FrameBufferConfig *config,
                 int x, int y, const char *s,
                 const struct PixelColor *color)
{
  for(int i=0; s[i] != '\0'; ++i) {
    WriteAscii(config, x + 8 * i, y, s[i], color);
  }
}
