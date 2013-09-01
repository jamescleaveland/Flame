#include "ImageGen.h"
#include "Image.h"
#include "Util.h"
#include <random>
#include <cmath>

ImageGen::ImageGen(Image& image, float quality)
  : m_image(image)
  , m_width(m_image.GetWidth())
  , m_height(m_image.GetHeight())
  , m_quality(m_width * m_height * quality)
  , m_postX(0.0)
  , m_postY(0.0)
  , m_plot(m_width, m_height)
  , m_gen()
  , m_biunitDouble(-1.0, 1.0)
  , m_biunitFloat(-1.0f, 1.0f)
{}

//-----------------------------------------------------------
void ImageGen::ColorizePlot()
{
  for (auto y = 0; y < m_height; ++y)
  {
    const Plot::Data* row = m_plot.GetRow(y);
    for (auto x = 0; x < m_width; ++x)
    {
      if (row[x] != 0)
      {
        m_image.PutPixel(x, y, ColorRGB::White());
      }
    }
  }
}

//-----------------------------------------------------------
void ImageGen::SierpinskisGasket(int count) const
{
  ColorRGB white(255);
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dist(0, 2);

  std::uniform_int_distribution<int> plane(0, m_width);
  float x = static_cast<float>(plane(generator));
  float y = static_cast<float>(plane(generator));

  for (auto j = 0; j < count; ++j)
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

//-----------------------------------------------------------
void ImageGen::Sine(double& outX, double& outY) const
{
  outX = sin(outX);
  outY = sin(outY);
}

//-----------------------------------------------------------
void ImageGen::Spherical(double& outX, double& outY) const
{
  double r = sqrt(outX * outX + outY * outY);
  double oneOverR2 = 1.0 / (r * r);
  outX = oneOverR2 * outX;
  outY = oneOverR2 * outY;
}

//-----------------------------------------------------------
void ImageGen::Synthesize()
{
  double x = BiunitDouble();
  double y = BiunitDouble();

  std::default_random_engine generator;
  std::uniform_int_distribution<int> dist(0, m_affineTransforms.size() - 1);

  for (auto j = 0; j < m_quality; ++j)
  {
    // Affine transform chosen chaotically
    int i = dist(generator);
    (*m_affineTransforms[i])(x, y);

    // Non-linear transforms executed afterwards
    for (auto t : m_nonLinearTransforms)
    {
      (*t)(x, y);
    }

    m_plot.Record(x + m_postX, y + m_postY);
  }
}

