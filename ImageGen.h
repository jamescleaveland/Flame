#ifndef __IMAGEGEN_H__
#define __IMAGEGEN_H__

#include <random>
#include "Transforms.h"
#include "Plot.h"

class Image;

class ImageGen
{
  public:
    ImageGen(Image& image, float quality);
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
    Image& m_image;
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

#endif
