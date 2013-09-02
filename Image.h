#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cassert>
#include "VectorTypes.h"
#include "Util.h"
#include "png.h"

template<typename T>
class Png
{
  public:
    Png(int width, int height);
    ~Png();

    void PutPixel(int x, int y, const T& c);

    inline int GetWidth() const { return m_width; }
    inline int GetHeight() const { return m_height; }

    bool Save(const char* path);

  private:
    const int m_width;
    const int m_height;
    const int m_bytesPerPixel;
    png_byte m_colorType;
    png_bytep* m_rowPointers;
};

//-----------------------------------------------------------
template<typename T>
bool Png<T>::Save(const char* path)
{
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
  {
    Log("Error creating png write struct");
    return false;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    Log("Error creating png info struct");
    return false;
  }

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    Log("Error during png init_io");
    return false;
  }

  FILE* fp = fopen(path, "wb");
  png_init_io(png_ptr, fp);

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    Log("Error writing header");
    return false;
  }

  const int bitDepth = 8;
  png_set_IHDR(png_ptr, info_ptr, m_width, m_height, bitDepth, m_colorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    Log("Error writing bytes");
    return false;
  }

  png_write_image(png_ptr, m_rowPointers);

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    Log("Failed during end of write");
    return false;
  }

  png_write_end(png_ptr, NULL);

  return true;
}

#endif

