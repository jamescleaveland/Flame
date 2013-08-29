#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cassert>
#include "png.h"

// TODO: Move this to CLANG
#define DEBUG 1

struct ColorRGB
{
  ColorRGB(char _r, char _g, char _b) : r(_r), g(_g), b(_b) {}
  ColorRGB(char x) : r(x), g(x), b(x) {}
  char r, g, b;
};

struct ColorRGBA
{
  ColorRGBA(char _r, char _g, char _b, char _a) : r(_r), g(_g), b(_b), a(_a) {}
  ColorRGBA(char x) : r(x), g(x), b(x), a(x) {}
  char r, g, b, a;
};

class Image
{
  public:
    enum ColorType
    {
      RGB,
      RGBA
    };

    Image(int width, int height, ColorType colorType = ColorType::RGB);
    ~Image();

    inline void ValidatePut(int x, int y, ColorRGB& c)
    {
#if DEBUG
      assert(x >= 0 && x < m_width);
      assert(y >= 0 && y < m_height);
      assert(m_bytesPerPixel == 3);
#endif
    }

    inline void ValidatePut(int x, int y, ColorRGBA& c)
    {
#if DEBUG
      assert(x >= 0 && x < m_width);
      assert(y >= 0 && y < m_height);
      assert(m_bytesPerPixel == 4);
#endif
    }

    inline void PutPixel(int x, int y, ColorRGB& c)
    {
      ValidatePut(x, y, c);
      png_byte* pixelBytes = &m_rowPointers[y][x * m_bytesPerPixel];
      pixelBytes[0] = c.r;
      pixelBytes[1] = c.g;
      pixelBytes[2] = c.b;
    }

    inline void PutPixel(int x, int y, ColorRGBA& c)
    {
      ValidatePut(x, y, c);
      png_byte* pixelBytes = &m_rowPointers[y][x * m_bytesPerPixel];
      pixelBytes[0] = c.r;
      pixelBytes[1] = c.g;
      pixelBytes[2] = c.b;
      pixelBytes[3] = c.a;
    }

    inline int GetWidth() const { return m_width; }
    inline int GetHeight() const { return m_height; }

    bool Save(const char* path);

  private:
    const int m_width;
    const int m_height;
    png_byte m_colorType;
    int m_bytesPerPixel;
    png_bytep* m_rowPointers;
};

#endif
