#include "ImageGen.h"
#include "Image.h"
#include <random>

ImageGen::ImageGen(Image& image)
  : m_image(image)
  , m_width(m_image.GetWidth())
  , m_height(m_image.GetHeight())
{}

//-----------------------------------------------------------
void ImageGen::VerticalBars() const
{
  ColorRGB white(255);
  ColorRGB black(0);

  for (auto y = 0; y < m_height; ++y)
  {
    for (auto x = 0; x < m_width; ++x)
    {
      m_image.PutPixel(x, y, x % 10 == 0 ? white : black);
    }
  }
}

//-----------------------------------------------------------
void ImageGen::RandomNoise() const
{
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 255);

  for (auto y = 0; y < m_height; ++y)
  {
    for (auto x = 0; x < m_width; ++x)
    {
      ColorRGB randomColor(distribution(generator));
      m_image.PutPixel(x, y, randomColor);
    }
  }
}

//-----------------------------------------------------------
void ImageGen::SierpinskisGasket() const
{
  ColorRGB white(255);
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dist(0, 2);

  std::uniform_int_distribution<int> plane(0, m_width);
  float x = static_cast<float>(plane(generator));
  float y = static_cast<float>(plane(generator));

  for (auto j = 0; j < 10000; ++j)
  {
    int i = dist(generator);

    if (i == 0)
    {
      x = x / 2.0f;
      y = y / 2.0f;
    }
    else if (i == 1)
    {
      x = (x + m_width) / 2.0f;
      y = y / 2.0f;
    }
    else if (i == 2)
    {
      x = x / 2.0f;
      y = (y + m_width) / 2.0f;
    }

    m_image.PutPixel(static_cast<int>(x), static_cast<int>(y), white);
  }
}
