#include "Transforms.h"

namespace Transforms
{
  //-----------------------------------------------------------
#define T(name, code) \
  void name(double& x, double& y) \
  { code }

  //-----------------------------------------------------------
  T(a1,
    x = x / 2.0;
    y = y / 2.0;
  );

  //-----------------------------------------------------------
  T(a2,
    x = (x + 1.0) / 2.0;
    y = y / 2.0;
  );

  //-----------------------------------------------------------
  T(a3,
    x = x / 2.0;
    y = (y + 1.0) / 2.0;
  );

  //-----------------------------------------------------------
  T(sine,
    x = sin(x);
    y = sin(y);
  );

  //-----------------------------------------------------------
  T(spherical,
    double r = sqrt(x * x + y * y);
    double oneOverR2 = 1.0 / (r * r);
    x = oneOverR2 * x;
    y = oneOverR2 * y;
  );

  //-----------------------------------------------------------
#undef T
#define T(collection, name) \
  collection.emplace(#name, std::bind(name, std::placeholders::_1, std::placeholders::_2));

  //-----------------------------------------------------------
  Ptr Get()
  {
    Ptr transforms = std::make_shared<Collection>();
    auto& t = *transforms;

    T(t, a1);
    T(t, a2);
    T(t, a3);
    T(t, sine);
    T(t, spherical);

    return transforms;
  }
}

