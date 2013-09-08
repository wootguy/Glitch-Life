#include "StdAfx.h"
#include "Keyvalue.h"
#include "Util.h"

Keyvalue::Keyvalue(void)
{
	key = value = "";
}

Keyvalue::Keyvalue( std::string key, std::string value )
{
	this->key = key;
	this->value = value;
}


Keyvalue::~Keyvalue(void)
{
}

vec3 Keyvalue::getVector()
{
	vec3 v;
	int coordidx = 0;
	int begin = -1;
	for (int i = 0, len = value.length(); i < len; i++)
	{
		char c = value[i];
		if (begin != -1)
		{
			if (i == len-1)
				i = len;
			if (c == ' ' || i == len)
			{
				float coord = atof(getSubStr(value,begin,i).c_str());
				if (coordidx == 0)
					v.x = coord;
				else if (coordidx == 1)
					v.y = coord;
				else if (coordidx == 2)
					v.z = coord;
				else
					err("too many coordinates in vertex");
				coordidx++;
				begin = -1;
			}
		}
		else if (isNumeric(c) || c == '.' || c == '-')
			begin = i;
	}
	if (coordidx < 3) err("Not enough coordinates in vector");
	return v;
}
