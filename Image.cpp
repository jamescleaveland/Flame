#include "Image.h"
#include "Util.h"
#include "png.h"

Image::Image
  ( int width
  , int height
  , Image::ColorType colorType /* Image::ColorType::RGB */
  )
  : m_width(width)
  , m_height(height)
{
  switch (colorType)
  {
    case Image::ColorType::RGB:
      m_colorType = PNG_COLOR_TYPE_RGB;
      m_bytesPerPixel = 3;
      break;

    case Image::ColorType::RGBA:
      m_colorType = PNG_COLOR_TYPE_RGBA;
      m_bytesPerPixel = 4;
      break;

    default:
      assert(0);
  }

  m_rowPointers = new png_bytep[m_height];
  for (auto i = 0; i < m_height; ++i)
  {
    m_rowPointers[i] = new png_byte[m_width * m_bytesPerPixel];
  }
}

//-----------------------------------------------------------
Image::~Image()
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
bool Image::Save(const char* path)
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
