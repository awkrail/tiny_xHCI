#pragma once

#include "frame_buffer_config.h"

struct PixelColor 
{
  uint8_t r, g, b;
};

struct IntVector2D
{
  int x, y;
};

void WriteRGBPixel(const struct FrameBufferConfig *config,
                   int x, int y, const struct PixelColor *c);

void WriteBGRPixel(const struct FrameBufferConfig *config,
                   int x, int y, const struct PixelColor *c);

void WritePixel(const struct FrameBufferConfig *config,
                int x, int y, const struct PixelColor *c);

void DrawRectangle(const struct FrameBufferConfig *config,
                   const struct IntVector2D *pos,
                   const struct IntVector2D *size,
                   const struct PixelColor *color);

void FillRectangle(const struct FrameBufferConfig *config,
                   const struct IntVector2D *pos,
                   const struct IntVector2D *size,
                   const struct PixelColor *color);
