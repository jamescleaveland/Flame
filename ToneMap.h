#ifndef __TONEMAP_H__
#define __TONEMAP_H__

#include <cassert>
#include <map>
#include <limits>
#include "Util.h"

template<typename T>
class ToneMap
{
  public:
    inline void AddTone(float t, T tone) {m_tones.emplace(t, tone);}
    T GetTone(float t);

  private:
    std::map<float, T> m_tones;
};

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
    if (min == fMax && pair.first <= t)
    {
      min = pair.first;
      minTone = pair.second;
    }

    if (max == fMin && pair.first >= t)
    {
      max = pair.first;
      maxTone = pair.second;
    }
  }

  assert(min != fMax && max != fMin);
  //Log("min: %2.2f max: %2.2f t: %2.2f", min, max, t);

  const float ratio = (t - min) / (max - min);
  return (minTone * (1.0f - ratio)) + (maxTone * ratio);
}

#endif

