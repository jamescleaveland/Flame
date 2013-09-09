#include "Util.h"
#include "ImageWriter.h"
#include "ImageGen.h"
#include "ImageGenConfigurator.h"
#include "Image.h"

int main(int argc, char* argv[])
{
  if (argc != 8)
  {
    Error("Incorrect parameters. Usage: Flame <width> <height> <quality (1.0)> <config>");
  }

  const int w = atoi(argv[1]);
  const int h = atoi(argv[2]);
  const int imageWidth = std::max(16, w);
  const int imageHeight = std::max(16, h);
  const int plotWidth = imageWidth * 2;
  const int plotHeight = imageHeight * 2;

  float quality = atof(argv[3]);
  char* commands = argv[4];
  double postX = atof(argv[5]);
  double postY = atof(argv[6]);
  int numThreads = atoi(argv[7]);

  if (numThreads < 1)
  {
    numThreads = 1;
  }

  ImageGen gen(plotWidth, plotHeight, quality);
  Transforms::Ptr transforms = Transforms::Get();
  ImageGenConfigurator::Configure(commands, postX, postY, transforms, gen);
  
  Log("Performing %d iterations across %d threads...", gen.GetQuality(), numThreads);
  gen.SynthesizeAsync(numThreads);

  ToneMap<ColorRGBf> tm;

  // Fire
  /*tm.AddTone(0.0f, ColorRGBf(0.0f));
  tm.AddTone(0.2f, ColorRGBf(0.1f, 0.0f, 0.2f));
  tm.AddTone(0.5f, ColorRGBf(0.65f, 0.2f, 0.25f));
  tm.AddTone(0.8f, ColorRGBf(0.8f, 0.5f, 0.0f));
  tm.AddTone(1.0f, ColorRGBf(1.0f, 1.0f, 0.5f));*/
  
  // Jayleigh's Palette
  tm.AddTone(0.0f, ColorRGBf(0.0f));
  tm.AddTone(0.1f, ColorRGBf(0.0f, 0.1f, 0.2f));
  tm.AddTone(0.3f, ColorRGBf(0.0f, 0.2f, 0.35f));
  tm.AddTone(0.8f, ColorRGBf(0.2f, 0.7f, 0.0f));
  tm.AddTone(1.0f, ColorRGBf(1.0f, 1.0f, 0.0f));
  
  // Green & Blue
  /*tm.AddTone(0.0f, ColorRGBf(0.0f));
  tm.AddTone(0.05f, ColorRGBf(0.0f, 0.05f, 0.1f));
  tm.AddTone(0.2f, ColorRGBf(0.0f, 0.35f, 0.6f));
  tm.AddTone(0.5f, ColorRGBf(0.2f, 0.7f, 0.0f));
  tm.AddTone(1.0f, ColorRGBf(1.0f, 1.0f, 0.0f));*/

  // Fuji
  /*tm.AddTone(0.0f, ColorRGBf(0.0f));
  tm.AddTone(0.3f, ColorRGBf(0.05f, 0.1f, 0.125f));
  tm.AddTone(0.4f, ColorRGBf(0.1f, 0.05f, 0.35f));
  tm.AddTone(0.6f, ColorRGBf(0.4f, 0.1f, 0.6f));
  tm.AddTone(0.8f, ColorRGBf(0.8f, 0.35f, 0.5f));
  tm.AddTone(1.0f, ColorRGBf(1.0f, 0.8f, 1.0f));*/
  
  // Calibration
  //tm.AddTone(0.0f, ColorRGBf(0.0f, 0.0f, 0.0f));
  //tm.AddTone(0.5f, ColorRGBf(1.0f, 0.0f, 0.0f));
  //tm.AddTone(1.0f, ColorRGBf(1.0f, 1.0f, 1.0f));

  //tm.AddTone(0.0f, ColorRGBf(1.0f, 0.0f, 0.0f));
  //tm.AddTone(0.25f, ColorRGBf(1.0f, 0.0f, 1.0f));
  //tm.AddTone(0.5f, ColorRGBf(0.0f, 1.0f, 0.0f));
  //tm.AddTone(0.75f, ColorRGBf(1.0f, 0.0f, 1.0f));
  //tm.AddTone(1.0f, ColorRGBf(0.0f, 0.0f, 1.0f));

  Png<ColorRGBf> png(imageWidth, imageHeight);
  ImageWriter<ColorRGBf> writer(png, gen.GetPlot(), tm);
  writer.Write();

  Log("Saving...");
  png.Save("output.png");
  Log("Done");
}

