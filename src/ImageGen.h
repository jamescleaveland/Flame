#ifndef __IMAGEGEN_H__
#define __IMAGEGEN_H__

#include <random>
#include "Transforms.h"
#include "Plot.h"

class ImageGen
{
  public:
    ImageGen(int width, int height, float quality);
    inline int GetQuality() const { return m_quality; }
    inline const Plot& GetPlot() const { return m_plot; }

    void AddAffineTransform(const Transform* t) { m_affineTransforms.push_back(t); }
    void AddNonLinearTransform(const Transform* t) { m_nonLinearTransforms.push_back(t); }
    inline void SetPostTransform(const double x, const double y) {m_postX=x; m_postY=y;}

    void SynthesizeAsync(const int numThreads);

  private:
    const int m_width;
    const int m_height;
    const int m_quality;

    // Transformations to be applied during image generation
    std::vector<const Transform*> m_affineTransforms;
    std::vector<const Transform*> m_nonLinearTransforms;
    double m_postX;
    double m_postY;

    Plot m_plot;
};

#endif

