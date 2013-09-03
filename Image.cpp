#include "Image.h"
#include "Util.h"
#include "png.h"

//-----------------------------------------------------------
template<>
void Png<ColorRGBf>::PutPixel(int x, int y, const ColorRGBf& c)
{
  if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    return;

  m_rawPixels[y][x] = c;
}

//-----------------------------------------------------------
template<>
void Png<ColorRGBf>::PutPixel(int x, int y, const ColorRGBAf& c)
{
  if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    return;

  m_rawPixels[y][x] = ColorRGBf(c.r, c.g, c.b);
}

//-----------------------------------------------------------
template<>
Image<ColorRGBf>* Png<ColorRGBf>::Clone() const
{
  Png<ColorRGBf>* clone = new Png<ColorRGBf>(m_width, m_height);

  for (auto y = 0; y < m_height; ++y)
  {
    for (auto x = 0; x < m_width; ++x)
    {
      clone->m_rawPixels[y][x] = m_rawPixels[y][x];
    }
  }

  return clone;
}

//-----------------------------------------------------------
template<>
bool Png<ColorRGBf>::Save(const char* path)
{
  // New up some png buffers to quantize into
  png_bytep* rowPointers = new png_bytep[m_height];
  const int bytesPerPixel = 3;
  for (auto i = 0; i < m_height; ++i)
  {
    rowPointers[i] = new png_byte[m_width * bytesPerPixel];
  }

  // Quantize
  for (auto y = 0; y < m_height; ++y)
  {
    for (auto x = 0; x < m_width; ++x)
    {
      png_byte* pixelBytes = &rowPointers[y][x * bytesPerPixel];
      const ColorRGBf& c = m_rawPixels[y][x];
      pixelBytes[0] = static_cast<unsigned char>(c.r * 255);
      pixelBytes[1] = static_cast<unsigned char>(c.g * 255);
      pixelBytes[2] = static_cast<unsigned char>(c.b * 255);
    }
  }

  bool success = SaveImpl(path, PNG_COLOR_TYPE_RGB, rowPointers);

  // Free
  for (auto i = 0; i < m_height; ++i)
  {
    delete[] rowPointers[i];
    rowPointers[i] = nullptr;
  }
  delete[] rowPointers;
  rowPointers = nullptr;

  return success;
}

