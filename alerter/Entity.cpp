#include "gsrand.h"
#include "Entity.h"
#include <string>
#include "Util.h"

using namespace std;

Entity::Entity(void)
{
}

Entity::Entity(const string& classname)
{
	addKeyvalue("classname", classname);
}

Entity::~Entity(void)
{
}

void Entity::addKeyvalue( Keyvalue& k )
{
	int dup = 1;
	if (keyvalues.find(k.key) == keyvalues.end())
		keyvalues[k.key] = k.value;
	else
	{
		while (true)
		{
			string newKey = k.key + '#' + to_string((_Longlong)dup);
			if (keyvalues.find(newKey) == keyvalues.end())
			{
				//println("wrote dup key " + newKey);
				keyvalues[newKey] = k.value;
				break;
			}
			dup++;
		}
	}
}

void Entity::addKeyvalue(const std::string& key, const std::string& value)
{
	keyvalues[key] = value;
}

bool Entity::hasKey(const std::string& key)
{
	return keyvalues.find(key) != keyvalues.end();
}
