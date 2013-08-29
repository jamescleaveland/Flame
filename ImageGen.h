#ifndef __IMAGEGEN_H__
#define __IMAGEGEN_H__

class Image;

class ImageGen
{
  public:
    ImageGen(Image& image);

    void VerticalBars() const;
    void RandomNoise() const;
    void SierpinskisGasket() const;

  private:
    Image& m_image;
    const int m_width;
    const int m_height;
};

#endif
