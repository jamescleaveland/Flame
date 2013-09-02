#ifndef __IMAGEGEN_H__
#define __IMAGEGEN_H__

#include <random>
#include "Image.h"
#include "Transforms.h"
#include "Plot.h"
#include "ToneMap.h"

template<typename ImgT, typename ColorT>
class ImageGen
{
  public:
    ImageGen(ImgT& image, float quality);
    inline int GetQuality() const { return m_quality; }

    void ColorizePlot();

    void AddAffineTransform(const Transform* t) { m_affineTransforms.push_back(t); }
    void AddNonLinearTransform(const Transform* t) { m_nonLinearTransforms.push_back(t); }
    inline void SetPostTransform(const double x, const double y) {m_postX=x; m_postY=y;}
    void Synthesize();

  private:
    inline double BiunitDouble() { return m_biunitDouble(m_gen); }
    inline float BiunitFloat() { return m_biunitFloat(m_gen); }

    // Image members
    ImgT& m_image;
    const int m_width;
    const int m_height;
    const int m_quality;

    // Transformations to be applied during image generation
    std::vector<const Transform*> m_affineTransforms;
    std::vector<const Transform*> m_nonLinearTransforms;
    double m_postX;
    double m_postY;

    Plot m_plot;

    // Generator & distribution for biunit random floats
    std::default_random_engine m_gen;
    std::uniform_real_distribution<double> m_biunitDouble;
    std::uniform_real_distribution<float> m_biunitFloat;
};

//-----------------------------------------------------------
template<typename ImgT, typename ColorT>
ImageGen<ImgT, ColorT>::ImageGen(ImgT& image, float quality)
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
template<typename ImgT, typename ColorT>
void ImageGen<ImgT, ColorT>::ColorizePlot()
{
  ToneMap<ColorT> tm;

  // Fire
  /*tm.AddTone(0.0f, ColorT(0.0f));
  tm.AddTone(0.075f, ColorT(0.1f, 0.0f, 0.2f));
  tm.AddTone(0.15f, ColorT(0.65f, 0.2f, 0.25f));
  tm.AddTone(0.4f, ColorT(0.8f, 0.5f, 0.0f));
  tm.AddTone(1.0f, ColorT(1.0f, 1.0f, 0.5f));*/
  
  // Jayleigh's Palette
  tm.AddTone(0.0f, ColorT(0.0f));
  tm.AddTone(0.05f, ColorT(0.0f, 0.1f, 0.2f));
  tm.AddTone(0.3f, ColorT(0.0f, 0.35f, 0.6f));
  tm.AddTone(0.5f, ColorT(0.2f, 0.7f, 0.0f));
  tm.AddTone(1.0f, ColorT(1.0f, 1.0f, 0.0f));

  const int maxPlots = m_plot.GetMax();

  for (auto y = 0; y < m_height; ++y)
  {
    const Plot::Data* row = m_plot.GetRow(y);
    for (auto x = 0; x < m_width; ++x)
    {
      if (row[x] != 0)
      {
        float val = static_cast<float>(row[x]) / maxPlots;
        val = 1.0f - val;
        val = 1.0f - pow(val, 200);
        val = val < 0.0f ? 0.0f : (val > 1.0f ? 1.0f : val);
        m_image.PutPixel(x, y, tm.GetTone(val));
      }
    }
  }
}

//-----------------------------------------------------------
template<typename ImgT, typename ColorT>
void ImageGen<ImgT, ColorT>::Synthesize()
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

#endif

