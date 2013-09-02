#include "Image.h"
#include "Util.h"
#include "png.h"

//-----------------------------------------------------------
template<>
Png<ColorRGBf>::Png(int width, int height)
  : m_width(width)
  , m_height(height)
  , m_bytesPerPixel(3)
{
  m_colorType = PNG_COLOR_TYPE_RGB;

  m_rowPointers = new png_bytep[m_height];
  for (auto i = 0; i < m_height; ++i)
  {
    m_rowPointers[i] = new png_byte[m_width * m_bytesPerPixel];
  }
}

//-----------------------------------------------------------
template<>
Png<ColorRGBf>::~Png()
{
  for (auto i = 0; i < m_height; ++i)
  {
    delete[] m_rowPointers[i];
    m_rowPointers[i] = nullptr;
  }

  delete[] m_rowPointers;
  m_rowPointers = nullptr;
}

//-----------------------------------------------------------
template<>
void Png<ColorRGBf>::PutPixel(int x, int y, const ColorRGBf& c)
{
  if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    return;

  // TODO: Could be *much* faster
  png_byte* pixelBytes = &m_rowPointers[y][x * m_bytesPerPixel];
  pixelBytes[0] = static_cast<unsigned char>(c.r * 255);
  pixelBytes[1] = static_cast<unsigned char>(c.g * 255);
  pixelBytes[2] = static_cast<unsigned char>(c.b * 255);
}

