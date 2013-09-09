#include "Transforms.h"
#include <limits>
#include <random>

namespace Transforms
{
  static double k_pi = 3.1415926535897932384626433832795;
  static double k_eps = std::numeric_limits<double>::epsilon();
  static std::default_random_engine generator;
  std::uniform_int_distribution<int> bi(0, 1);

  //-----------------------------------------------------------
  inline double _r2(double& x, double& y)
  {
    return x * x + y * y;
  }

  //-----------------------------------------------------------
  inline double _r(double& x, double& y)
  {
    return sqrt(_r2(x, y));
  }

  //-----------------------------------------------------------
  inline double _theta(double& x, double& y)
  {
    return atan(x / y);
  }

  //-----------------------------------------------------------
  inline double _phi(double& x, double& y)
  {
    return atan(y / x);
  }

  //-----------------------------------------------------------
  inline double _omega()
  {
    return bi(generator) ? 0.0 : k_pi;
  }
  //-----------------------------------------------------------
#define T(name, code) \
  inline void name(double& x, double& y) \
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
  T(push,
    x += 0.5;
    y += 0.5;
  );

  //-----------------------------------------------------------
  T(serp1,
    x = x / 2.0;
    y = y / 2.0;
  );

  //-----------------------------------------------------------
  T(serp2,
    x = (x + 1.0) / 2.0;
    y = y / 2.0;
  );

  //-----------------------------------------------------------
  T(serp3,
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
    double r2 = _r2(x, y);
    double oneOverR2 = 1.0 / r2;
    x = oneOverR2 * x;
    y = oneOverR2 * y;
  );

  //-----------------------------------------------------------
  T(swirl,
    double r2 = _r2(x, y);
    x = x * sin(r2) - y * cos(r2);
    y = x * cos(r2) + y * sin(r2);
  );

  //-----------------------------------------------------------
  T(horseshoe,
    double oneOverR = 1.0 / _r(x, y);
    x = oneOverR * ((x - y) * (x + y));
    y = oneOverR * (2 * x * y);
  );

  //-----------------------------------------------------------
  T(polar,
    double theta = _theta(x, y);
    double r = _r(x, y);
    x = theta / k_pi;
    y = r - 1;
  );

  //-----------------------------------------------------------
  T(handkerchief,
    double r = _r(x, y);
    double theta = _theta(x, y);
    x = r * sin(theta + r);
    y = r * cos(theta - r);
  );

  //-----------------------------------------------------------
  T(heart,
    double r = _r(x, y);
    double theta = _theta(x, y);
    x = r * sin(theta * r);
    y = r * -cos(theta * r);
  );

  //-----------------------------------------------------------
  T(disc,
    double r = _r(x, y);
    double thetaOverPi = _theta(x, y) / k_pi;
    x = thetaOverPi * sin(k_pi * r);
    y = thetaOverPi * cos(k_pi * r);
  );

  //-----------------------------------------------------------
  T(spiral,
    double r = _r(x, y);
    double theta = _theta(x, y);
    double oneOverR = 1.0 / (r + k_eps);
    x = oneOverR * (cos(theta) + sin(r));
    y = oneOverR * (sin(theta) - cos(r));
  );

  //-----------------------------------------------------------
  T(hyperbolic,
    double r = _r(x, y);
    double theta = _theta(x, y);
    x = sin(theta) / r;
    y = r * cos(theta);
  );

  //-----------------------------------------------------------
  T(diamond,
    double r = _r(x, y);
    double theta = _theta(x, y);
    x = sin(theta) * cos(r);
    y = cos(theta) * sin(r);
  );

  //-----------------------------------------------------------
  T(ex,
    double r = _r(x, y);
    double theta = _theta(x, y);
    double p03 = pow(sin(theta + r), 3.0);
    double p13 = pow(cos(theta - r), 3.0);
    x = r * (p03 + p13);
    y = r * (p03 - p13);
  );

  //-----------------------------------------------------------
  T(julia,
    double sqrtR = sqrt(_r(x, y));
    double thetaOver2 = _theta(x, y) / 2.0;
    x = sqrtR * cos(thetaOver2 + _omega());
    y = sqrtR * sin(thetaOver2 + _omega());
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
    T(t, push);
    T(t, serp1);
    T(t, serp2);
    T(t, serp3);
    T(t, sine);
    T(t, cosine);
    T(t, tangent);
    T(t, spherical);
    T(t, swirl);
    T(t, horseshoe);
    T(t, polar);
    T(t, handkerchief);
    T(t, heart);
    T(t, disc);
    T(t, spiral);
    T(t, hyperbolic);
    T(t, diamond);
    T(t, ex);
    T(t, julia);

    return transforms;
  }
}

