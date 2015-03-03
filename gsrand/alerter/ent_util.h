#include "Keyvalue.h"
#include "Entity.h"
#include <string>

using namespace std;

struct BSP;

Keyvalue extractKeyvalue(string line);

void ripent(BSP * map, Entity** entData, bool restore);

bool needsRipent(BSP * map, Entity** ents);

Entity ** getMapEnts(BSP * map, bool printInfo, int& numEnts);

void update_changelevels(Entity** ents, string mapname);

void do_entity_randomization(Entity** ents, string mapname);

Entity * add_new_entity(Entity** current_ents, string classname);

int get_weapon_id(string weapon_name);
