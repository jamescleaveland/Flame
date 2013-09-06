#ifndef __IMAGEWRITER_H__
#define __IMAGEWRITER_H__

#include "Image.h"
#include "Plot.h"
#include "ToneMap.h"
#include "Filter.h"
#include <math.h>

template<typename T>
class ImageWriter
{
  public:
    ImageWriter(Image<T>& image, const Plot& plot, ToneMap<T>& toneMap);
    void Write();

  private:
    Image<T>& m_image;
    const Plot& m_plot;
    ToneMap<T>& m_toneMap;
};

//-----------------------------------------------------------
template<typename T>
ImageWriter<T>::ImageWriter(Image<T>& image, const Plot& plot, ToneMap<T>& toneMap)
  : m_image(image)
  , m_plot(plot)
  , m_toneMap(toneMap)
{}

//-----------------------------------------------------------
template<typename T>
void ImageWriter<T>::Write()
{
  const int width = m_image.GetWidth();
  const int height = m_image.GetHeight();

  // Convert plot to tone-mapped image
  for (auto y = 0; y < height; ++y)
  {
    const Plot::Data* row = m_plot.GetRow(y);
    for (auto x = 0; x < width; ++x)
    {
      m_image.PutPixel(x, y, m_toneMap.GetTone(row[x].weight));
    }
  }

  // Filter result
  Filter<T> filter(m_image, m_plot);
  //filter.WeightedAverage();
  filter.Bilinear();
}

#endif

