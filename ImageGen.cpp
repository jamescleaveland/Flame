#include "ImageGen.h"
#include "Perf.h"
#include <cassert>
#include <future>
#include <chrono>

ImageGen::ImageGen(int width, int height, float quality)
  : m_width(width)
  , m_height(height)
  , m_quality(m_width * m_height * quality)
  , m_postX(0.0)
  , m_postY(0.0)
  , m_plot(m_width, m_height)
{}

//-----------------------------------------------------------
void ImageGen::SynthesizeAsync(const int numThreads)
{
  assert(numThreads > 0);
  std::vector<std::future<void>> futures;
  futures.reserve(numThreads - 1);
  const int threadLoad = m_quality / numThreads;

  auto work = [&](unsigned int seed)
  {
    std::default_random_engine generator(seed + 1);
    std::uniform_real_distribution<double> doubleDist;

    double x = doubleDist(generator);
    double y = doubleDist(generator);

    std::uniform_int_distribution<int> dist(0, m_affineTransforms.size() - 1);

    for (auto j = 0; j < threadLoad; ++j)
    {
      // Affine transform chosen chaotically
      int i = dist(generator);
      (*m_affineTransforms[i])(x, y);

      // Non-linear transforms executed afterwards
      for (auto t : m_nonLinearTransforms)
      {
        (*t)(x, y);
      }
      
      m_plot.Record(x + m_postX, y + m_postY);
    }
  };

  PERF_START_TIMER("Synthesize started...");

  // Assign work to the main thread alongside async tasks
  for (auto i = 0; i < numThreads; ++i)
  {
    if (i != numThreads - 1)
    {
      futures.push_back(std::async(work, i));
    }
    else
    {
      work(i);
    }
  }

  for (auto& f : futures)
  {
    f.wait();
  }

  PERF_END_TIMER("Synthesize took");

  // Not thread-safe:
  m_plot.PostProcess();
}

