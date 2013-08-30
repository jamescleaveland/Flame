#ifndef __IMAGEGEN_H__
#define __IMAGEGEN_H__

#include <random>
#include "Plot.h"

class Image;

class ImageGen
{
  public:
    ImageGen(Image& image, float quality);
    inline int GetQuality() const { return m_quality; }

    void ColorizePlot();

    void VerticalBars(int count) const;
    void RandomNoise(int count) const;
    void SierpinskisGasket(int count) const;
    void Sine(int count) const;
    void Sine(double& outX, double& outY) const;
    void Spherical(double& outX, double& outY) const;
    void Transform();

  private:
    inline double BiunitDouble() { return m_biunitDouble(m_gen); }
    inline float BiunitFloat() { return m_biunitFloat(m_gen); }

    Image& m_image;
    const int m_width;
    const int m_height;
    const int m_quality;
    Plot m_plot;

    // Generator & distribution for biunit random floats
    std::default_random_engine m_gen;
    std::uniform_real_distribution<double> m_biunitDouble;
    std::uniform_real_distribution<float> m_biunitFloat;
};

#endif
