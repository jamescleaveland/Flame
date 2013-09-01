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

