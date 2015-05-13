#pragma once
#include <string>
#include "Globals.h"

class Keyvalue
{
public:
	std::string key;
	std::string value;

	Keyvalue(std::string key, std::string value);
	Keyvalue(void);
	~Keyvalue(void);

	vec3 getVector();
};

