#include "Util.h"
#include "ImageGen.h"
#include "ImageGenConfigurator.h"
#include "Image.h"

int main(int argc, char* argv[])
{
  if (argc != 5)
  {
    Error("Incorrect parameters. Usage: Flame <quality (1.0)> <config>");
  }

  float quality = atof(argv[1]);
  char* commands = argv[2];
  double postX = atof(argv[3]);
  double postY = atof(argv[4]);

  Image image(512, 512);
  ImageGen gen(image, quality);
  Transforms::Ptr transforms = Transforms::Get();
  ImageGenConfigurator::Configure(commands, postX, postY, transforms, gen);
  
  Log("Performing %d iterations", gen.GetQuality());
  gen.Synthesize();
  gen.ColorizePlot();

  image.Save("test.png");
  Log("Done");
}
