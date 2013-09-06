#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include <string>
#include <map>
#include <math.h>

typedef std::function<void(double&, double&)> Transform;

namespace Transforms
{
  typedef std::map<std::string, Transform> Collection;
  typedef std::shared_ptr<Collection> Ptr;

  Ptr Get();
}

#endif

