/*******************************************************************************
 *
 *	filename:    Light.hpp
 *	author:      Do Won Cha
 *	content:     Light class to put in scenes
 *
 ******************************************************************************/

#pragma once
#ifndef _RAST_LIGHT_
#define _RAST_LIGHT_

#include <glm/vec3.hpp>

class Light
{
public:
    glm::vec3 pos;
    float intensity;

    Light(const glm::vec3 pos, float intensity) :
        pos(pos),
        intensity(intensity)
    { }

    Light(float x, float y, float z, float intensity) :
        pos(x, y, z),
        intensity(intensity)
    { }

    ~Light() {}
};

#endif // _RAST_LIGHT_
