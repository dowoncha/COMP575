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
	int specPower;

    Light(const glm::vec3& _pos, float _intensity) :
        pos(_pos),
        intensity(_intensity),
		specPower(32)
    { }

	Light(const glm::vec3& _pos, float _intensity, int pow) :
		pos(_pos),
		intensity(_intensity),
		specPower(pow)
	{ }

    Light(float x, float y, float z, float _intensity) :
        pos(x, y, z),
        intensity(_intensity)
    { }

    ~Light() {}
};

#endif // _RAST_LIGHT_
