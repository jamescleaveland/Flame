#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cassert>
#include "VectorTypes.h"
#include "Util.h"
#include "png.h"

template<typename T>
class Image
{
  public:
    Image(int width, int height);
    virtual ~Image();

    //TODO: Move these to pure templates, and not pure virtual
    virtual void PutPixel(int x, int y, const ColorRGBf& c) = 0;
    virtual void PutPixel(int x, int y, const ColorRGBAf& c) = 0;
    virtual Image<T>* Clone() const = 0;
    virtual bool Save(const char* path) = 0;

    T* GetPixelRow(int index);
    T* GetPixelAt(int x, int y);
    inline int GetWidth() const { return m_width; }
    inline int GetHeight() const { return m_height; }

  protected:
    const int m_width;
    const int m_height;
    T** m_rawPixels;
};

//-----------------------------------------------------------
template<typename T>
Image<T>::Image(int width, int height)
  : m_width(width)
  , m_height(height)
{
  m_rawPixels = new T*[m_height];
  for (auto i = 0; i < m_height; ++i)
  {
    m_rawPixels[i] = new T[m_width];
  }
}

//-----------------------------------------------------------
template<typename T>
Image<T>::~Image()
{
  for (auto i = 0; i < m_height; ++i)
  {
    delete[] m_rawPixels[i];
    m_rawPixels[i] = nullptr;
  }

  delete[] m_rawPixels;
  m_rawPixels = nullptr;
}

//-----------------------------------------------------------
template<typename T>
T* Image<T>::GetPixelRow(int index)
{
  assert(index >= 0 && index < m_height);
  return m_rawPixels[index];
}

//-----------------------------------------------------------
template<typename T>
T* Image<T>::GetPixelAt(int x, int y)
{
  assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
  return &m_rawPixels[y][x];
}

//-----------------------------------------------------------
template<typename T>
class Png : public Image<T>
{
  public:
    Png(int width, int height);
    virtual ~Png();

    void PutPixel(int x, int y, const ColorRGBf& c) override;
    void PutPixel(int x, int y, const ColorRGBAf& c) override;
    Image<T>* Clone() const override;
    bool Save(const char* path) override;

  protected:
    bool SaveImpl(const char* path, png_byte colorType, png_bytep* rowPointers);
};

//-----------------------------------------------------------
template<typename T>
Png<T>::Png(int width, int height)
  : Image<T>(width, height)
{}

//-----------------------------------------------------------
template<typename T>
Png<T>::~Png()
{}

//-----------------------------------------------------------
template<typename T>
bool Png<T>::SaveImpl(const char* path, png_byte colorType, png_bytep* rowPointers)
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
  png_set_IHDR(png_ptr, info_ptr, this->m_width, this->m_height, bitDepth, colorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    Log("Error writing bytes");
    return false;
  }

  png_write_image(png_ptr, rowPointers);

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    Log("Failed during end of write");
    return false;
  }

  png_write_end(png_ptr, NULL);

  return true;
}

#endif

