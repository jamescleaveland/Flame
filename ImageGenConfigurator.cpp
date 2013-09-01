#include "ImageGenConfigurator.h"
#include "ImageGen.h"

/* static */ void ImageGenConfigurator::Configure(char* commands, double x, double y, Transforms::Ptr transforms, ImageGen& imageGen)
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
