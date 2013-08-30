#include "Util.h"
#include "ImageGen.h"
#include "Image.h"

int main(int argc, char* argv[])
{
  float quality = 1.0f;
  if (argc == 2)
  {
    quality = atof(argv[1]);
  }

  Image image(512, 512);

  ImageGen gen(image, quality);
  Log("Performing %d iterations", gen.GetQuality());
  gen.Transform();
  gen.ColorizePlot();

  image.Save("test.png");
  std::cout << "Done" << std::endl;
}
