#ifndef __FILTER_H__
#define __FILTER_H__

#include "Image.h"
#include "Plot.h"

template<typename T>
class Filter
{
  public:
    Filter(Image<T>& image, const Plot& plot);
    ~Filter();

    void Horizontal(int kernelWidth);
    void Cross(int kernelSize);
    void Bilinear();
    void WeightedAverage();

  protected:
    int Lerp(int start, int end, float t) const;
    float Lerp(float start, float end, float t) const;
    int Bound(int idx, int limit) const;

  private:
    Image<T>& m_image;
    Image<T>* m_clone;
    const Plot& m_plot;
};

//-----------------------------------------------------------
template<typename T>
Filter<T>::Filter(Image<T>& image, const Plot& plot)
  : m_image(image)
  , m_plot(plot)
{
  m_clone = m_image.Clone();
}

//-----------------------------------------------------------
template<typename T>
Filter<T>::~Filter()
{
  delete m_clone;
  m_clone = nullptr;
}

//-----------------------------------------------------------
template<typename T>
void Filter<T>::Horizontal(int kernelWidth)
{
  const int width = m_image.GetWidth();
  const int height = m_image.GetHeight();

  for (auto y = 0; y < height; ++y)
  {
    T* pixelRow = m_clone->GetPixelRow(y);
    const Plot::Data* plotRow = m_plot.GetRow(y);

    for (auto x = 0; x < width; ++x)
    {
      const float plotWeight = plotRow[x].weight;

      // Let the kernel width be a function of plot weight
      const int k = Lerp(kernelWidth, 1, plotWeight);
      assert(k != 0);

      const float sampleWeight = 1.0f / k;
      T filteredPixel;

      for (auto i = 0; i < k; ++i)
      {
        const int offset = i - (k / 2);
        int cx = x + offset;
        cx = cx < 0 ? 0 : (cx >= width ? width - 1 : cx);
        filteredPixel += pixelRow[cx] * sampleWeight;
      }

      filteredPixel = filteredPixel * (1.0f - plotWeight) + pixelRow[x] * plotWeight;
      m_image.PutPixel(x, y, filteredPixel);
    }
  }
}

//-----------------------------------------------------------
template<typename T>
void Filter<T>::Cross(int kernelSize)
{
  const int width = m_image.GetWidth();
  const int height = m_image.GetHeight();

  for (auto y = 0; y < height; ++y)
  {
    for (auto x = 0; x < width; ++x)
    {
      const float plotWeight = m_plot.GetAt(x, y)->weight;

      // Let the kernel size be a function of plot weight
      const int k = Lerp(kernelSize, 1, plotWeight);
      assert(k != 0);

      // Sample weight is divided in half to compensate for cross
      const float sampleWeight = 1.0f / k * 0.5f;
      T filteredPixel;

      // Horizontal pass of cross
      for (auto i = 0; i < k; ++i)
      {
        const int offset = i - (k / 2);
        int cx = x + offset;
        cx = cx < 0 ? 0 : (cx >= width ? width - 1 : cx);
        filteredPixel += *m_clone->GetPixelAt(cx, y) * sampleWeight;
      }

      // Vertical pass of cross
      for (auto i = 0; i < k; ++i)
      {
        const int offset = i - (k / 2);
        int cy = y + offset;
        cy = cy < 0 ? 0 : (cy >= height ? height - 1 : cy);
        filteredPixel += *m_clone->GetPixelAt(x, cy) * sampleWeight;
      }

      filteredPixel = filteredPixel * (1.0f - plotWeight) + *m_clone->GetPixelAt(x, y) * plotWeight;
      m_image.PutPixel(x, y, filteredPixel);
    }
  }
}

//-----------------------------------------------------------
template<typename T>
void Filter<T>::Bilinear()
{
  const int width = m_image.GetWidth();
  const int height = m_image.GetHeight();

  for (auto y = 0; y < height; ++y)
  {
    for (auto x = 0; x < width; ++x)
    {
      const float weight = m_plot.GetAt(x, y)->weight;
      const float invWeight = 1.0f - weight;
      const int l_x = Bound(x - 1, width);
      const int r_x = Bound(x + 1, width);
      const int u_y = Bound(y + 1, height);
      const int d_y = Bound(y - 1, height);
      T* left = m_clone->GetPixelAt(l_x, y);
      T* right = m_clone->GetPixelAt(r_x, y);
      T* up = m_clone->GetPixelAt(x, u_y);
      T* down = m_clone->GetPixelAt(x, d_y);
      T* center = m_clone->GetPixelAt(x, y);
      const float cw = Lerp(0.2f, 0.8f, weight);
      const float remainder = 1.0f - cw;
      const float qw = remainder * 0.25f;
      m_image.PutPixel(x, y, *center*cw + *left*qw + *right*qw + *up*qw + *down*qw);
    }
  }
}

//-----------------------------------------------------------
template<typename T>
void Filter<T>::WeightedAverage()
{
  const int width = m_image.GetWidth();
  const int height = m_image.GetHeight();

  for (auto y = 0; y < height; ++y)
  {
    for (auto x = 0; x < width; ++x)
    {
      const float weight = m_plot.GetAt(x, y)->weight;
      const float invWeight = 1.0f - weight;
      const int l_x = Bound(x - 1, width);
      const int r_x = Bound(x + 1, width);
      const int u_y = Bound(y + 1, height);
      const int d_y = Bound(y - 1, height);

      T* left = m_clone->GetPixelAt(l_x, y);
      T* right = m_clone->GetPixelAt(r_x, y);
      T* up = m_clone->GetPixelAt(x, u_y);
      T* down = m_clone->GetPixelAt(x, d_y);
      T* center = m_clone->GetPixelAt(x, y);

      float lw = m_plot.GetAt(x, y)->weight;
      float rw = m_plot.GetAt(x, y)->weight;
      float uw = m_plot.GetAt(x, y)->weight;
      float dw = m_plot.GetAt(x, y)->weight;
      float cw = m_plot.GetAt(x, y)->weight;

      float sum = std::numeric_limits<float>::epsilon();
      sum += lw;
      sum += rw;
      sum += uw;
      sum += dw;
      sum += cw;
      sum = 1.0f / sum;
      lw *= sum;
      rw *= sum;
      uw *= sum;
      dw *= sum;
      cw *= sum;

      m_image.PutPixel(x, y, *center*cw + *left*lw + *right*rw + *up*uw + *down*dw);
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

