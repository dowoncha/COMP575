#pragma once

#include <algorithm>

#ifdef __WIN32
#define M_PI 3.141592653
#endif

namespace Util
{
	float PinToUnit(float in)
	{
		return std::max(0.0f, std::min(1.0f, in));
	}
} //namespace Util
