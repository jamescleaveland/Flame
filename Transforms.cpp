#include "Transforms.h"

namespace Transforms
{
#define PI 3.1415926535897932384626433832795;

  //-----------------------------------------------------------
  inline double r2(double& x, double& y)
  {
    return x * x + y * y;
  }

  //-----------------------------------------------------------
  inline double r(double& x, double& y)
  {
    return sqrt(r2(x, y));
  }

  //-----------------------------------------------------------
  inline double theta(double& x, double& y)
  {
    return atan(x / y);
  }

  //-----------------------------------------------------------
  inline double phi(double& x, double& y)
  {
    return atan(y / x);
  }

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
    double dr2 = r2(x, y);
    double oneOverR2 = 1.0 / dr2;
    x = oneOverR2 * x;
    y = oneOverR2 * y;
  );

  //-----------------------------------------------------------
  T(swirl,
    double dr2 = r2(x, y);
    x = x * sin(dr2) - y * cos(dr2);
    y = x * cos(dr2) + y * sin(dr2);
  );

  //-----------------------------------------------------------
  T(horseshoe,
    double oneOverR = 1.0 / r(x, y);
    x = oneOverR * ((x - y) * (x + y));
    y = oneOverR * (2 * x * y);
  );

  //-----------------------------------------------------------
  T(polar,
    double theta = atan(x / y);
    double dr = r(x, y);
    x = theta / PI;
    y = dr - 1;
  );

  //-----------------------------------------------------------
  T(handkerchief,
    double dr = r(x, y);
    double dtheta = theta(x, y);
    x = dr * sin(dtheta + dr);
    y = dr * cos(dtheta - dr);
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
    T(t, polar);
    T(t, handkerchief);

    return transforms;
  }
}

