/*********************************************************************************
 *
 *  filename : Primitives.h
 *  author   : Do Won Cha
 *  content  : Contains primitives to use for math
 *
****************************************************************************************/

#pragma once

#ifndef _RAY_PRIM_
#define _RAY_PRIM_

#include <cmath>
#include <iostream>

// forward declaration of template classes
template<class T> class Vector2;
template<class T> class Vector3;
template<class T> class Vector4;

template<class T>
class Vector2
{
public:
  typedef T ElementType;
  T x, y;
public:
  Vector2()           : x(T(0)), y(T(0)) { }
  Vector2(T s)        : x(s), y(s) { }
  Vector2(T x_, T y_) : x(x_), y(y_) { }
  Vector2(const Vector2& b) : x(b.x), y(b.y) { }

  ~Vector2() { }

  Vector2  operator +  (const Vector2& b) { return Vector2(x + b.x, y + b.y); }
  Vector2& operator += (const Vector2& b) { x += b.x; y += b.y; return *this; }
  Vector2  operator -  (const Vector2& b) { return Vector2(x - b.x, y - b.y); }
  Vector2& operator -= (const Vector2& b) { x -= b.x; y -= b.y; return *this; }

  Vector2  operator *  (T s) { return Vector2(x * s, y * s); }
  Vector2& operator *= (T s) { x *= s; y *= s; return *this;}

  T GetLength() { return std::sqrt(x * x + y * y); }

  void Normalize() {
    T len = GetLength();
    if ( len != T(0) )
    {
      x /= len;
      y /= len;
    }
  }

  T DotProduct(const Vector2& b) { return x * b.x + y * b.y; }
};

typedef Vector2<float> Vector2f;
typedef Vector2<int>   Vector2i;

template<class T>
class Vector3
{
public:
  typedef T ElementType;
  T x, y, z;
public:
  Vector3() : x(T(0)), y(T(0)), z(T(0)) {}
  Vector3(T s)              : x(s), y(s), z(s) { }
  Vector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) { }
  Vector3(const Vector3& b) : x(b.x), y(b.y), z(b.z) { }

  static Vector3 Zero() { return Vector3(T(0), T(0), T(0)); }

  ~Vector3() { }

  Vector3  operator +  (const Vector3& b) const { return Vector3(x + b.x, y + b.y, z + b.z); }
  Vector3& operator += (const Vector3& b) { x += b.x; y += b.y; z += b.z; return *this; }
  Vector3  operator -  (const Vector3& b) const { return Vector3(x - b.x, y - b.y, z - b.z); }
  Vector3& operator -= (const Vector3& b) { x -= b.x; y -= b.y; z -= b.z; return *this; }

  Vector3  operator -  () const { return Vector3(-x, -y, -z); }
  //Vector3& operator -  () { x = -x; y = -y; z = -z; }

  Vector3  operator *  (T s) const { return Vector3(x * s, y * s, z * s);  }
  Vector3& operator *= (T s) { x *= s; y *= s; z *= s; return *this; }

  Vector3  operator /  (T s) const { return Vector3(x / s, y / s, z / s); }

  // Vector3 multiply operation is the dot product
  T  operator *  (const Vector3& b) const { return DotProduct(b); }

  T Length()
  {
	  return std::sqrt(x * x + y * y + z * z);
  }

  friend std::ostream& operator << (std::ostream& s, const Vector3& a)
  {
	  return s << a.x << ' ' << a.y << ' ' << a.z;
  }

  void Normalize()
  {
    T len = std::sqrt(x * x + y * y + z * z);
    if ( len != T(0) )
    {
      x /= len;
      y /= len;
      z /= len;
    }
  }

  Vector3 Normalized() const
  {
    T len = std::sqrt(x * x + y * y + z * z);
    if ( len != T(0))
      return Vector3(x / len, y / len, z / len);
    else
      return *this;
  }

  T DotProduct(const Vector3& b) const { return x * b.x + y * b.y + z * b.z; }
};

typedef Vector3<float> Vector3f;
typedef Vector3<int>   Vector3i;

template<class T>
class Vector4
{
public:
  typedef T ElementType;
  T x, y, z, w;
public:
  Vector4() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) { }
  Vector4(T s) : x(s), y(s), z(s), w(s) { }
  Vector4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) { }
  Vector4(const Vector3<T>& a, T w_) : x(a.x), y(a.y), z(a.z), w(w_) { }
  Vector4(const Vector4& b) : x(b.x), y(b.y), z(b.z), w(b.w)  { }

  ~Vector4() {}

  Vector4  operator +  (const Vector4& b) { return Vector4(x + b.x, y + b.y, z + b.z, w + b.w);  }
  Vector4& operator += (const Vector4& b) { x += b.x; y += b.y; z += b.z; w += b.w; return *this; }
  Vector4  operator -  (const Vector4& b) { return Vector4(x - b.x, y - b.y, z - b.z, w - b.w);  }
  Vector4& operator -= (const Vector4& b) { x -= b.x; y -= b.y; z -= b.z; w -= b.w; return *this; }

  Vector4  operator *  (T s) { return Vector4(x * s, y * s, z * s, w * s); }
  Vector4& operator *= (T s) { x *= s; y *= s; z *= s; w *= s; return *this; }

  T DotProduct(const Vector4& b) { return x * b.x + y * b.y + z * b.z + w * b.w; }

  void Normalize()
  {
    T len = std::sqrt(x * x + y * y + z * z + w * w);
    if ( len != T(0) )
    {
      x /= len;
      y /= len;
      z /= len;
      w /= len;
    }
  }

  Vector4 Normalized() const
  {
    T len = std::sqrt(x * x + y * y + z * z + w * w);
    if ( len != T(0))
      return Vector4(x / len, y / len, z / len, w / len);
    else
      return *this;
  }

  friend std::ostream& operator << (std::ostream& s, const Vector4& a)
  {
	  return s << a.x << ' ' << a.y << ' ' << a.z << ' ' << a.w;
  }
};

typedef Vector4<float> Vector4f;
typedef Vector4<int> Vector4i;

#endif		// RAY_PRIM header
