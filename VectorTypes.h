#ifndef __VECTORTYPES_H__
#define __VECTORTYPES_H__

template<typename T>
struct Vec3_tpl
{
  Vec3_tpl(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
  Vec3_tpl(T _v) : x(_v), y(_v), z(_v) {}
  Vec3_tpl() : x(), y(), z() {}
  inline T* elems() { return &x; }

  union { T x, r; };
  union { T y, g; };
  union { T z, b; };
};

//-----------------------------------------------------------
template<typename T>
struct Vec4_tpl
{
  Vec4_tpl(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
  Vec4_tpl(T _v) : x(_v), y(_v), z(_v), w(_v) {}
  Vec4_tpl() : x(), y(), z(), w() {}
  inline T* elems() { return &x; }
  
  union { T x, r; };
  union { T y, g; };
  union { T z, b; };
  union { T w, a; };
};

//-----------------------------------------------------------
struct Vec3 : public Vec3_tpl<float>
{
  Vec3(float _x, float _y, float _z)
    : Vec3_tpl<float>(_x, _y, _z)
  {}

  Vec3(float _v)
    : Vec3_tpl<float>(_v)
  {}

  Vec3() : Vec3_tpl<float>() {}

  inline Vec3 operator*(float scalar)
  {
    return Vec3(x * scalar, y * scalar, z * scalar);
  }

  inline Vec3 operator+(Vec3 other)
  {
    return Vec3(x + other.x, y + other.y, z + other.z);
  }
};

//-----------------------------------------------------------
struct Vec4 : public Vec4_tpl<float>
{
  Vec4(float _x, float _y, float _z, float _w)
    : Vec4_tpl<float>(_x, _y, _z, _w)
  {}

  Vec4(float _v)
    : Vec4_tpl<float>(_v)
  {}

  Vec4() : Vec4_tpl<float>() {}
};

//-----------------------------------------------------------
typedef Vec3 ColorRGBf;
typedef Vec4 ColorRGBAf;
//-----------------------------------------------------------

#endif

