#ifndef __IMAGEGENCONFIGURATOR_H__
#define __IMAGEGENCONFIGURATOR_H__

#include "Transforms.h"

class ImageGen;

class ImageGenConfigurator
{
  public:
    static void Configure(char* commands, double x, double y, Transforms::Ptr transforms, ImageGen& imageGen);
};

#endif
