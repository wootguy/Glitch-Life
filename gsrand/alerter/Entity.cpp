#include "Entity.h"
#include <string>
#include "Util.h"

using namespace std;

Entity::Entity(void)
{
}


Entity::~Entity(void)
{
}

void Entity::addKeyvalue( Keyvalue& k )
{
	int dup = 1;
	if (keyvalues[k.key].length() <= 0)
		keyvalues[k.key] = k.value;
	else
	{
		while (true)
		{
			string newKey = k.key + '#' + to_string((_Longlong)dup);
			if (keyvalues[newKey].length() <= 0)
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
