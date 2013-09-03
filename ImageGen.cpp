#include "ImageGen.h"

ImageGen::ImageGen(int width, int height, float quality)
  : m_width(width)
  , m_height(height)
  , m_quality(m_width * m_height * quality)
  , m_postX(0.0)
  , m_postY(0.0)
  , m_plot(m_width, m_height)
{}

//-----------------------------------------------------------
void ImageGen::Synthesize()
{
  std::default_random_engine generator;
  std::uniform_real_distribution<double> doubleDist;

  double x = doubleDist(generator);
  double y = doubleDist(generator);

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

  m_plot.ComputeWeights();
}

