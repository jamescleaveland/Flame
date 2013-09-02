#include "Transforms.h"

namespace Transforms
{
  //-----------------------------------------------------------
#define T(name, code) \
  void name(double& x, double& y) \
  { code }

  //-----------------------------------------------------------
  T(half,
    x = x / 2.0;
    y = y / 2.0;
  );

  //-----------------------------------------------------------
  T(dbl,
    x *= 2.0;
    y *= 2.0;
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
  T(cosine,
    x = cos(x);
    y = cos(y);
  );

  //-----------------------------------------------------------
  T(tangent,
    x = tan(x);
    y = tan(y);
  );

  //-----------------------------------------------------------
  T(spherical,
    double r2 = x * x + y * y;
    double oneOverR2 = 1.0 / r2;
    x = oneOverR2 * x;
    y = oneOverR2 * y;
  );

  //-----------------------------------------------------------
  T(swirl,
    double r2 = x * x + y * y;
    x = x * sin(r2) - y * cos(r2);
    y = x * cos(r2) + y * sin(r2);
  );

  //-----------------------------------------------------------
  T(horseshoe,
    double oneOverR = 1.0 / sqrt(x * x + y * y);
    x = oneOverR * ((x - y) * (x + y));
    y = oneOverR * (2 * x * y);
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

    T(t, half);
    T(t, dbl);
    T(t, a2);
    T(t, a3);
    T(t, sine);
    T(t, cosine);
    T(t, tangent);
    T(t, spherical);
    T(t, swirl);
    T(t, horseshoe);

    return transforms;
  }
}

