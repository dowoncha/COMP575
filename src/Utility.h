/**
 *
 *  filename    : Utility.h
 *  author      : Do Won Cha
 *  content     : functions that are used throughout
 */

#pragma once
#ifndef _RAY_UTIL_
#define _RAY_UTIL_

#ifdef _WIN32
#define M_PI 3.14159265359
//#define NOMINMAX
#elif __unix__ || __APPLE__
#endif

//#include <cstdint>
#include <algorithm>

namespace Utility
{

template<typename T>
inline T clamp(T min, T value, T max)
{
	// stupid windows requires me to do this because of the min macro defined in windef.
	// I don't use windef anywhere though...
    return (std::max)(min, (std::min)(value, max));
}

static inline float PinToUnit(float x)
{
    return (std::max)(0.0f, (std::min)(1.0f, x));
}

inline float floor_clamp(float value)
{
    return value - std::floor(value);
}

template<typename T>
inline T lerp(T v0, T v1, float t) {
    return (1.0f - t) * v0 + t * v1;
}

inline uint8_t floatToByte(float i)
{
  int isx = (int)(i * 255.99999f);
  return (uint8_t)(isx & 0xff);
}

}   // end of namespace Util

#endif // end of header guard _RAY_UTIL_
