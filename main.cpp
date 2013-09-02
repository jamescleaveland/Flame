#include "Util.h"
#include "ImageGen.h"
#include "ImageGenConfigurator.h"
#include "Image.h"

int main(int argc, char* argv[])
{
  if (argc != 7)
  {
    Error("Incorrect parameters. Usage: Flame <width> <height> <quality (1.0)> <config>");
  }

  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  float quality = atof(argv[3]);
  char* commands = argv[4];
  double postX = atof(argv[5]);
  double postY = atof(argv[6]);

  Png<ColorRGBf> png(width, height);
  ImageGen<Png<ColorRGBf>, ColorRGBf> gen(png, quality);
  Transforms::Ptr transforms = Transforms::Get();
  ImageGenConfigurator<Png<ColorRGBf>, ColorRGBf>::Configure(commands, postX, postY, transforms, gen);
  
  Log("Performing %d iterations", gen.GetQuality());
  gen.Synthesize();
  gen.ColorizePlot();

  png.Save("test.png");
  Log("Done");
}
