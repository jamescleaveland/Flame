#ifndef __TONEMAP_H__
#define __TONEMAP_H__

#include <cassert>
#include <map>
#include <limits>
#include "Util.h"
#include <math.h>

template<typename T>
class ToneMap
{
  public:
    void AddTone(float t, T tone);
    T GetTone(float t);

  private:
    std::map<float, T> m_tones;
};

//-----------------------------------------------------------
template<typename T>
void ToneMap<T>::AddTone(float t, T tone)
{
  m_tones.emplace(t, tone);
}

//-----------------------------------------------------------
template<typename T>
T ToneMap<T>::GetTone(float t)
{
  assert(t >= 0.0f && t <= 1.0f);

  const float fMin = std::numeric_limits<float>::min();
  const float fMax = std::numeric_limits<float>::max();

  float min = fMax;
  T minTone;
  float max = fMin;
  T maxTone;

  for (auto& pair : m_tones)
  {
    if (pair.first <= t)
    {
      min = pair.first;
      minTone = pair.second;
    }

    if (pair.first >= t)
    {
      max = pair.first;
      maxTone = pair.second;
    }

    if (min != fMax && max != fMin)
    {
      break;
    }
  }

  assert(min != fMax && max != fMin);
  //Log("min: %2.2f max: %2.2f t: %2.2f", min, max, t);

  if (fabs(max - min) < fMin)
  {
    return minTone;
  }
  else
  {
    const float ratio = (t - min) / (max - min);
    return (minTone * (1.0f - ratio)) + (maxTone * ratio);
  }
}

#endif

