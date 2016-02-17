/**
 *
 *  filename    : Utility.h
 *  author      : Do Won Cha
 *  content     : functions that are used throughout
 */

#pragma once
#ifndef _RAY_UTIL_
#define _RAY_UTIL_

namespace Utility
{

template<typename T>
inline T clamp(T min, T value, T max)
{
    // WinDef.h has max as a macro, stupid but parantheses are necessary
    return (std::max)(min, (std::min)(value, max));
}

static inline float PinToUnit(float x)
{
    // WinDef.h has max as a macro, stupid but parantheses are necessary
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
