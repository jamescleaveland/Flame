#include "Plot.h"
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
void Plot::Record(const double& x, const double& y)
{
  int newVal;
  int i_x = static_cast<int>((x * 0.5 + 0.5) * (m_width - 1));
  int i_y = static_cast<int>((y * 0.5 + 0.5) * (m_height - 1));
  if (i_x >= 0 && i_x < m_width && i_y >= 0 && i_y < m_height)
  {
    newVal = ++m_data[i_y * m_width + i_x].hits;
  }

  if (newVal > m_max)
  {
    m_max = newVal;
  }
}

//-----------------------------------------------------------
void Plot::ComputeWeights()
{
  for (auto i = 0; i < m_width * m_height; ++i)
  {
    float weight = static_cast<float>(m_data[i].hits) / m_max;
    weight = 1.0f - weight;
    weight = 1.0f - pow(weight, 1400);
    weight = weight < 0.0f ? 0.0f : (weight > 1.0f ? 1.0f : weight);
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

