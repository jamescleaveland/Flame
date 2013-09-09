#ifndef __FILTER_H__
#define __FILTER_H__

#include "Image.h"
#include "Plot.h"
#include <cmath>

template<typename T>
class Filter
{
  public:
    Filter(Image<T>& image, const Plot& plot, const ToneMap<T>& toneMap);

    void Box();

  protected:
    int Lerp(int start, int end, float t) const;
    float Lerp(float start, float end, float t) const;
    int Bound(int idx, int limit) const;

  private:
    Image<T>& m_image;
    const Plot& m_plot;
    const ToneMap<T>& m_toneMap;
};

//-----------------------------------------------------------
template<typename T>
Filter<T>::Filter(Image<T>& image, const Plot& plot, const ToneMap<T>& toneMap)
  : m_image(image)
  , m_plot(plot)
  , m_toneMap(toneMap)
{
  assert(m_image.GetWidth() * 2 == m_plot.GetWidth());
  assert(m_image.GetHeight() * 2 == m_plot.GetHeight());
}

//-----------------------------------------------------------
template<typename T>
void Filter<T>::Box()
{
  const int imgWidth = m_image.GetWidth();
  const int imgHeight = m_image.GetHeight();
  const int plotWidth = m_plot.GetWidth();
  const int plotHeight = m_plot.GetHeight();
  const float e = std::numeric_limits<float>::epsilon();

  for (auto y = 0; y < imgHeight; ++y)
  {
    for (auto x = 0; x < imgWidth; ++x)
    {
      const auto xx = x * 2;
      const auto yy = y * 2;

      const int l_x = Bound(xx - 1, plotWidth);
      const int r_x = Bound(xx + 1, plotWidth);
      const int u_y = Bound(yy + 1, plotHeight);
      const int d_y = Bound(yy - 1, plotHeight);

      const float center = m_plot.GetAt(xx, yy)->weight;
      const float left = m_plot.GetAt(l_x, yy)->weight;
      const float right = m_plot.GetAt(r_x, yy)->weight;
      const float up = m_plot.GetAt(xx, u_y)->weight;
      const float down = m_plot.GetAt(xx, d_y)->weight;
      const float lu = m_plot.GetAt(l_x, u_y)->weight;
      const float ru = m_plot.GetAt(r_x, u_y)->weight;
      const float ld = m_plot.GetAt(l_x, d_y)->weight;
      const float rd = m_plot.GetAt(r_x, d_y)->weight;

      const float coef = 0.111111f;
      float final = (center + left + right + up + down + lu + ru + ld + rd + e) * coef;
      final = std::max(std::min(final, 1.0f), 0.0f);

      m_image.PutPixel(x, y, m_toneMap.GetTone(final));
    }
  }
}

//-----------------------------------------------------------
template<typename T>
int Filter<T>::Lerp(int start, int end, float t) const
{
  return static_cast<int>((end - start) * t) + start;
}

//-----------------------------------------------------------
template<typename T>
float Filter<T>::Lerp(float start, float end, float t) const
{
  return (end - start) * t + start;
}

//-----------------------------------------------------------
template<typename T>
int Filter<T>::Bound(int idx, int limit) const
{
  return idx < 0 ? 0 : (idx >= limit ? limit - 1 : idx);
}

#endif

