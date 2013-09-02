#ifndef __IMAGEGENCONFIGURATOR_H__
#define __IMAGEGENCONFIGURATOR_H__

#include "ImageGen.h"
#include "Transforms.h"

template<typename ImgT, typename ColorT>
class ImageGenConfigurator
{
  public:
    static void Configure(char* commands, double x, double y, Transforms::Ptr transforms, ImageGen<ImgT, ColorT>& imageGen);
};

//-----------------------------------------------------------
template<typename ImgT, typename ColorT>
/* static */ void ImageGenConfigurator<ImgT, ColorT>::Configure(char* commands, double x, double y, Transforms::Ptr transforms, ImageGen<ImgT, ColorT>& imageGen)
{
  const char* tokens = " ";
  char* split = strtok(commands, tokens);
  bool parsingNonLinear = false;
  while (split)
  {
    if (strcmp(split, "|") == 0)
    {
      parsingNonLinear = true;
    }

    const Transform* t = nullptr;;
    auto tIter = transforms->find(split);
    if (tIter != std::end(*transforms))
    {
      t = &tIter->second;
    }

    if (t)
    {
      if (parsingNonLinear)
      {
        imageGen.AddNonLinearTransform(t);
      }
      else
      {
        imageGen.AddAffineTransform(t);
      }
    }

    split = strtok(nullptr, tokens);
  }

  imageGen.SetPostTransform(x, y);
}

#endif

