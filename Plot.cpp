#include "Plot.h"
#include "Util.h"
#include <math.h>
#include <cassert>

Plot::Plot(int width, int height)
  : m_width(width)
  , m_height(height)
  , m_data(nullptr)
{
  m_data = new Plot::Data[m_width * m_height];
}

//-----------------------------------------------------------
Plot::~Plot()
{
  delete[] m_data;
  m_data = nullptr;
}

//-----------------------------------------------------------
void Plot::PostProcess()
{
  // Compute max
  m_max = 0;
  for (auto i = 0; i < m_width * m_height; ++i)
  {
    unsigned int hits = m_data[i].hits.load();
    if (hits > m_max)
    {
      m_max = hits;
    }
  }

  // Compute plot hit weights as a ratio of max
  for (auto i = 0; i < m_width * m_height; ++i)
  {
    float weight = static_cast<float>(m_data[i].hits.load()) / m_max;
    weight = 1.0f - weight;
    weight = 1.0f - pow(weight, 1400);
    weight = weight < 0.0f ? 0.0f : (weight > 1.0f ? 1.0f : weight);
    weight = pow(weight, 1.0 / 3.0);
    //Log("%f", weight);
    m_data[i].weight = weight;
  }
}

//-----------------------------------------------------------
const Plot::Data* Plot::GetRow(int rowIndex) const
{
  assert(rowIndex >= 0 && rowIndex < m_height);
  return &m_data[rowIndex * m_width];
}

//-----------------------------------------------------------
const Plot::Data* Plot::GetAt(int x, int y) const
{
  assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
  return &m_data[m_width * y + x];
}

