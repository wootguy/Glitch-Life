#pragma once
#include <string>
#include <vector>

#define OS_WIN

struct vec3
{
	float x, y, z;
	vec3() : x(), y(), z() {}
	vec3( float x, float y, float z ) : x( x ), y( y ), z( z ) {}
};