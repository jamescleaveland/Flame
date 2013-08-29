#include "Util.h"
#include "ImageGen.h"
#include "Image.h"

int main()
{
  Image image(512, 512);

  ImageGen gen(image);
  gen.SierpinskisGasket();

  image.Save("test.png");
  std::cout << "Done" << std::endl;
}
