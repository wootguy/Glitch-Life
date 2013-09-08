#pragma once
#include "Keyvalue.h"
#include <unordered_map>

typedef std::tr1::unordered_map< std::string, std::string > hashmap;

class Entity
{
public:
	hashmap keyvalues;

	Entity(void);
	~Entity(void);

	void addKeyvalue(Keyvalue& k);
	void addKeyvalue(const std::string& key, const std::string& value);
};

